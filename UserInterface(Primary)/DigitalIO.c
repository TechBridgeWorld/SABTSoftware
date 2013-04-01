/**
 * @file DigitalIO.c
 * @brief handle digital input/output
 * @author Nick LaGrow (nlagrow)
 * @author Alex Etling (petling)
 * @author Kory Stiger (kstiger)
 */

#include "GlobalsUI.h"

/**
 * @brief initialize the IO ports
 * @return Void
 */
void initialize_digital_io(void)
{
  enter1_state = BUTTON_OFF;
  enter2_state = BUTTON_OFF;
  mode1_state = BUTTON_OFF;
  mode2_state = BUTTON_OFF;
  vol_up_state = BUTTON_OFF;
  vol_down_state = BUTTON_OFF;
    
  // Clear the DDRC bits
  DDRC &= ~_BV(UI_BR1);  
  DDRC &= ~_BV(UI_BR2);
  DDRC &= ~_BV(UI_BR3);
  DDRC &= ~_BV(UI_BR4);
  DDRC &= ~_BV(UI_BR5);
  DDRC &= ~_BV(UI_BR6);

  // Clear the DDRB bits
  DDRB &= ~_BV(UI_ENTER1);
  DDRB &= ~_BV(UI_ENTER2);
  DDRB &= ~_BV(UI_MODE1);
  DDRB &= ~_BV(UI_MODE2);

  // Set the Data Direction register - 0 input, 1 output
  // Volume controls are input
  // LED lights 1 and 2 are output
  DDRD &= ~_BV(UI_VOLUP);
  DDRD &= ~_BV(UI_VOLDOWN);
  DDRD |= _BV(UI_STAT1)|_BV(UI_STAT2);

  // LED lights are off initially
  set_stat_led1(false);
  set_stat_led2(false);
}

/**
 * @brief determine whether command buttons have been pressed
 * @return Void
 * TODO document each step / if statement here
 */
void check_command_buttons(void)
{
  // Check to see if enter1 is pressed
  if(!(PINB & (1 << UI_ENTER1)))
  {
    if(enter1_state == BUTTON_OFF)
    {
      if(!(PINB & (1 << UI_ENTER1)))
      {
        enter1_state = BUTTON_ON;
      }
    }
  }
  else enter1_state = BUTTON_OFF;

  // Check to see if enter2 (exit) is pressed
  if(!(PINB & (1 << UI_ENTER2)))
  {
    if(enter2_state == BUTTON_OFF)
    {
      _delay_ms(100);
      if(!(PINB & (1 << UI_ENTER2)))
      {
        enter2_state = BUTTON_ON;
      }
    }
  }
  else enter2_state = BUTTON_OFF;

  // Check to see if mode1 (left mode button) is pressed
  if(!(PINB & (1 << UI_MODE1)))
  {
    if(mode1_state == BUTTON_OFF)
    {
      if(!(PINB & (1 << UI_MODE1)))
      {
        mode1_state = BUTTON_ON;
      }
    }
  }
  else mode1_state = BUTTON_OFF;

  // Check to see if mode2 (right mode button) is pressed
  if(!(PINB & (1 << UI_MODE2)))
  {
    if(mode2_state == BUTTON_OFF)
    {
      if(!(PINB & (1 << UI_MODE2)))
      {
        mode2_state = BUTTON_ON;
      }
    }
  }
  else mode2_state = BUTTON_OFF;

  // Check to see if volume up button is pressed
  if(!(PIND & (1 << UI_VOLUP)))
  {
    if(vol_up_state == BUTTON_OFF)
    {
      if(!(PIND & (1 << UI_VOLUP)))
      {
        vol_up_state = BUTTON_ON;
      }
    }
  }
  else vol_up_state = BUTTON_OFF;

  // Check to see if volume down button is pressed
  if(!(PIND & (1 << UI_VOLDOWN)))
  {
    if(vol_down_state == 0)
    {
      if(!(PIND & (1 << UI_VOLDOWN)))
      {
        vol_down_state = BUTTON_ON;
      }
    }
  }
  else vol_down_state = BUTTON_OFF;
}

/**
 * @brief Run command tasks (?)
 * @return Void
 */
void run_command_tasks(void)
{
  if(enter1_state == BUTTON_ON)
  {
    enter1_task();
    enter1_state = BUTTON_PROCESSED;
  }
  if(enter2_state == BUTTON_ON)
  {
    enter2_task();
    enter2_state = BUTTON_PROCESSED;
  }
  if(mode1_state == BUTTON_ON)
  {
    mode1_task();
    mode1_state = BUTTON_PROCESSED;
  }
  if(mode2_state == BUTTON_ON)
  {
    mode2_task();
    mode2_state = BUTTON_PROCESSED;
  }
  // Flip from reading / writing mode
  if((vol_down_state >= BUTTON_ON) && (vol_up_state >= BUTTON_ON)) // Flip the keyboard
  {
    if(interface_type == INTERFACE_READING) interface_type = INTERFACE_WRITING;
    else interface_type = INTERFACE_READING;
    return;
  }
  if(vol_up_state == BUTTON_ON)
  {
    vol_up_task();
    vol_up_state = BUTTON_PROCESSED;
  }
  if(vol_down_state == BUTTON_ON)
  {
    vol_down_task();
    vol_down_state = BUTTON_PROCESSED;
  }
}

/**
 * @brief enter1 button pressed
 * @return Void
 */
void enter1_task(void)
{
  set_stat_led2(true);
  mcu_message_payload[0] = ENTER1_PAYLOAD;
  send_packet('D', (char*)&mcu_message_payload, 1);
  
  // TODO fix this
  delay10();delay10();delay10();delay10();delay10();
  delay10();delay10();delay10();delay10();delay10();
  delay10();delay10();delay10();delay10();delay10();

  // Send the value of the currently entered cell
  capture_cell_value();
}

/**
 * @brief enter2 button pressed
 * @return Void
 */
void enter2_task(void)
{
  set_stat_led2(false);
  mcu_message_payload[0] = ENTER2_PAYLOAD;
  mcu_message_payload[1] = 'E';
  send_packet('D', (char*)&mcu_message_payload, 2);
}

/**
 * @brief mode1 button pressed
 * @return Void
 */
void mode1_task(void)
{
  set_stat_led1(true);
  mcu_message_payload[0] = MODE1_PAYLOAD;
  send_packet('D', (char*)&mcu_message_payload, 1);
}

/**
 * @brief mode2 button pressed
 * @return Void
 */
void mode2_task(void)
{
  set_stat_led1(false);
  mcu_message_payload[0] = MODE2_PAYLOAD;
  send_packet('D', (char*)&mcu_message_payload, 1);
}

/**
 * @brief volume up button pressed
 * @return Void
 */
void vol_up_task(void)
{
  mcu_message_payload[0] = VOL_UP_PAYLOAD;
  send_packet('D', (char*)&mcu_message_payload, 1);
}

/**
 * @brief volume down button pressed
 * @return Void
 */
void vol_down_task(void)
{
  mcu_message_payload[0] = VOL_DOWN_PAYLOAD;
  send_packet('D', (char*)&mcu_message_payload, 1);
}

/**
 * @brief set LED1 (yellow) to on or off
 * @param led_on true turns on LED, false turns off LED
 * @return Void
 */
void set_stat_led1(bool led_on)
{
  if(!led_on) PORTD &= ~_BV(UI_STAT1);
  else PORTD |= _BV(UI_STAT1);
}

/**
 * @brief set LED1 (green) to on or off
 * @param led_on true turns on LED, false turns off LED
 * @return Void
 */
void set_stat_led2(bool led_on)
{
  if(!led_on) PORTD &= ~_BV(UI_STAT2);
  else PORTD |= _BV(UI_STAT2);
}
