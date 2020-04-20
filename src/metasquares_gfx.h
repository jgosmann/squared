/***************************************************************************
 * Project: squared                                                        *
 * Filename: metasquares_gfx.h                                             *
 * Begin: Mo Mar 21 08:10:26 CET 2005                                      *
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

#ifndef METASQUARES_GFX_H
#define METASQUARES_GFX_H 1

#ifndef ALLEGRO_H
 #error metasquares_gfx.h: allegro.h has to be included
#else

/***** Includes *****/
#include <cmath>

#include <cstdio>

#include "metasquares.h"

/***** Defines *****/
/* Dateinamen */
#define BMP_BGR_FILENAME "back.bmp"
#define BMP_SQR_FILENAME "sqr.bmp"
#define BMP_PLR1_FILENAME "plr1.tga"
#define BMP_PLR2_FILENAME "plr2.tga"

/* Farben */
#define COL_PLR1 makecol (50, 92, 140)
#define COL_PLR2 makecol (78, 140, 50)
#define COL_BGR makecol (191, 207, 222)

/* Positionen und Größen */
#define SQR_SIZE 400
#define SQR_X_OFFSET 20
#define SQR_Y_OFFSET 30
#define SCOREOUT_X_OFFSET 447
#define SCOREOUT_Y_OFFSET 37
#define TL_X_OFFSET 447
#define TL_Y_OFFSET 67
#define TL_WIDTH 165
#define BGR_WIDTH 640
#define BGR_HEIGHT 480

/***** Classes *****/
class metasquares_gfx : public metasquares {
  public:
    /*** Functions ***/
    metasquares_gfx (bool alpha_blend = 1);
    ~metasquares_gfx (void);
    
    void start_game (unsigned int set_str_plr_to = 2, unsigned int rec_depth = 0);
    
    friend int d_metasquare_proc (int msg, DIALOG *d, int c);
    friend int d_scoreout_proc (int msg, DIALOG *d, int c);
    friend int d_output_turn_list_proc (int msg, DIALOG *d, int c);

  protected:
    /*** Variables ***/
    BITMAP *bmp_bgr; // Hintergrund
    BITMAP *bmp_sqr; // Spielfeldgrafik
    BITMAP *bmp_plr[2]; // Spielsteine der Spieler
    int col_plr[2]; // Farben der Spieler
    
    bool alpha_blending;
};

/***** Functions *****/
//============================================================================
// Allegro-Dialog-Handler
//============================================================================
//----------------------------------------------------------------------------
// Allegro-Handler für das Spielfeld
//----------------------------------------------------------------------------
int
d_metasquare_proc (int msg, DIALOG *d, int c)
{
  /*** Variables ***/
  metasquares_gfx *obj = (metasquares_gfx *) d->dp; // Objekt, für dass ge-
                                                    // zeichnet wird
  pos_t pos; // Position, auf die gesetzt wird
  int x_dif, y_dif; // x- u. y-Differenz zweier Punkte
  BITMAP *buffer; // Bitmap für doppelte Pufferung
  BITMAP *alpha_buffer; // Zeichenroutinen von Allegro zeichnen hierdrauf und
                        // mit draw_trans_sprite kann dann eine Transparenz
                        // hinzugefügt werden.
  unsigned int i, j, k, l, m; // Zählvariablen

  /*** Commands ***/
  switch (msg)
    {
      case MSG_CLICK:
        pos.x = (unsigned int) floor ((double) (gui_mouse_x () - d->x) / (d->w / SQUARE_SIZE));
        pos.y = (unsigned int) floor ((double) (gui_mouse_y () - d->y) / (d->h / SQUARE_SIZE));
        if (obj->make_move (&pos))
          {
            if (obj->is_game_end ())
              {
                broadcast_dialog_message(MSG_DRAW, 0);
                return D_CLOSE;
              }
            if (obj->recursion_depth > 0)
              {
                obj->make_ai_move (obj->act_player);
                if (obj->is_game_end ())
                  {
                    broadcast_dialog_message(MSG_DRAW, 0);
                    return D_CLOSE;
                  }
              }

            return D_REDRAW;
          }
        break;
      case MSG_DRAW:
        buffer = create_bitmap (d->w, d->h);
        alpha_buffer = create_bitmap (d->w, d->h);
        stretch_sprite (buffer, obj->bmp_sqr, 0, 0, d->w, d->h);

        set_alpha_blender ();
        for (i = 0; i < 2; i++)
          {
            for (j = 0; j < obj->plr_positions[i].size (); j++)
              draw_trans_sprite (buffer, obj->bmp_plr[i],
                                 obj->plr_positions[i][j].x * (d->w / SQUARE_SIZE),
                                 obj->plr_positions[i][j].y * (d->h / SQUARE_SIZE));
          }

        set_trans_blender (0, 0, 0, 20);
        for (i = 0; i < 2; i++)
          {
            for (j = 0; j < obj->plr_squares[i].size (); j++)
              {
                clear_to_color (alpha_buffer, makecol (255, 0, 255));
                for (k = 0; k < 4 && obj->plr_squares[i][j].status == 4; k++)
                  {
                    for (l = 0; l < 4; l++)
                      {
                        if (l == k)
                          continue;
                          
                        for (m = l + 1; m < 4; m++)
                          {
                            if (m == k)
                              continue;

                            if (obj->_round (obj->_intersection_angle (&obj->plr_squares[i][j].positions[k],
                                                                       &obj->plr_squares[i][j].positions[l],
                                                                       &obj->plr_squares[i][j].positions[k],
                                                                       &obj->plr_squares[i][j].positions[m]))
                                == 90)
                              {
                                if (obj->alpha_blending)
                                  {
                                    triangle (alpha_buffer, obj->plr_squares[i][j].positions[k].x * (d->h / SQUARE_SIZE) + (d->h / SQUARE_SIZE / 2),
                                                            obj->plr_squares[i][j].positions[k].y * (d->w / SQUARE_SIZE) + (d->w / SQUARE_SIZE / 2),
                                                            obj->plr_squares[i][j].positions[l].x * (d->h / SQUARE_SIZE) + (d->h / SQUARE_SIZE / 2),
                                                            obj->plr_squares[i][j].positions[l].y * (d->w / SQUARE_SIZE) + (d->w / SQUARE_SIZE / 2),
                                                            obj->plr_squares[i][j].positions[m].x * (d->h / SQUARE_SIZE) + (d->h / SQUARE_SIZE / 2),
                                                            obj->plr_squares[i][j].positions[m].y * (d->w / SQUARE_SIZE) + (d->w / SQUARE_SIZE / 2) ,
                                              obj->col_plr[i]);
                                    floodfill (alpha_buffer, (int) (0.5 * (obj->plr_squares[i][j].positions[k].x
                                                                           + 0.5 * (obj->plr_squares[i][j].positions[l].x
                                                                                    + obj->plr_squares[i][j].positions[m].x))
                                                                    * (d->h / SQUARE_SIZE) + (d->h / SQUARE_SIZE / 2)),
                                                             (int) (0.5 * (obj->plr_squares[i][j].positions[k].y
                                                                           + 0.5 * (obj->plr_squares[i][j].positions[l].y
                                                                                    + obj->plr_squares[i][j].positions[m].y))
                                                                * (d->w / SQUARE_SIZE) + (d->w / SQUARE_SIZE / 2)),
                                               obj->col_plr[i]);
                                  }
                                
                                line (buffer, obj->plr_squares[i][j].positions[k].x * (d->h / SQUARE_SIZE) + (d->h / SQUARE_SIZE / 2),
                                              obj->plr_squares[i][j].positions[k].y * (d->w / SQUARE_SIZE) + (d->w / SQUARE_SIZE / 2),
                                              obj->plr_squares[i][j].positions[l].x * (d->h / SQUARE_SIZE) + (d->h / SQUARE_SIZE / 2),
                                              obj->plr_squares[i][j].positions[l].y * (d->w / SQUARE_SIZE) + (d->w / SQUARE_SIZE / 2),
                                      obj->col_plr[i]);
                                line (buffer, obj->plr_squares[i][j].positions[k].x * (d->h / SQUARE_SIZE) + (d->h / SQUARE_SIZE / 2),
                                              obj->plr_squares[i][j].positions[k].y * (d->w / SQUARE_SIZE) + (d->w / SQUARE_SIZE / 2),
                                              obj->plr_squares[i][j].positions[m].x * (d->h / SQUARE_SIZE) + (d->h / SQUARE_SIZE / 2),
                                              obj->plr_squares[i][j].positions[m].y * (d->w / SQUARE_SIZE) + (d->w / SQUARE_SIZE / 2),
                                      obj->col_plr[i]);
                              }
                          }
                      }
                  }
                if (obj->alpha_blending)
                  draw_trans_sprite (buffer, alpha_buffer, 0, 0);
              }
          }
        draw_sprite (gui_get_screen (), buffer, d->x, d->y);
        destroy_bitmap (buffer);
        destroy_bitmap (alpha_buffer);
        break;
    }
  
  return D_O_K;
}

//----------------------------------------------------------------------------
// Allegro-Handler für die Punkteanzeige
//----------------------------------------------------------------------------
int
d_scoreout_proc (int msg, DIALOG *d, int c)
{
  /*** Variables ***/
  char output_buffer[64];
  metasquares_gfx *obj = (metasquares_gfx *) d->dp; // Objekt, für dass ge-
                                                    // zeichnet wird
  /*** Commands ***/
  if (msg == MSG_DRAW)
    {
      sprintf (output_buffer, "Spieler 1: %i", obj->score[0][3]);
      textout_ex(gui_get_screen (), font, output_buffer, d->x, d->y, COL_PLR1, -1);
      sprintf (output_buffer, "Spieler 2: %i", obj->score[1][3]);
      textout_ex(gui_get_screen (), font, output_buffer, d->x, d->y + 10, COL_PLR2, -1);
    }

  return D_O_K;
}

//----------------------------------------------------------------------------
// Allegro-Handler, um den Bildschirm zu aktualisieren
//----------------------------------------------------------------------------
int
d_update_screen_proc (int msg, DIALOG *d, int c)
{
  if (msg == MSG_DRAW)
    {
      scare_mouse ();
      draw_sprite (screen, gui_get_screen (), 0, 0);
      unscare_mouse ();
      show_mouse (screen);
    }

    
  return D_O_K;
}

//----------------------------------------------------------------------------
// Gibt die Zugliste aus
//----------------------------------------------------------------------------
int
d_output_turn_list_proc (int msg, DIALOG *d, int c)
{
  /*** Variablen ***/
  metasquares_gfx *obj = (metasquares_gfx *) d->dp; // Objekt, für dass ge-
                                                  // zeichnet wird
  unsigned int output_for_player = obj->start_player; // Von welchem Spieler der
                                                 // aktuelle Zug gemacht wurde
  unsigned int first_plr_not_strtplr = 0; // erster Spieler nicht Startspieler
                                          // -> auf d->h setzen
  pos_t pos; // Position, auf die ein Spieler gesetzt hat.
  char backparsed_position[2]; // Position als Buchstabe + Zahl
  char buffer[32]; // Ausgabebuffer
  BITMAP *bmp_bg; // Zeilenhinterlegung
  bool shaded = false; // Schattierte Zeile
  unsigned int i, j; // Zählvariable

  /*** Anweisungen ***/
  if (msg == MSG_DRAW)
    {
      bmp_bg = create_bitmap (d->w, d->h);
      rectfill (bmp_bg, 0, 0, d->w, d->h, d->bg);
      set_trans_blender (0, 0, 0, 150);
  
      textout_ex(gui_get_screen (), font, "Spieler 1 | Spieler 2", d->x, d->y, d->fg, -1);
      for (i = 0, j = 0; i < obj->plr_positions[0].size ()
                         || j < obj->plr_positions[1].size ();)
        {
          if (output_for_player == 0)
            {
              if (i == j)
                {
                  if (shaded)
                    draw_trans_sprite (gui_get_screen (), bmp_bg, d->x, d->y + i * d->h + 14);
                  shaded = !shaded;
                }
              else if (i == j - 1)
                {
                  if (shaded)
                    draw_trans_sprite (gui_get_screen (), bmp_bg, d->x, d->y + (i + 1) * d->h + 14);
                  shaded = !shaded;
                }

              pos = obj->plr_positions[output_for_player][i];
              obj->_backparse_coordinate (&pos, backparsed_position);
              sprintf(buffer, "    %c%c", backparsed_position[0],
                      backparsed_position[1]);
              textout_ex(gui_get_screen (), font, buffer, d->x,
                         d->y + i * d->h + 15 + first_plr_not_strtplr, d->fg, -1);
              output_for_player = 1;
              i++;
            }
          else
            {
              if (i == 0 && j == 0)
                {
                  draw_trans_sprite (gui_get_screen (), bmp_bg, d->x, d->y + 14);
                  first_plr_not_strtplr = d->h;
                }
              pos = obj->plr_positions[output_for_player][j];
              obj->_backparse_coordinate (&pos, backparsed_position);
              sprintf (buffer, "               %c%c", backparsed_position[0],
                       backparsed_position[1]);
              textout_ex(gui_get_screen (), font, buffer, d->x, d->y + j * d->h + 15, d->fg, -1);
              output_for_player = 0;
              j++;
            }

        }
      destroy_bitmap (bmp_bg);
    }
    
  return D_O_K;
}


//============================================================================
// Class: metasquares_txt
//============================================================================
//----------------------------------------------------------------------------
// Konstruktor
//----------------------------------------------------------------------------
metasquares_gfx::metasquares_gfx (bool alpha_blend)
{
  bmp_bgr = load_bitmap (BMP_BGR_FILENAME, NULL);
  bmp_sqr = load_bitmap (BMP_SQR_FILENAME, NULL);
  bmp_plr[0] = load_bitmap (BMP_PLR1_FILENAME, NULL);
  bmp_plr[1] = load_bitmap (BMP_PLR2_FILENAME, NULL);
  if( !bmp_bgr || !bmp_sqr || !bmp_plr[ 0 ] || !bmp_plr[ 1 ] )
    exit_error( "Graphics file could not be loaded!" );
  
  col_plr[0] = COL_PLR1;
  col_plr[1] = COL_PLR2;
  
  alpha_blending = alpha_blend;
}

//----------------------------------------------------------------------------
// Destruktor
//----------------------------------------------------------------------------
metasquares_gfx::~metasquares_gfx (void)
{
  destroy_bitmap (bmp_bgr);
  destroy_bitmap (bmp_sqr);
  destroy_bitmap (bmp_plr[0]);
  destroy_bitmap (bmp_plr[1]);
}

//----------------------------------------------------------------------------
// Geht in die Schleife für die Ausführung eines Spieles
//----------------------------------------------------------------------------
void
metasquares_gfx::start_game (unsigned int set_str_plr_to, unsigned int rec_depth)
{
  /*** Variables ***/
  int play_again; // Noch ein Spiel?
  BITMAP *buffer = create_bitmap (SCREEN_W, SCREEN_H);
  DIALOG gui[] = {
    { d_bitmap_proc, 0, 0, BGR_WIDTH, BGR_HEIGHT, 0, 0, 0, 0, 0, 0, bmp_bgr, NULL, NULL },
    { d_metasquare_proc, SQR_X_OFFSET, SQR_Y_OFFSET, SQR_SIZE, SQR_SIZE, 0, 0,
      0, 0, 0, 0, this, NULL, NULL },
    { d_scoreout_proc, SCOREOUT_X_OFFSET, SCOREOUT_Y_OFFSET, 0, 0, 0, 0, 0, 0,
      0, 0, this, NULL, NULL },
    { d_output_turn_list_proc, TL_X_OFFSET, TL_Y_OFFSET, TL_WIDTH, 10, makecol (0, 0, 0), makecol (255, 255, 255), 0, 0, 0, 0, this, NULL, NULL },
    { d_update_screen_proc, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, NULL, NULL, NULL },
    { NULL, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, NULL, NULL, NULL }
  };

  /*** Commands ***/
  gui_bg_color = COL_BGR;

  do
    {
      play_again = false;
      
      draw_sprite (screen, bmp_bgr, 0, 0);

      switch( alert( "Spielmodus waehlen:", "", "", "Mensch vs. Mensch", "Mensch vs. Computer", -1, -1 ) )
        {
          case 1:
            rec_depth = 0;
            break;
          case 2:
            if( rec_depth <= 0 )
              rec_depth = 3;
            break;
        }

      switch (alert3 ("Startspieler waehlen:", "", "", "Spieler 1", "Spieler 2", "Zufaellig", -1, -1, -1))
        {
          case 1:
            initialize (0, rec_depth);
            break;
          case 2:
            initialize (1, rec_depth);
            break;
          case 3:
            initialize (2, rec_depth);
            if (start_player == 0)
              alert ("Spieler 1 beginnt.", "", "", "OK", NULL, -1, -1);
            else
              alert ("Spieler 2 beginnt.", "", "", "OK", NULL, -1, -1);
            break;
        }
      
      if (start_player == 1 && recursion_depth > 0)
        make_ai_move (act_player);
      
      gui_set_screen (buffer);
      do_dialog (gui, -1);
      gui_set_screen (screen);

      if (is_game_end ()) // Wenn Escape gedrückt wurde, keine Msg anzeigen
        {
          if (score[0][3] > score[1][3])
            play_again = alert ("Spieler 1 gewinnt das Spiel!", "", "Noch ein Spiel?", "Ja", "Nein", -1, -1);
          else
            play_again = alert ("Spieler 2 gewinnt das Spiel!", "", "Noch ein Spiel?", "Ja", "Nein", -1, -1);
        }
    }
  while (play_again == 1);

  return;
}

#endif
#endif
