/***************************************************************************
 * Project: squared                                                        *
 * Filename: metasquares_txt.h                                             *
 * Begin: Sa Mar 19 10:30:22 CET 2005                                      *
 * Description: Umsetzung des Spiels MetaSquares mit einem Computergegener.*
 ***************************************************************************
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

#ifndef METASQUARES_TXT_H
#define METASQUARES_TXT_H 1

/***** Includes *****/
#include <iostream>

#include "metasquares.h"

/***** Classes *****/
class metasquares_txt : public metasquares {
  public:
    void start_game (unsigned int set_str_plr_to = 2, unsigned int rec_depth = 0);
    void output_square (void);
    void output_turn_list (void);
};

/***** Functions *****/
//============================================================================
// Class: metasquares_txt
//============================================================================
//----------------------------------------------------------------------------
// Geht in die Schleife für die Ausführung eines Spieles
//----------------------------------------------------------------------------
void
metasquares_txt::start_game (unsigned int set_str_plr_to, unsigned int rec_depth)
{
  /*** Variablen ***/
  char input_pos[2]; // Spielereingabe
  pos_t pos; // Konvertierte Eingabe

  /*** Anweisungen ***/
  initialize (set_str_plr_to, rec_depth);
  
  cout << "Spieler " << start_player + 1 << " beginnt." << endl;
  
  while (!is_game_end ())
    {
      if (act_player == 0 || recursion_depth == 0)
        {
          output_square ();
          do
            {
              do
                {
                  cout << "Zug eingeben: ";
                  cin >> input_pos[0] >> input_pos[1];
                  if (input_pos[0] == '\\')
                    {
                      switch (input_pos[1])
                        {
                          case 'l':
                            output_turn_list ();
                            break;
                          case 's':
                            output_square ();
                            break;
                          case 'q':
                            return;
                            break;
                        }
                    }
                }
              while (!_parse_coordinate (input_pos, &pos));
            }
          while (!make_move(&pos));
        }
      else
        make_ai_move (act_player);
    }

  output_square ();
  if (score[0][3] > score[1][3])
    cout << "Spieler 1 gewinnt das Spiel!" << endl;
  else
    cout << "Spieler 2 gewinnt das Spiel!" << endl;

  return;
}

//----------------------------------------------------------------------------
// Gibt den Punktestand und das Spielfeld aus
//----------------------------------------------------------------------------
void
metasquares_txt::output_square (void)
{
  /*** Variablen ***/
  pos_t last_pos; // Position, auf die der letzte Spieler gesetzt hat.
  char backparsed_position[2]; // Position als Buchstabe + Zahl
  unsigned int other_plr = (act_player == 0) ? 1 : 0;
  unsigned int i, j; // Zählvariablen

  /*** Anweisungen ***/
  if (plr_positions[other_plr].size () > 0)
    {
      last_pos = plr_positions[other_plr][plr_positions[other_plr].size () - 1];
      _backparse_coordinate (&last_pos, backparsed_position);
    }
  else
    {
      backparsed_position[0] = ' ';
      backparsed_position[1] = ' ';
    }

  cout << "---------------------" << endl << endl;
  cout << "Punkte Spieler 1: " << score[0][3] << endl;
  cout << "Punkte Spieler 2: " << score[1][3] << endl << endl;
  cout << "Letzter Zug: " << backparsed_position[0] << backparsed_position[1]
       << endl << endl;
  cout << "    A B C D E F G H" << endl;
  cout << "  +-----------------+" << endl;

  for (i = 0; i < 8; i++)
    {
      cout << 8 - i << " |";
      for (j = 0; j < 8; j++)
        {
          if (square[j][i] < 2)
            cout << " " << square[j][i] + 1;
          else
            cout << " 0";
        }
      cout << " | " << 8 - i << endl;
    }

  cout << "  +-----------------+" << endl;
  cout << "    A B C D E F G H" << endl << endl;

  return;
}

//----------------------------------------------------------------------------
// Gibt die Zugliste aus
//----------------------------------------------------------------------------
void
metasquares_txt::output_turn_list (void)
{
  /*** Variablen ***/
  unsigned int output_for_player = start_player; // Von welchem Spieler der
                                                 // aktuelle Zug gemacht wurde
  pos_t pos; // Position, auf die ein Spieler gesetzt hat.
  char backparsed_position[2]; // Position als Buchstabe + Zahl
  unsigned int i, j; // Zählvariable

  /*** Anweisungen ***/
  cout << "Spieler 1 | Spieler 2" << endl;
  for (i = 0, j = 0; i < plr_positions[0].size ()
                     || j < plr_positions[1].size ();)
    {
      if (output_for_player == 0)
        {
          pos = plr_positions[output_for_player][i];
          _backparse_coordinate (&pos, backparsed_position);
          cout << "       " << backparsed_position[0]
                            << backparsed_position[1] << " | ";
          output_for_player = 1;
          i++;
        }
      else
        {
          pos = plr_positions[output_for_player][j];
          _backparse_coordinate (&pos, backparsed_position);
          if (i == 0 && j == 0)
            cout << "          | ";
          cout << backparsed_position[0]
               << backparsed_position[1] << endl;
          output_for_player = 0;
          j++;
        }
    }
  cout << endl;

  return;
}

#endif
