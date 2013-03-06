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
void Initialize_Digital_IO(void)
{
  Enter1State = 0;
  Enter2State = 0;
  Mode1State = 0;
  Mode2State = 0;
  VolUpState = 0;
  VolDownState = 0;
    
  // Clear the DDRC bits
  // TODO: what are each of these?
  DDRC &= ~_BV(UI_BR1);  
  DDRC &= ~_BV(UI_BR2);
  DDRC &= ~_BV(UI_BR3);
  DDRC &= ~_BV(UI_BR4);
  DDRC &= ~_BV(UI_BR5);
  DDRC &= ~_BV(UI_BR6);

  // Clear the DDRB bits
  // TODO: what are each of these?
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

  //switch off stat LEDs
  SetStatLED1(false);
  SetStatLED2(false);
}

/**
 * @brief determine whether command buttons have been pressed
 * @return Void
 * TODO document each step / if statement here
 */
void CheckCommandButtons(void)
{
  if(!(PINB & (1<<UI_ENTER1)))
  {
    if(Enter1State==0)
    {
      //_delay_ms(100);
      if(!(PINB & (1<<UI_ENTER1)))
      {
        Enter1State=1;
      }
    }
  }
  else
  {
    Enter1State=0;
  }

  if(!(PINB & (1<<UI_ENTER2)))
  {
    if(Enter2State==0)
    {
      _delay_ms(100);
      if(!(PINB & (1<<UI_ENTER2)))
      {
        Enter2State=1;
      }
    }
  }
  else
  {
    Enter2State=0;
  }

  if(!(PINB & (1<<UI_MODE1)))
  {
    if(Mode1State==0)
    {
      //_delay_ms(100);
      if(!(PINB & (1<<UI_MODE1)))
      {
        Mode1State=1;
      }
    }
  }
  else
  {
    Mode1State=0;
  }

  if(!(PINB & (1<<UI_MODE2)))
  {
    if(Mode2State==0)
    {
      //_delay_ms(100);
      if(!(PINB & (1<<UI_MODE2)))
      {
        Mode2State=1;
      }
    }
  }
  else
  {
    Mode2State=0;
  }

  if(!(PIND & (1<<UI_VOLUP)))
  {
    if(VolUpState==0)
    {
      //_delay_ms(100);
      if(!(PIND & (1<<UI_VOLUP)))
      {
        VolUpState=1;
      }
    }
  }
  else
  {
    VolUpState=0;
  }

  if(!(PIND & (1<<UI_VOLDOWN)))
  {
    if(VolDownState==0)
    {
      //_delay_ms(100);
      if(!(PIND & (1<<UI_VOLDOWN)))
      {
        VolDownState=1;
      }
    }
  }
  else
  {
    VolDownState=0;
  }
}

/**
 * @brief Run command tasks (?)
 * @return Void
 * TODO document these cases
 */
void RunCommandTasks(void)
{
  if(Enter1State==1)
  {
    Enter1Task();
    Enter1State=2;  //Set it as used
  }
  if(Enter2State==1)
  {
    Enter2Task();
    Enter2State=2;  //Set it as used
  }
  if(Mode1State==1)
  {
    Mode1Task();
    Mode1State=2;  //Set it as used
  }
  if(Mode2State==1)
  {
    Mode2Task();
    Mode2State=2;  //Set it as used
  }
  if((VolDownState >= 1) && (VolUpState >= 1)) //Flip the keyboard
  {
    if(InterfaceType==1)
    {
      InterfaceType=2;
    }
    else
    {
      InterfaceType=1;
    }
    return;
  }
  if(VolUpState==1)
  {
    VolUpTask();
    VolUpState=2;  //Set it as used
  }
  if(VolDownState==1)
  {
    VolDownTask();
    VolDownState=2;  //Set it as used
  }
}

/**
 * @brief enter1 button pressed
 * @return Void
 */
void Enter1Task(void)
{
  SetStatLED2(true);
  mcu_message_payload[0] = 0x01;
  send_packet('D',(char*)&mcu_message_payload,1);
  delay10();delay10();delay10();delay10();delay10();delay10();delay10();delay10();delay10();delay10();delay10();delay10();delay10();delay10();delay10();
  CaptureCellValue();
}

/**
 * @brief enter2 button pressed
 * @return Void
 */
void Enter2Task(void)
{
  SetStatLED2(false);
  mcu_message_payload[0] = 0x02;
  mcu_message_payload[1] = 'E';
  send_packet('D',(char*)&mcu_message_payload,2);
}

/**
 * @brief mode1 button pressed
 * @return Void
 */
void Mode1Task(void)
{
  SetStatLED1(true);
  mcu_message_payload[0] = 0x03;
  send_packet('D',(char*)&mcu_message_payload,1);
}

/**
 * @brief mode2 button pressed
 * @return Void
 */
void Mode2Task(void)
{
  SetStatLED1(false);
  mcu_message_payload[0] = 0x04;
  send_packet('D',(char*)&mcu_message_payload,1);
}

/**
 * @brief volume up button pressed
 * @return Void
 */
void VolUpTask(void)
{
  mcu_message_payload[0] = 0x06;
  send_packet('D',(char*)&mcu_message_payload,1);
}

/**
 * @brief volume down button pressed
 * @return Void
 */
void VolDownTask(void)
{
  mcu_message_payload[0] = 0x05;
  send_packet('D', (char*)&mcu_message_payload, 1);
}

// The following was commented out
// TODO find out what these were for
/*
bool Enter1_Pressed(void)
{
//  if((PINB & (1<<UI_ENTER1))) return true;
//  return false;
  if(PINB & (1<<UI_ENTER1))
  {
    return false;
  }
  else
  {
    return true;
  }
}

bool Enter2_Pressed(void)
{
//  if((PINB & (1<<UI_ENTER2))) return true;
//  return false;
  if(PINB & (1<<UI_ENTER2))
  {
    return false;
  }
  else
  {
    return true;
  }
}

bool Mode1_Pressed(void)
{
  if((PINB & (1<<UI_MODE1))) 
  {
    return false;
  }
  else
  {
    return true;
  }
}

bool Mode2_Pressed(void)
{
  if((PINB & (1<<UI_MODE2))) 
  {
    return false;
  }
  else
  {
    return true;
  }
}

bool VolUp_Pressed(void)
{
  if((PIND & (1<<UI_VOLUP))) 
  {
    return false;
  }
  else
  {
    return true;
  }
}
bool VolDown_Pressed(void)
{
  if((PIND & (1<<UI_VOLDOWN))) 
  {
    return false;
  }
  else
  {
    return true;
  }
}
*/
///////////////////////////////////

/**
 * @brief set LED1 (yellow) to on or off
 * @param bState true turns on LED, false turns off LED
 * @return Void
 */
void SetStatLED1(bool bState)
{
  if(!bState)
  {
    PORTD &= ~_BV(UI_STAT1);
  }
  else
  {
    PORTD |= _BV(UI_STAT1);
  }
}

/**
 * @brief set LED1 (green) to on or off
 * @param bState true turns on LED, false turns off LED
 * @return Void
 */
void SetStatLED2(bool bState)
{
  if(!bState)
  {
    PORTD &= ~_BV(UI_STAT2);
  }
  else
  {
    PORTD |= _BV(UI_STAT2);
  }
}
