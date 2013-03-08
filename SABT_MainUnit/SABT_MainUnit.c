/**
 * @file SABT_MainUnit.c
 * @brief contains the main routine for the SABT main unit
 * @author Nick LaGrow (nlagrow), Alex Etling (petling)
 * @author Alex Etling (petling)
 * @author Kory Stiger (kstiger)
 */

#include "Globals.h"

volatile bool timer_interrupt;
volatile bool LED_STAT;

/*
volatile unsigned char iInit=0;
unsigned char option, error, data, FAT32_active;
unsigned int i;
*/

void InitializeSystem(void);


/**
 * @brief Turns off/on bits 5,6,7 in Portd depending on LED_STAT to make the 3 LEDs on the board blink?
 * This is the function called each time a timer interrupt occurs.
 * @ref AtATmega1284P(Preferred).pdf
 * @return Void
 */
void timer_routine(void)
{
  if(!LED_STAT){
    PORTD &= ~_BV(5);
    PORTD &= ~_BV(6);
    PORTD &= ~_BV(7);
    LED_STAT=true;
  }
  else
  {
    PORTD |= _BV(5);
    PORTD |= _BV(6);
    PORTD |= _BV(7);
    LED_STAT=false;
  }
}

/**
 * @brief the main routine
 * @return Void
 */
int main(void)
{
  InitializeSystem();
/*
Code to test the file write section
*/
/*
  const char* ModesFile="MODES.DAT";
  unsigned char TestFileContent[20];
  int iT=0;
  for(iT=0;iT<17;iT++)
  {
    TestFileContent[iT]=iT+65;
  }
  TestFileContent[17]='$';
  ReplaceTheContentOfThisFileWith(ModesFile,TestFileContent);
  UI_CheckModes();
*/
/*
End of test code
*/

  UI_MODE_SELECTED = 1; // @TODO remove
  UI_Current_Mode = 3;  // @TODO remove after tuesday

  //Display the files in the SD card
  //TX_NEWLINE_PC;
  //findFiles(GET_LIST,0);
  //TX_NEWLINE_PC;
  TX_NEWLINE_PC;
  //USART_transmitStringToPCFromFlash (PSTR("Press a key and see it returns."));
  TX_NEWLINE_PC;
  while(1)
  {
    // TODO remove test string
    // DPRINTF("Small waves crashing against the sand%d.", 42);

    if(timer_interrupt)
    {
      timer_interrupt=false;
    //  timer_routine();
    }

    // check to see if we've received data from UI board
    // if true, process the single byte
    if(USART_Keypad_DATA_RDY)
    {
      /* one of two types:
       * [U][I][msglen][msg_number][msg_type][payload][CRC1][CRC2]
       * [M][C][msglen][msg_number][msg_type][payload][CRC1][CRC2]
       * msg_type:
       *  A: contains braille dot at this location in the UI
       *  B: contains braille character at this location in the UI
       *  C: payload contains an error message
       *  D: payload contains a control button input from UI
       *  E: miscellaneous */
	   //USART_transmitByteToPC(USART_Keypad_Received_Data);
      USART_Keypad_ReceiveAction();
    }

    // check to see if we've received data from a connected PC 
    // if true, process the single byte
    if(USART_PC_DATA_RDY)
    {
      USART_PC_ReceiveAction();
      /*
      if(USART_PC_ReceiveAction()=='z')//This is a special case where the PC is requesting ACK for port detect
      {
        USART_transmitStringToPCFromFlash(PSTR("SABT-v2.1"));
        TX_NEWLINE_PC;
      }
      */
    }
    if(USART_PC_Message_ready) //If a message ready from the PC, process it
    {
      PC_parse_message();
    }
    if(USART_UI_Message_ready) //If a message ready from the user interface, process it
    {
      UI_parse_message(false);
    }
    if(UI_MP3_file_Pending)  //If the UI handler needs to play new file, play it (the main loop won't be called while playing another file, so don't worry)
    {
      PlayMP3file(fileName);
	  // KORY CHANGED
	  UI_MP3_file_Pending = false;
    }
    UI_RunMainOfCurrentMode();
  }
  return 1;
}


/**
 * @brief ISR is an interrupt handler that will be called when its argument
 *        corresponds to the interupt called
 * @param Interrupt vector.  Compared to interupt that is fired to see if correct
 *        function to call
 * @ref   http://www.nongnu.org/avr-libc/
 * @return  Void
 */
ISR(TIMER1_COMPA_vect)
{
  timer_interrupt = true;
}


/**
 * @brief ISR is an interrupt handler that will be called when its argument
 *        corresponds to the interupt called
 * @param Interrupt vector.  Compared to interupt that is fired to see if correct
 *        function to call
 * @ref   http://www.nongnu.org/avr-libc/
 * @return  Void
 */
ISR(USART1_RX_vect)
{
  USART_Keypad_Received_Data = UDR1;
  USART_Keypad_DATA_RDY = true;
  
  //set_last_dot(USART_Keypad_Received_Data);
  //set_last_dot2(USART_Keypad_Received_Data);
  //Set_last_dot(USART_Keypad_Received_Data); // TODO delete this handled elsewhere
};

/**
 * @brief ISR is an interrupt handler that will be called when its argument
 *        corresponds to the interupt called
 * @param Interrupt vector.  Compared to interupt that is fired to see if correct
 *        function to call
 * @ref   http://www.nongnu.org/avr-libc/
 * @return  Void
 */
ISR(USART0_RX_vect)
{
//Temporarily using the PC as the UI
//  USART_Keypad_Received_Data=UDR0;
//  USART_Keypad_DATA_RDY=true; 
//  Temporarily disabled the PC communications since we are simulating the UI with PC
  USART_PC_Received_Data=UDR0;
  USART_PC_DATA_RDY=true;
  USART_transmitByteToPC(USART_PC_Received_Data);
//
};

/**
 * @brief Initialize the system and interrupts
 * @return Void
 */
void InitializeSystem(void)
{
  UI_MP3_file_Pending = false;
  timer_interrupt = false;   // clear the timer interrupt flag
  PORTA = 0x00;
  DDRA = 0xFF;  
  PORTA = 0x00;  

  DDRD |= _BV(5)|_BV(6)|_BV(7);
  
  TCCR1A=0x00;
  TCCR1B=0x0D;
  OCR1A=390; //1s interval
  TIMSK1 |= (1<<OCIE1A); //Enable interrupt

  init_USART_Keypad();
  init_USART_PC();
  SPI_Initialize();
  
  sei();  // sets the interrupt flag (enables interrupts)

  UI_Current_Mode=0;  //No mode selected
  TX_NEWLINE_PC;
  USART_transmitStringToPCFromFlash (PSTR("SABT testing..."));
  TX_NEWLINE_PC;

  InitSDCard(true);

  message_count = 0;
  valid_message = true;  
  
  if(!UI_CheckModes())
  {
    USART_transmitStringToPCFromFlash (PSTR("Mode file not found"));
    TX_NEWLINE_PC; 
  }
  else
  {
    USART_transmitStringToPCFromFlash (PSTR("Mode file found"));
    TX_NEWLINE_PC;
  }

  RequestToPlayMP3file("INT.MP3");  // Play the welcome message
  
}
