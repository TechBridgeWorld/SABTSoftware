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

void initialize_system(void);

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
  initialize_system();

  while(1)
  {
    // read in the dict file till done
    // check to see if we've received data from UI board
    // if true, process the single byte
    if(usart_keypad_data_ready)
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
      usart_keypad_receive_action();
    }

    // check to see if we've received data from a connected PC 
    // if true, process the single byte
    if(usart_pc_data_ready)
    {
      usart_pc_receive_action();
    }

    if(timer_interrupt)
    {
      timer_interrupt = false;
      //  timer_routine();
    }

    if(usart_pc_message_ready) //If a message ready from the PC, process it
    {
      pc_parse_message();
    }

    if(usart_ui_message_ready) //If a message ready from the user interface, process it
    {
      ui_parse_message(false);
    }

    if(ui_mp3_file_pending)  //If the UI handler needs to play new file, play it (the main loop won't be called while playing another file, so don't worry)
    {
      play_mp3_file(g_file_name);
    }

    ui_run_main_of_current_mode();
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
  usart_keypad_received_data = UDR1;
  usart_keypad_data_ready = true;
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
  // Temporarily disabled the PC communications since we are simulating the UI with PC
  usart_pc_received_data = UDR0;
  usart_pc_data_ready = true;
  usart_transmit_byte_to_pc(usart_pc_received_data);
};

/**
 * @brief Initialize the system and interrupts
 * @return Void
 */
void initialize_system(void)
{
  ui_mp3_file_pending = false;
  timer_interrupt = false;      // Clear the timer interrupt flag
  PORTA = 0x00;
  DDRA = 0xFF;
  PORTA = 0x00;

  // Set the data direction register values
  DDRD |= _BV(5) | _BV(6) | _BV(7);

  TCCR1A = 0x00;
  TCCR1B = 0x0D;
  OCR1A = 390;            // 1s interval
  TIMSK1 |= (1<<OCIE1A);  // Enable interrupt

  init_usart_keypad();
  init_usart_pc();
  spi_initialize();

  sei();  // sets the interrupt flag (enables interrupts)

  usart_transmit_string_to_pc_from_flash (PSTR("SABT testing..."));
  TX_NEWLINE_PC;

  init_sd_card(true);

  //say that we are not playing a sound file
  playing_sound = false;

  message_count = 0;
  valid_message = true;  

  if(!ui_check_modes())
  {
    usart_transmit_string_to_pc_from_flash (PSTR("Mode file not found"));
    TX_NEWLINE_PC; 
  }
  else
  {
    usart_transmit_string_to_pc_from_flash (PSTR("Mode file found"));
    TX_NEWLINE_PC;
  }

  ui_current_mode = number_of_modes;  //No mode selected
  ui_selected_mode = number_of_modes;

  init_read_dict((unsigned char *)"wordsEn.txt");

  while(!done_rd_dict){
    read_dict_file();
  }

  request_to_play_mp3_file("WELCOME.MP3"); 
}
