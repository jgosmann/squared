/***************************************************************************
 * Project: squared                                                        *
 * Filename: metasquares.h                                                 *
 * Begin: Fr Mar 18 13:40:15 CET 2005                                      *
 * Description: Umsetzung des Spiels MetaSquares mit einem Computergegener.*
 ***************************************************************************
 * Authors: Jan Gosmann (blubb@bbmy.ath.cx)                                *
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

#ifndef METASQUARES_H
#define METASQUARES_H 1

/***** Includes *****/
#include <cmath>
#include <ctime>
#include <cstdlib>

#include <vector>

/***** Defines *****/
#ifndef PI
 #define PI 3.141592654
#endif

/* Spielfeldgröße */
#define SQUARE_SIZE 8

/* Misc */
using namespace std;

/***** Structs + Datatypes *****/
//----------------------------------------------------------------------------
// Position auf dem Spielfeld
//----------------------------------------------------------------------------
struct pos_t { // Zur Speicherung einer Position auf dem Spielfeld
  unsigned int x, y; // x- und y-Koordinate
  pos_t (void) { };
  pos_t (pos_t *pos);
  void operator = (pos_t *source);
  bool operator == (pos_t *cmp_with);
  bool operator != (pos_t *cmp_with);
};

/*** Konstruktor ***/
pos_t::pos_t (pos_t *pos)
{
  x = pos->x;
  y = pos->y;
  return;
}

/*** Zuweisung ***/
void pos_t::operator = (pos_t *source)
{
  x = source->x;
  y = source->y;
  return;
}

/*** Vergeleich ***/
bool pos_t::operator == (pos_t *cmp_with)
{
  if (x == cmp_with->x && y == cmp_with->y)
    return true;
  else
    return false;
}

bool pos_t::operator != (pos_t *cmp_with)
{
  return !(this == cmp_with);
}

//----------------------------------------------------------------------------
// Infos zu einem (un)vollständigen Quadrat
//----------------------------------------------------------------------------
struct sqr_info_t { // Zur Speicherung von Informationen zu einem vollständigen
                    // oder unvollständigen (mit 2 Punkte) Quadrat.
  pos_t positions[4]; // Positionen der Quadratecken
  bool placed_on[4]; // Liegt auf der Position schon ein Spielstein?
  unsigned int score; // Punkte, die das Quadrat bringt
  unsigned int status; // Wie vollständig ist das Quadrat?
  unsigned int player; // Wessen Quadrat?
  sqr_info_t (void) { };
  sqr_info_t (sqr_info_t *source);
  void operator = (sqr_info_t *source);
  bool operator == (sqr_info_t *cmp_with);
  bool operator != (sqr_info_t *cmp_with);
};

/*** Konstruktor ***/
sqr_info_t::sqr_info_t (sqr_info_t *source)
{
  unsigned int i;
  for (i = 0; i < 4; i++)
    {
      positions[i] = source->positions[i];
      placed_on[i] = source->placed_on[i];
    }
  score = source->score;
  status = source->status;
  player = source->player;
  return;
}

/*** Zuweisung ***/
void sqr_info_t::operator = (sqr_info_t *source)
{
  unsigned int i;
  for (i = 0; i < 4; i++)
    {
      positions[i] = source->positions[i];
      placed_on[i] = source->placed_on[i];
    }
  score = source->score;
  status = source->status;
  player = source->player;
}

/*** Vergleich ***/
bool sqr_info_t::operator == (sqr_info_t *cmp_with)
{
  /*** Variables ***/
  unsigned int i, j;
  bool found;
  
  /*** Commands ***/
  if (score != cmp_with->score)
    return false;
  for (i = 0; i < 4; i++)
    {
      found = false;
      for (j = 0; j < 4; j++)
        {
          if (positions[i].x == cmp_with->positions[j].x
              && positions[i].y == cmp_with->positions[j].y)
            found = true;
        }
      if (!found)
        return false;
    }
  return true;
}

bool sqr_info_t::operator != (sqr_info_t *cmp_with)
{
  return !(this == cmp_with);
}

/***** Classes *****/
class metasquares {
  public:
    /*** Variables ***/
    unsigned int square[SQUARE_SIZE][SQUARE_SIZE]; // Spielfeld
    unsigned int start_player; // Startspieler
    unsigned int act_player; // Aktueller Spieler
    unsigned int recursion_depth; // Rekursionstiefe
    
    vector <pos_t> plr_positions[2]; // Positionen, auf die gesetzt wurde
    vector <sqr_info_t> plr_squares[2]; // Infos zu den (möglichen) Quadraten
    
    int score[2][4]; // Punkte der Spieler, die auf Quadrate mit
                     // einer bestimmten Vollständigkeit fallen
    
    /*** Functions ***/
    void initialize (unsigned int set_str_plr_to, unsigned int rec_depth);
    bool make_move (pos_t *pos);
    int make_ai_move (unsigned int mv_for_player, unsigned int act_recdepth = 0,
                      int reval_above = 0, bool set_above = false);
    void make_ai_move_old (unsigned int mv_for_player);
    bool is_game_end (void);
    
  protected:
    /*** Functions ***/
    void _get_pos_sqrs (pos_t *pos, unsigned int player, vector <sqr_info_t> *save_to);
    void _del_pos_sqrs (pos_t *pos, vector <sqr_info_t> *del_from);
    void _get_pos_sqr_score (pos_t *pos, int save_to[2][4]);
    bool _search_pos (pos_t *pos, vector <pos_t> *search_in);
    unsigned int _search_sqr (sqr_info_t *sqr, vector <sqr_info_t> *search_in);
    double _intersection_angle (pos_t *posA1, pos_t *posA2, pos_t *posB1, pos_t *posB2);
    inline void _add_score (int add[2][4], unsigned int player);
    template <class type> type _round (type n);
    bool _parse_coordinate (char in[2], pos_t *out);
    bool _backparse_coordinate (pos_t *in, char out[2]);
};

/***** Functions *****/
//============================================================================
// Class: metasquares
//============================================================================
//----------------------------------------------------------------------------
// Initialisierung
// Eingabe: set_str_plr_to = 0 oder 1 für entsprechenden Startspieler oder > 1
//                           für zufälligen Startspieler
//               rec_depth = Rekursionstiefe der KI, 0 für menschlichen
//                           Spieler
//----------------------------------------------------------------------------
void
metasquares::initialize (unsigned int set_str_plr_to, unsigned int rec_depth)
{
  /*** Variables ***/
  unsigned int i, j; // Zählvariablen

  /*** Commands ***/
  srand ((unsigned int) time (NULL));

  /* square initialisieren */
  for (i = 0; i < SQUARE_SIZE; i++)
  {
    for (j = 0; j < SQUARE_SIZE; j++)
      square[i][j] = 2;
  }

  /* score initialisieren */
  for (i = 0; i < 2; i++)
  {
    for (j = 0; j < 4; j++)
      score[i][j] = 0;
  }

  /* Aktuellen Spieler setzen */
  if (set_str_plr_to > 1)
    act_player = rand () % 2;
  else
    act_player = set_str_plr_to;
  start_player = act_player;

  /* Rekursionstiefe setzen */
  recursion_depth = rec_depth;
  
  /* plr_positions, plr_squares */
  plr_positions[0].clear ();
  plr_positions[1].clear ();
  
  plr_squares[0].clear ();
  plr_squares[1].clear ();

  return;
}

//----------------------------------------------------------------------------
// Setzt einen Spielstein in das in position übergeben Feld
// Rückgabe: true, bei Erfolg, false, wenn Feld schon belegt oder ungültig
//----------------------------------------------------------------------------
bool
metasquares::make_move (pos_t *pos)
{
  if (pos->x > 8 || pos->y > 8)
    return false;

  if (square[pos->x][pos->y] != 2)
    return false;

  square[pos->x][pos->y] = act_player;

  _get_pos_sqrs (pos, act_player, &plr_squares[act_player]);
  plr_positions[act_player].push_back (new pos_t (pos));

  act_player = (act_player == 0) ? 1 : 0;

  _del_pos_sqrs (pos, &plr_squares[act_player]);

  return true;
}

//----------------------------------------------------------------------------
// Lasse die KI setzen
// Eingabe:  mv_for_player = Der Spieler für den gesetzt werden soll
//            act_recdepth = Aktuelle Rekursionstiefe
//            retval_above = Vorläufiger Rückgabewert eine
//                           Rekursionsstufe höher
//               set_above = Wurde eine Rekursionstiefe höher schon act_retval
//                           gesetzt?
// Rückgabe: Bewertung des Astes des Spielbaumes
//----------------------------------------------------------------------------
int
metasquares::make_ai_move (unsigned int mv_for_player, unsigned int act_recdepth,
                           int retval_above, bool set_above)
{
  /*** Variables ***/
  int score_dif[2][4]; // Punktedifferenz
  pos_t best_move; // Wo soll hingesetzt werden?
  pos_t act_move; // Aktuelle Position, die geprüft wird
  bool set = false; // Wurde act_retval schon verändert?
  int act_retval = 0; // Provisorischer Rückgabewert
  int retval_bottom; // Rückgabewert der untergeordneten Funktion
  int maxmin = (mv_for_player == 0) ? 1 : -1; // Maximierender oder
                                              // minimierender Spieler?
  unsigned int i, j; // Zählvariablen

  /*** Commands ***/
  for (i = 0; i < SQUARE_SIZE; i++)
    {
      for (j = 0; j < SQUARE_SIZE; j++)
        {
      	  if (square[i][j] == 2)
	          {
              /* "Setzen" */
	            act_move.x = i;
	            act_move.y = j;
              square[act_move.x][act_move.y] = mv_for_player;
              _get_pos_sqr_score (&act_move, score_dif);
              _add_score (score_dif, mv_for_player);
              plr_positions[mv_for_player].push_back (new pos_t (act_move));

              /* Prüfen */
	            if (act_recdepth < recursion_depth - 1)
	              retval_bottom = make_ai_move ((mv_for_player == 0) ? 1 : 0, act_recdepth + 1,
		                                          act_retval, set);
	            else
	              {
                  retval_bottom = (int) (score[0][1] + pow ((double) score[0][2], 2)
		                                    + pow ((double) score[0][3], 3) - score[1][1]
				                                - pow ((double) score[1][2], 2)
				                                - pow ((double) score[1][3], 3));
	                if (score[0][3] > 150 && score[0][3] > score[1][3] * 1.1)
                    retval_bottom += (int) pow (6888.0, 4);
                  if (score[1][3] > 150 && score[1][3] > score[0][3] * 1.1)
                    retval_bottom -= (int) pow (6888.0, 4);
	              }

	            if (retval_bottom * maxmin > act_retval * maxmin || !set)
	              {
                  set = true;
	                best_move = act_move;
	                act_retval = retval_bottom;
		              if (act_retval * maxmin >= retval_above * maxmin && set_above)
		                {
                      square[act_move.x][act_move.y] = 2;
                      plr_positions[mv_for_player].erase
		                      (plr_positions[mv_for_player].end ());
                      _add_score (score_dif, (mv_for_player == 0) ? 1 : 0);
                      return act_retval;
		                }
	              }

	            /* Zug rückängig machen */
	            square[act_move.x][act_move.y] = 2;
              plr_positions[mv_for_player].erase (plr_positions[mv_for_player].end ());
              _add_score (score_dif, (mv_for_player == 0) ? 1 : 0);
	          }
	      }
    }

    if (act_recdepth == 0)
      {
        if (!set || act_retval == 0)
          {
            do
              {
                best_move.x = rand () % 8;
                best_move.y = rand () % 8;
              }
            while (!make_move (&best_move));
          }
        else
          if(!make_move (&best_move))
            {
              cerr << "KI hat einen nicht möglichen Zug versucht:" << endl;
              cerr << best_move.x << " " << best_move.y << endl;
              cerr << "Position belegt mit: " <<square[best_move.x][best_move.y]
                   << endl;
            }
      }

  return act_retval;
}

//----------------------------------------------------------------------------
// Lasse die KI setzen (alte KI-Version)
// Eingabe:  mv_for_player = Der Spieler für den gesetzt werden soll
//----------------------------------------------------------------------------
void
metasquares::make_ai_move_old (unsigned int mv_for_player)
{
  /*** Variables ***/
  pos_t best_move; // Bester move
  unsigned int best_score = 0; // Punkte für best_move
  pos_t act_move; // Auswertung für den aktuell geprüften move
  unsigned int act_score; // Punkte für aktuell geprüften move
  unsigned int win_state = 0; // 1 = Gegner gewinnt durch den move, 2 = KI gewinnt
  unsigned int act_win_state; // Wie win_state für aktuellen geprüften move
  bool set = false; // Wurde bereits gesetzt?
  unsigned int i, j, k, l, m, n; // Zählvariablen
  
  /*** Commands ***/
  /* Quadrat in x Zügen zu vervollständigen? */
  for (i = 0; i < 2; i++)
    {
      for (j = 0; j < plr_squares[i].size (); j++)
        {
          for (k = 0; k < 4; k++)
            {
              if (plr_squares[i][j].placed_on[k] == true)
                continue;
                
              act_win_state = 0;
              act_score = 0;
              act_score = (unsigned int) pow ((double) plr_squares[i][j].score,
                                              (double) plr_squares[i][j].status);
              act_move.x = plr_squares[i][j].positions[k].x;
              act_move.y = plr_squares[i][j].positions[k].y;
              
              if (plr_squares[i][j].status == 3
                  && score[i][4] + plr_squares[i][j].score >= 150
                  && score[i][4] + plr_squares[i][j].score
                      >= score[(i == 0) ? 1 : 0][4] * 1.1)
                {
                  if (i != mv_for_player)
                    act_win_state = 1;
                  else
                    act_win_state = 2;
                }
              
              for (l = 0; l < 2; l++)
                {
                  for (m = j + 1; m < plr_squares[l].size (); m++)
                    {
                      for (n = 0; n < 4; n++)
                        {
                          if (plr_squares[l][m].placed_on[n] == true)
                            continue;

                          if (act_move.x == plr_squares[l][m].positions[n].x
                              && act_move.y == plr_squares[l][m].positions[n].y)
                            act_score = (unsigned int) pow ((double) plr_squares[l][m].score,
                                                            (double) plr_squares[l][m].status);
                         
                          if (plr_squares[l][m].status == 3
                              && score[l][4] + plr_squares[l][m].score >= 150
                              && score[l][4] + plr_squares[l][m].score
                                  >= score[(l == 0) ? 1 : 0][4] * 1.1)
                            {
                              act_win_state++;
                              if (l != mv_for_player)
                                act_win_state = 1;
                              else
                                act_win_state = 2;
                            }
                        }
                    }
                }
                
              if (act_win_state > win_state || act_score > best_score)
                {
                  best_move = act_move;
                  best_score = act_score;
                  win_state = act_win_state;
                  set = true;
                }
            }
        }
    }
    
  /* Setzen */
  if (set)
    make_move (&best_move);
  else
    {
      make_ai_move (mv_for_player);
//      do
//        {
//          best_move.x = rand () % SQUARE_SIZE;
//          best_move.y = rand () % SQUARE_SIZE;
//        }
//      while (!make_move (&best_move));
    }

  return;
}

//----------------------------------------------------------------------------
// Prüft, ob das Spiel zu Ende ist.
// Eingabe: sqrs = Zeiger auf den Vektorarray mit den Quadraten
//----------------------------------------------------------------------------
bool
metasquares::is_game_end (void)
{
  if (score[0][3] >= 150)
    {
      if (score[0][3] - (0.1 * score[1][3]) > score[1][3])
        return true;
    }
  else if (score[1][3] >= 150)
    {
      if (score[1][3] - (0.1 * score[0][3]) > score[0][3])
        return true;
    }

  return false;
}

//----------------------------------------------------------------------------
// Berechnet sämtliche Quadrate, die mit dem in pos übergebenen Positiom und
//   einem der anderen eigenen Spielsteine möglich sind (oder bereits
//   existieren) für den Spieler player.
// Eingabe:     pos = siehe Beschreibung
//           player = siehe Beschreibung
//          save_to = Zeiger auf einen Vektor <sqr_info_t> zum Speichern der
//                    Quadrate
//----------------------------------------------------------------------------
void
metasquares::_get_pos_sqrs (pos_t *pos, unsigned int player, vector <sqr_info_t> *save_to)
{
  /*** Variables ***/
  sqr_info_t sqr; // Ein Quadrat
  sqr_info_t sqr_init; // Initialwerte von sqr
  int x_dif, y_dif; // x- u. y-Differenzen zwischen pos[0] u. pos[1]
  double x_dif_dig, y_dif_dig; // x- u. y-Differenzen zweier Punkte der
                               // gleichen Quadratseite, wenn pos[0] u. pos[1]
                               // die Diagonale sind
  double side_length; // Quadratsseitenlänge
  double angle; // Winkel, um wieviel das Quadrat zur Senkrechten ge-
                      // kippt ist.
  unsigned int i; // Zählvariable
  int j; // "Zählvariable"

  /*** Commands ***/
  /* sqr_init initialisieren */
  sqr_init.positions[0].x = pos->x;
  sqr_init.positions[0].y = pos->y;
  sqr_init.placed_on[0] = true;
  sqr_init.placed_on[1] = true;
  sqr_init.placed_on[2] = false;
  sqr_init.placed_on[3] = false;
  sqr_init.player = player;
  sqr_init.status = 2;

  for (i = 0; i < plr_positions[player].size (); i++)
    {
      /* sqr_init initialisieren */
      sqr_init.positions[1].x = plr_positions[player][i].x;
      sqr_init.positions[1].y = plr_positions[player][i].y;

      /* Weitere Variablen initialisieren */
      x_dif = sqr_init.positions[0].x - sqr_init.positions[1].x;
      y_dif = sqr_init.positions[0].y - sqr_init.positions[1].y;

      /* Möglichkeiten berechnen, wenn pos1 und pos2 eine Seitenkante wären */
      for (j = -1; j <= 1; j += 2)
        {
          sqr = sqr_init;

          sqr.positions[2].x = sqr.positions[0].x + (j * y_dif);
          sqr.positions[2].y = sqr.positions[0].y - (j * x_dif);

          sqr.positions[3].x = sqr.positions[1].x + (j * y_dif);
          sqr.positions[3].y = sqr.positions[1].y - (j * x_dif);

          sqr.score = (unsigned int) pow ((double) abs (x_dif) + abs (y_dif) + 1, 2);

          if (sqr.positions[2].x < SQUARE_SIZE && sqr.positions[3].x < SQUARE_SIZE
              && sqr.positions[2].y < SQUARE_SIZE && sqr.positions[3].y < SQUARE_SIZE)
            {
              if (square[sqr.positions[2].x][sqr.positions[2].y] == player)
                {
                  sqr.placed_on[2] = true;
                  sqr.status++;
                }
              else if (square[sqr.positions[2].x][sqr.positions[2].y] != 2)
                sqr.status -= 2;

              if (square[sqr.positions[3].x][sqr.positions[3].y] == player)
                {
                  sqr.placed_on[3] = true;
                  sqr.status++;
                }
              else if (square[sqr.positions[3].x][sqr.positions[3].y] != 2)
                sqr.status -= 2;
              
              if (sqr.status >= 2 && sqr.status < 5)
                {
                  if (_search_sqr (&sqr, save_to) < save_to->size ())
                    {
                      if ((*save_to)[_search_sqr(&sqr, save_to)] != &sqr
                          && sqr.status != (*save_to)[_search_sqr(&sqr, save_to)].status)
                        {
                          score[player][sqr.status - 1] += sqr.score;
                          score[player][sqr.status - 2] -= sqr.score;
                        }
                      (*save_to)[_search_sqr(&sqr, save_to)] = sqr;
                    }
                  else
                    {
                      save_to->push_back (sqr_info_t ());
                      (*save_to)[save_to->size () - 1] = sqr;
                      score[player][sqr.status - 1] += sqr.score;
                    }
                }
            }
        }

      /* Möglichkeit berechnen, wenn pos1 u. pos2 eine Diagonale wären */
      sqr = sqr_init;

      side_length = sqrt ((pow ((double) x_dif, 2) + pow ((double) y_dif, 2)) / 2);
      angle = (PI / 2) - asin (side_length / sqrt (pow ((double) x_dif, 2)
                                                   + pow ((double) y_dif, 2)))
                       - atan (abs ((double) y_dif / x_dif));

      x_dif_dig = _round (side_length * sin (angle));
      y_dif_dig = _round (side_length * cos (angle));
      
      sqr.positions[2].x = (unsigned int) (sqr.positions[0].x + x_dif_dig);
      sqr.positions[2].y = (unsigned int) (sqr.positions[0].y + y_dif_dig);

      sqr.positions[3].x = (unsigned int) (sqr.positions[1].x - x_dif_dig);
      sqr.positions[3].y = (unsigned int) (sqr.positions[1].y - y_dif_dig);

      sqr.score = (unsigned int) pow (abs (x_dif_dig) + abs (y_dif_dig) + 1, 2);

      if (_round (_intersection_angle (&sqr.positions[0], &sqr.positions[2],
                                       &sqr.positions[0], &sqr.positions[3]))
          == 90)
        {
          if (sqr.positions[2].x < SQUARE_SIZE && sqr.positions[3].x < SQUARE_SIZE
              && sqr.positions[2].y < SQUARE_SIZE && sqr.positions[3].y < SQUARE_SIZE)
            {
              if (square[sqr.positions[2].x][sqr.positions[2].y] == player)
                {
                  sqr.placed_on[2] = true;
                  sqr.status++;
                }
              else if (square[sqr.positions[2].x][sqr.positions[2].y] != 2)
                sqr.status -= 2;

              if (square[sqr.positions[3].x][sqr.positions[3].y] == player)
                {
                  sqr.placed_on[3] = true;
                  sqr.status++;
                }
              else if (square[sqr.positions[3].x][sqr.positions[3].y] != 2)
                sqr.status -= 2;

              if (sqr.status >= 2 && sqr.status < 5)
                {
                  if (_search_sqr (&sqr, save_to) < save_to->size ())
                    {
                      if ((*save_to)[_search_sqr(&sqr, save_to)] != &sqr
                          && sqr.status != (*save_to)[_search_sqr(&sqr, save_to)].status)
                        {
                          score[player][sqr.status - 1] += sqr.score;
                          score[player][sqr.status - 2] -= sqr.score;
                        }
                      (*save_to)[_search_sqr(&sqr, save_to)] = sqr;
                    }
                  else
                    {
                      save_to->push_back (sqr_info_t ());
                      (*save_to)[save_to->size () - 1] = sqr;
                      score[player][sqr.status - 1] += sqr.score;
                    }
                }
            }
        }

    }

  return;
}

//----------------------------------------------------------------------------
// Löscht alle Quadrate mit der Position pos aus del_from
//----------------------------------------------------------------------------
void
metasquares::_del_pos_sqrs (pos_t *pos, vector <sqr_info_t> *del_from)
{
  /*** Variables ***/
  vector <sqr_info_t>::iterator iter;


  
  /*** Commands ***/
  for (iter = del_from->begin (); iter != del_from->end (); iter++)
    {
      if (*pos == &(iter->positions[0])
          || *pos == &(iter->positions[1])
          || *pos == &(iter->positions[2])
          || *pos == &(iter->positions[3]))
        {
          score[iter->player][iter->status - 1] -= iter->score;
          iter = del_from->erase (iter);
          iter--;
        }
    }

  return;
}

//----------------------------------------------------------------------------
// Berechnet Punktedifferenzen, die für die Spieler beim Setzen auf pos auf-
//   treten.
// Eingabe:     pos = siehe Beschreibung
//          save_to = Zeiger auf einen Array [2][4] zum Speichern der Punkte-
//                    differenzen
//----------------------------------------------------------------------------
void
metasquares::_get_pos_sqr_score (pos_t *pos, int save_to[2][4])
{
  /*** Variables ***/
  sqr_info_t sqr; // Ein Quadrat
  sqr_info_t sqr_init; // Initialwerte von sqr
  int x_dif, y_dif; // x- u. y-Differenzen zwischen pos[0] u. pos[1]
  double x_dif_dig, y_dif_dig; // x- u. y-Differenzen zweier Punkte der
                               // gleichen Quadratseite, wenn pos[0] u. pos[1]
                               // die Diagonale sind
  double side_length; // Quadratsseitenlänge
  double angle; // Winkel, um wieviel das Quadrat zur Senkrechten ge-
                // kippt ist.
  unsigned int i, player; // Zählvariable
  int j; // "Zählvariable"

  /*** Commands ***/
  /* sqr_init initialisieren */
  sqr_init.positions[0].x = pos->x;
  sqr_init.positions[0].y = pos->y;
  sqr_init.placed_on[0] = true;
  sqr_init.placed_on[1] = true;
  sqr_init.placed_on[2] = false;
  sqr_init.placed_on[3] = false;
  
  /* save_to initialisieren */
  for (player = 0; player < 2; player++)
    {
      for (i = 0; i < 4; i++)
        save_to[player][i] = 0;
    }

  for (player = 0; player < 2; player++)
    {
      for (i = 0; i < plr_positions[player].size (); i++)
        {
          /* sqr_init initialisieren */
          sqr_init.positions[1].x = plr_positions[player][i].x;
          sqr_init.positions[1].y = plr_positions[player][i].y;

          /* Weitere Variablen initialisieren */
          x_dif = sqr_init.positions[0].x - sqr_init.positions[1].x;
          y_dif = sqr_init.positions[0].y - sqr_init.positions[1].y;

          /* Möglichkeiten berechnen, wenn pos1 und pos2 eine Seitenkante wären */
          for (j = -1; j <= 1; j += 2)
            {
              sqr = sqr_init;

              sqr.positions[2].x = sqr.positions[0].x + (j * y_dif);
              sqr.positions[2].y = sqr.positions[0].y - (j * x_dif);

              sqr.positions[3].x = sqr.positions[1].x + (j * y_dif);
              sqr.positions[3].y = sqr.positions[1].y - (j * x_dif);

              sqr.score = (unsigned int) pow ((double) abs (x_dif) + abs (y_dif) + 1, 2);
              sqr.status = 2;
              sqr.player = sqr.player;

              if (sqr.positions[2].x < SQUARE_SIZE && sqr.positions[3].x < SQUARE_SIZE
                  && sqr.positions[2].y < SQUARE_SIZE && sqr.positions[3].y < SQUARE_SIZE)
                {
                  if (square[sqr.positions[2].x][sqr.positions[2].y] == player)
                    {
                      sqr.placed_on[2] = true;
                      sqr.status++;
                    }
                  else if (square[sqr.positions[2].x][sqr.positions[2].y] != 2)
                    sqr.status -= 2;

                  if (square[sqr.positions[3].x][sqr.positions[3].y] == player)
                    {
                      sqr.placed_on[3] = true;
                      sqr.status++;
                    }
                  else if (square[sqr.positions[3].x][sqr.positions[3].y] != 2)
                    sqr.status -= 2;

                  if (sqr.status >= 2 && sqr.status < 5)
                    {
                      save_to[player][sqr.status - 1] += sqr.score;
                      save_to[player][sqr.status - 2] -= sqr.score;
                    }
                }
            }

          /* Möglichkeit berechnen, wenn pos1 u. pos2 eine Diagonale wären */
          sqr = sqr_init;

          side_length = sqrt ((pow ((double) x_dif, 2) + pow ((double) y_dif, 2)) / 2);
          angle = (PI / 2) - asin (side_length / sqrt (pow ((double) x_dif, 2)
                                                   + pow ((double) y_dif, 2)))
                           - atan (abs ((double) y_dif / x_dif));

          x_dif_dig = _round (side_length * sin (angle));
          y_dif_dig = _round (side_length * cos (angle));

          sqr.positions[2].x = (unsigned int) (sqr.positions[0].x + x_dif_dig);
          sqr.positions[2].y = (unsigned int) (sqr.positions[0].y + y_dif_dig);

          sqr.positions[3].x = (unsigned int) (sqr.positions[1].x - x_dif_dig);
          sqr.positions[3].y = (unsigned int) (sqr.positions[1].y - y_dif_dig);

          sqr.score = (unsigned int) pow (abs (x_dif_dig) + abs (y_dif_dig) + 1, 2);
          sqr.status = 2;
          sqr.player = player;

          if (_round (_intersection_angle (&sqr.positions[0], &sqr.positions[2],
                                           &sqr.positions[0], &sqr.positions[3]))
              == 90)
            {
              if (sqr.positions[2].x < SQUARE_SIZE && sqr.positions[3].x < SQUARE_SIZE
                  && sqr.positions[2].y < SQUARE_SIZE && sqr.positions[3].y < SQUARE_SIZE)
                {
                  if (square[sqr.positions[2].x][sqr.positions[2].y] == player)
                    {
                      sqr.placed_on[2] = true;
                      sqr.status++;
                    }
                  else if (square[sqr.positions[2].x][sqr.positions[2].y] != 2)
                    sqr.status -= 2;

                  if (square[sqr.positions[3].x][sqr.positions[3].y] == player)
                    {
                      sqr.placed_on[3] = true;
                      sqr.status++;
                    }
                  else if (square[sqr.positions[3].x][sqr.positions[3].y] != 2)
                    sqr.status -= 2;

                  if (sqr.status >= 2 && sqr.status < 5)
                    {
                      save_to[player][sqr.status - 1] += sqr.score;
                      save_to[player][sqr.status - 2] -= sqr.score;
                    }
                }
            }

        }
    }

  return;
}

//----------------------------------------------------------------------------
// Überprüft, ob pos in seach_in vorhanden ist
//----------------------------------------------------------------------------
bool
metasquares::_search_pos (pos_t *pos, vector <pos_t> *search_in)
{
  /*** Variables ***/
  unsigned int i;
  
  /*** Commands ***/
  for (i = 0; i < search_in->size (); i++)
    {
      if (*pos == &(*search_in)[i])
        return true;
    }
  return false;
}

//----------------------------------------------------------------------------
// Durchsucht sämtliche Quadrate in search_in nach dem Quadrat sqr. Sollte
//   dies nicht gefunden werden, so wird search_in->size () zurückgegeben.
//----------------------------------------------------------------------------
unsigned int
metasquares::_search_sqr (sqr_info_t *sqr, vector <sqr_info_t> *search_in)
{
  /*** Variables ***/
  unsigned int i; // Zählvariable
  
  /*** Commands ***/
  for (i = 0; i < search_in->size (); i++)
    {
      if (*sqr == &(*search_in)[i])
        return i;
    }

  return search_in->size ();
}

//----------------------------------------------------------------------------
// Berechnet den Schnittwinkel zwischen zwei Vektoren (in Grad) und gibt
//   diesen zurück.
// Eingabe: posA1 u. posA2 = erster Vektor
//          posB1 u. posB2 = zweiter Vektor
//----------------------------------------------------------------------------
double
metasquares::_intersection_angle (pos_t *posA1, pos_t *posA2, pos_t *posB1, pos_t *posB2)
{
  /*** Variables ***/
  int vectorA[2], vectorB[2]; // Die beiden Vektoren

  /*** Commands ***/
  vectorA[0] = posA1->x - posA2->x;
  vectorA[1] = posA1->y - posA2->y;

  vectorB[0] = posB1->x - posB2->x;
  vectorB[1] = posB1->y - posB2->y;

  return (180 / PI)
         * acos ( (vectorA[0] * vectorB[0] + vectorA[1] * vectorB[1])
                 / (sqrt (pow ((double) vectorA[0], 2)
                          + pow ((double) vectorA[1], 2))
                    * sqrt (pow ((double) vectorB[0], 2)
                            + pow ((double) vectorB[1], 2))) );
}

//----------------------------------------------------------------------------
// Addiert die Punkte in add für player auf und subtrahiert sie für den
//   anderen Spieler
//----------------------------------------------------------------------------
inline void
metasquares::_add_score (int add[2][4], unsigned int player)
{
  /*** Variables ***/
  unsigned int i; // Zählvariable
  const unsigned int other_plr = (player == 0) ? 1 : 0;

  /*** Commands ***/
  for (i = 0; i < 4; i++)
    {
      score[player][i] += add[player][i];
      score[other_plr][i] -= add[other_plr][i];
    }
}

//----------------------------------------------------------------------------
// Rundet n.
//----------------------------------------------------------------------------
template <class type> type
metasquares::_round (type n)
{
  /*** Variablen ***/
  type ipart, fpart; // Ganzzahliger Teil u. Bruchteil

  /*** Anweisungen ***/
  fpart = modf (n, &ipart);

  if (n < 0)
    {
      if (fpart < 0.5)
        return ipart - 1;
      else
        return ipart;
    }
  else
    {
      if (fpart < 0.5)
        return ipart;
      else
        return ipart + 1;
    }
  // not reached
}

//----------------------------------------------------------------------------
// Konvertiert eine Positionsangabe bestehend aus Buchstaben und Zahl in eine
//   Koordinate aus zwei Zahlen.
// Eingabe:  in = Positionsangabe bestehend aus Buchstabe und Zahl
//          out = Variable für die Rückgabe
// Rückgabe: Bei Erfolg true, sonst false
//----------------------------------------------------------------------------
bool
metasquares::_parse_coordinate (char in[2], pos_t *out)
{
  switch (in[0])
    {
      case 'A': case 'a': out->x = 0; break;
      case 'B': case 'b': out->x = 1; break;
      case 'C': case 'c': out->x = 2; break;
      case 'D': case 'd': out->x = 3; break;
      case 'E': case 'e': out->x = 4; break;
      case 'F': case 'f': out->x = 5; break;
      case 'G': case 'g': out->x = 6; break;
      case 'H': case 'h': out->x = 7; break;
      default:
        return false;
        break;
    }

  switch (in[1])
    {
      case '1': out->y = 7; break;
      case '2': out->y = 6; break;
      case '3': out->y = 5; break;
      case '4': out->y = 4; break;
      case '5': out->y = 3; break;
      case '6': out->y = 2; break;
      case '7': out->y = 1; break;
      case '8': out->y = 0; break;
      default:
        return false;
        break;
    }

  return true;
}

//----------------------------------------------------------------------------
// Konvertiert eine Positionsangabe bestehend aus zwei Zahlen in eine
//   Koordinate aus einem Buchstaben und einer Zahl.
// Eingabe:  in = Positionsangabe bestehend aus zwei Zahlen
//          out = Variable für die Rückgabe
// Rückgabe: Bei Erfolg true, sonst false
//----------------------------------------------------------------------------
bool
metasquares::_backparse_coordinate (pos_t *in, char out[2])
{
  switch (in->x)
    {
      case 0: out[0] = 'a'; break;
      case 1: out[0] = 'b'; break;
      case 2: out[0] = 'c'; break;
      case 3: out[0] = 'd'; break;
      case 4: out[0] = 'e'; break;
      case 5: out[0] = 'f'; break;
      case 6: out[0] = 'g'; break;
      case 7: out[0] = 'h'; break;
      default:
        return false;
        break;
    }

  switch (in->y)
    {
      case 0: out[1] = '8'; break;
      case 1: out[1] = '7'; break;
      case 2: out[1] = '6'; break;
      case 3: out[1] = '5'; break;
      case 4: out[1] = '4'; break;
      case 5: out[1] = '3'; break;
      case 6: out[1] = '2'; break;
      case 7: out[1] = '1'; break;
      default:
        return false;
        break;
    }

  return true;
}

#endif
