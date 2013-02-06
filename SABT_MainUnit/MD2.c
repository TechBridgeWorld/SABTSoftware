#include "Globals.h"
#include "Modes.h"

int Current_State;
char Last_Cell;
char MD2_Last_Dot;
//Current_State - Defines the status of the mode
/* Vales and meanings
0 - Just started (in the begining, play the Welcome message)
1 - Waiting for user input
2 - Last user input processed

*/

void PlayRequestedCell(void)
{
  switch(Last_Cell)
  {
    case 'a':
      RequestToPlayMP3file("MD2_a.MP3");
      break;
    case 'b':
      RequestToPlayMP3file("MD2_b.MP3");
      break;
    case 'c':
      RequestToPlayMP3file("MD2_c.MP3");
      break;
    case 'd':
      RequestToPlayMP3file("MD2_d.MP3");
      break;
    case 'e':
      RequestToPlayMP3file("MD2_e.MP3");
      break;
    case 'f':
      RequestToPlayMP3file("MD2_f.MP3");
      break;
    case 'g':
      RequestToPlayMP3file("MD2_g.MP3");
      break;
    case 'h':
      RequestToPlayMP3file("MD2_h.MP3");
      break;
    case 'i':
      RequestToPlayMP3file("MD2_i.MP3");
      break;
    case 'j':
      RequestToPlayMP3file("MD2_j.MP3");
      break;
    case 'k':
      RequestToPlayMP3file("MD2_k.MP3");
      break;
    case 'l':
      RequestToPlayMP3file("MD2_l.MP3");
      break;
    case 'm':
      RequestToPlayMP3file("MD2_m.MP3");
      break;
    case 'n':
      RequestToPlayMP3file("MD2_n.MP3");
      break;
    case 'o':
      RequestToPlayMP3file("MD2_o.MP3");
      break;
    case 'p':
      RequestToPlayMP3file("MD2_p.MP3");
      break;
    case 'q':
      RequestToPlayMP3file("MD2_q.MP3");
      break;
    case 'r':
      RequestToPlayMP3file("MD2_r.MP3");
      break;
    case 's':
      RequestToPlayMP3file("MD2_s.MP3");
      break;
    case 't':
      RequestToPlayMP3file("MD2_t.MP3");
      break;
    case 'u':
      RequestToPlayMP3file("MD2_u.MP3");
      break;
    case 'v':
      RequestToPlayMP3file("MD2_v.MP3");
      break;
    case 'w':
      RequestToPlayMP3file("MD2_w.MP3");
      break;
    case 'x':
      RequestToPlayMP3file("MD2_x.MP3");
      break;
    case 'y':
      RequestToPlayMP3file("MD2_y.MP3");
      break;
    case 'z':
      RequestToPlayMP3file("MD2_z.MP3");
      break;
    default:
      RequestToPlayMP3file("MD2ER1.MP3");
      break;
  }
}


void MD2_PlayRequestedDot(void)
{
  switch(MD2_Last_Dot)
  {
    case '1':
      RequestToPlayMP3file("MD1_1.MP3");
      break;
    case '2':
      RequestToPlayMP3file("MD1_2.MP3");
      break;
    case '3':
      RequestToPlayMP3file("MD1_3.MP3");
      break;
    case '4':
      RequestToPlayMP3file("MD1_4.MP3");
      break;
    case '5':
      RequestToPlayMP3file("MD1_5.MP3");
      break;
    case '6':
      RequestToPlayMP3file("MD1_6.MP3");
      break;
  }
}


void MD2_Reset(void)
{
  Current_State=0;
  MD2_Last_Dot=0;
}

void MD2_Main(void)
{
  switch(Current_State)
  {
    case 0:
      RequestToPlayMP3file("MD2INT.MP3");
      Current_State=1;
      break;
    case 1:
      break;
    case 2:
      PlayRequestedCell();
      Current_State=1;
      break;
    case 3:
      MD2_PlayRequestedDot();
      Current_State=1;
      break;
  }
}

void MD2_CallModeYesAnswer(void)
{

}

void MD2_CallModeNoAnswer(void)
{

}

void MD2_InputDot(char thisDot)
{
  MD2_Last_Dot=thisDot;
  Current_State=3;
}

void MD2_InputCell(char thisCell)
{
  if(MD2_Last_Dot!=0)
  {
    Last_Cell=thisCell;
    Current_State=2;
  }
}
