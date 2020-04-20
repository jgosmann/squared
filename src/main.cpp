/***************************************************************************
 * Project: squared                                                        *
 * Filename: main.cpp                                                      *
 * Begin: Di Feb 15 17:47:16 CET 2005                                      *
 * Description: Umsetzung des Spiels MetaSquares mit einem Computergegener.*
 ***************************************************************************
 * Authors: Jan Gosmann (jan@hyper-world.de)                               *
 ***************************************************************************
 * Copyright (C) 2005, 2008 Jan Gosmann                                    *
 ***************************************************************************/
/*  This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

void exit_error (const char *msg);

/***** Includes *****/
#include <iostream>
#include <string>

#include "config.h"

#if ALLEGRO_SUPPORT == 1
 #include <allegro.h>
 #include "metasquares_gfx.h"
#endif

#include "metasquares_txt.h"

/***** Defines *****/
#define PRGM_NAME "Squared"
#define PRGM_VERSION "2.2"

#ifndef EXIT_SUCCESS
 #define EXIT_SUCCESS 0
#endif
#ifndef EXIT_FAILURE
 #define EXIT_FAILURE 1
#endif

#define CURSOR_FOCUS_X 9
#define CURSOR_FOCUS_Y 11

/***** Misc *****/
using namespace std;

/***** Variables *****/
bool ui_mode = 1; // 0 = Konsolenmodus, 1 = GUI-Modus
int gfx_mode = 0; // 0 = Fenster, 1 = Vollbild
bool enable_alpha_blending = 1; // Alpha-Blending-FX?

#if ALLEGRO_SUPPORT == 1
 BITMAP *cursor; // Grafik für den Cursor
#endif

/***** Functions *****/
unsigned int is_option (char short_opt, string long_opt, int argc, char *argv[]);

//============================================================================
// Global
//============================================================================
//----------------------------------------------------------------------------
// main ()
//----------------------------------------------------------------------------
int
main (int argc, char *argv[])
{
  /*** Variables ***/
  unsigned int start_player = 3; // Startspieler
  unsigned int rec_depth = 0; // Rekursionstiefe
  
  /*** Commands ***/
  cout << "squared Copyright (C) 2005, 2008 Jan Gosmann" << endl;
  cout << "This program comes with ABSOLUTELY NO WARRANTY; for details see COPYING." << endl;
  cout << "This is free software, and you are welcome to redistribute it" << endl;
  cout << "under certain conditions." << endl << endl;
  cout << "Welcome to " << PRGM_NAME << " v" << PRGM_VERSION
       << ", Copyright (C) 2005, 2008 Jan Gosmann" << endl;
  cout << "This application was programmend with C++ and the great "
       << "Allegro-library!" << endl;
  cout << "Graphics by Yujiang Wang." << endl << endl;
  cout << "Please visit the following websites:" << endl;
  cout << "www.hyper-world.de" << endl;
  cout << "www.simple-graphix.de" << endl;
  cout << "alleg.source-forge.net" << endl;
  cout << "www.allegro.cc" << endl << endl;
  cout << "If you have any suggestions or found an bug, mail to:" << endl;
  cout << "jan@hyper-world.de" << endl << endl;
       
  /* Grafik-Optionen auslesen */
  if (is_option ('t', "text", argc, argv) > 0)
    ui_mode = 0;
  if (is_option ('f', "fullscreen", argc, argv) > 0)
    gfx_mode = 1;
  if (is_option ('a', "noalpha", argc, argv) > 0)
    enable_alpha_blending = 0;
    
  /* Weitere Optionen */
  if (is_option ('s', "startplayer", argc, argv) > 0)
    start_player = atoi (argv[is_option ('s', "startplayer", argc, argv) + 1]);
  switch (start_player)
    {
      case 1:
      case 2:
        start_player--;
        break;
      default:
        start_player = 2;
    }
    
  if (is_option ('r', "recursion-depth", argc, argv) > 0)
    rec_depth = atoi (argv[is_option ('r', "recursion-depth", argc, argv) + 1]);
  
  /* Initialisiere Allegro */
  if (ui_mode)
    {
      #if ALLEGRO_SUPPORT == 1
        allegro_init ();
        set_color_depth (32);
        if (gfx_mode)
          {
            if (set_gfx_mode (GFX_AUTODETECT_FULLSCREEN, 640, 480, 0, 0) < 0)
              exit_error ("Grafischer Modus konnte nicht initialisiert werden!");
          }
        else
          {
            if (set_gfx_mode (GFX_AUTODETECT_WINDOWED, 640, 480, 0, 0) < 0)
              exit_error ("Grafischer Modus konnte nicht initialisiert werden!");
          }
        if (install_timer () < 0)
          exit_error ("Timer konnte nicht initialisiert werden!");
        if (install_mouse () < 0)
          exit_error ("Maus konnte nicht initialisiert werden!");
        if( !(cursor = load_bitmap("cursor.bmp", NULL)) )
          exit_error( "cursor.bmp not found!" );
        set_mouse_sprite (cursor);
        set_mouse_sprite_focus (CURSOR_FOCUS_X, CURSOR_FOCUS_Y);
        if (install_keyboard () < 0)
          exit_error ("Tastatur konnte nicht initialisiert werden!");
        metasquares_gfx *game = new metasquares_gfx (enable_alpha_blending);
                                                     // Das Metasquares-Spiel
        game->start_game (start_player, rec_depth);
        delete game;
      #else
        exit_error ("Diese Squared-Version wurde nicht mit der Unterstützung für den grafischen Modus kompiliert.");
      #endif
    }
  else
    {
      metasquares_txt *game = new metasquares_txt (); // Das Metasquares-Spiel
      game->start_game (start_player, rec_depth);
      delete game;
    }

  /* Aufräumen */
  return EXIT_SUCCESS;
}
#if ALLEGRO_SUPPORT == 1
 END_OF_MAIN();
#endif

//----------------------------------------------------------------------------
// Überprüft, ob eine Option beim Programmstart angegeben wurde.
// Eingabe: short_opt = Option in kurzer Schreibeweise
//          long_opt = Option in langer Schreibweise
//          argc = argc aus der main()-Funktion
//          argv = argv aus der main()-Funktion
// Rückgabe: Erstes Vorkommen der Option in argv oder 0 falls die Option nicht
//           angegeben wurde.
//----------------------------------------------------------------------------
unsigned int
is_option (char short_opt, string long_opt, int argc, char *argv[])
{
  /*** Variables ***/
  int i, j; // Zählvariablen

  /*** Commands ***/
  for (i = 1; i < argc; i++)
    {
      if (argv[i][0] == '-') // Wenn überhaupt eine Option
        {
          if (argv[i][1] == '-') // Wenn lange Schreibweise
            {
              if ("--" + long_opt == argv[i] && long_opt != "")
                return i;
            }
          else // Wenn kurze Schreibweise
            {
              /* Das Ende von argv wird in der nachfolgenden for-Schleife
               * nicht mit strlen() überprüft, damit nicht die C-String-
               * Funktionen zusätzlich eingebunden werden müssen.
               */
              for (j = 1; argv[i][j] != '\0'; j++)
                {
                  if (short_opt == argv[i][j])
                    return i;
                }
            }
        }
    }

  return 0;
}

//----------------------------------------------------------------------------
// Gibt eine Fehlermeldung aus und beendet das Programm.
// Eingabe: msg = Fehlermeldung
//----------------------------------------------------------------------------
void
exit_error (const char *msg)
{
  cerr << PRGM_NAME << ": " << msg << endl << endl;
  exit (EXIT_FAILURE);
}
