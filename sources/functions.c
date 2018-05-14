/*
 * functions.c
 *
 *  Created on: May 11, 2018
 *      Author: lei-n
 */

#include "functions.h"

#include "hid_mouse.h"
#include "hid_keyboard.h"

#include "FreeRTOS.h"
#include "timers.h"

extern usb_device_hid_mouse_struct_t s_UsbDeviceHidMouse;
extern usb_device_hid_keyboard_struct_t s_UsbDeviceHidKeyboard;

#define FSM_SIZE 15
#define DELAY 300

const StateType FSM_Moore[FSM_SIZE] =
{
	{&openPaint,KEYBOARD},
	{&drawRectangle,MOUSE},

	{&openNotepad,KEYBOARD},
	{&enter,KEYBOARD},

	{&windowSideLeft,KEYBOARD},

	{&openNotepad,KEYBOARD},
	{&enter,KEYBOARD},

	{&windowSideRight,KEYBOARD},

	{&movePosition,KEYBOARD},
	{&writeHelloWorld,KEYBOARD},
	{&select,KEYBOARD},
	{&copy,KEYBOARD},

	{&movePosition,KEYBOARD},
	{&paste,KEYBOARD},


};

volatile uint8_t g_position = 0;

bool wait = false;
TimerHandle_t g_delay_timer = NULL;

void TimerCallback (TimerHandle_t timeIn)
{
    wait = false;
}

void initFunctions()
{
    const TickType_t g_xTimerPeriod = pdMS_TO_TICKS(DELAY); //periodo a interrumpir
   //Interrupt I2C nw
   const char *pcTimerName = "Timer";    //nombre
   const UBaseType_t uxAutoReload = pdFALSE;    //si se hace auto reload
   void * const pvTimerID = 0; //handle de las tareas, regresa un valor para identificar la tarea
   TimerCallbackFunction_t pxCallbackFunction = TimerCallback; //callback function

   //se crea el timer, es global, tipo TimerHandle_t
   g_delay_timer = xTimerCreate (pcTimerName, g_xTimerPeriod, uxAutoReload,
           pvTimerID, pxCallbackFunction);
}

void clearKeys()
{
    s_UsbDeviceHidKeyboard.buffer[2] = 0;
    s_UsbDeviceHidKeyboard.buffer[3] = 0;
    s_UsbDeviceHidKeyboard.buffer[4] = 0;
	s_UsbDeviceHidKeyboard.buffer[5] = 0;
	s_UsbDeviceHidKeyboard.buffer[6] = 0;
	s_UsbDeviceHidKeyboard.buffer[7] = 0;
}

void clearMouse()
{
    s_UsbDeviceHidMouse.buffer[0] = 0U;
    s_UsbDeviceHidMouse.buffer[1] = 0U;
    s_UsbDeviceHidMouse.buffer[2] = 0U;
}

uint8_t paintCommand()
{
    s_UsbDeviceHidKeyboard.buffer[2] = KEY_PAGEUP;
    return 1;
}

uint8_t openPaint()
{
    static uint8_t count = 0;
	    if(0==count)
	        clearKeys();
	    else if(1==count)
	        s_UsbDeviceHidKeyboard.buffer[2] = KEY_LEFT_GUI;
	    else if(2==count)
	        s_UsbDeviceHidKeyboard.buffer[3] = KEY_R;
	    else if(4==count){
//	    	s_UsbDeviceHidKeyboard.buffer[2] = KEY_M;
	        clearKeys();
	    }
	    else if(5==count)
	    	s_UsbDeviceHidKeyboard.buffer[3] = KEY_M;
	    else if(6==count)
	        s_UsbDeviceHidKeyboard.buffer[3] = KEY_S;
	    else if(7==count)
	        s_UsbDeviceHidKeyboard.buffer[3] = KEY_P;
	    else if(8==count)
            s_UsbDeviceHidKeyboard.buffer[3] = KEY_A;
	    else if(9==count)
            s_UsbDeviceHidKeyboard.buffer[3] = KEY_I;
	    else if(10==count)
            s_UsbDeviceHidKeyboard.buffer[3] = KEY_N;
	    else if(11==count)
	        s_UsbDeviceHidKeyboard.buffer[3] = KEY_T;
        else if(12==count)
	        s_UsbDeviceHidKeyboard.buffer[3] = KEY_ENTER;

	    else if(13==count)
	    {
	        clearKeys();
	        count = 0;
	        return 1;
	    }
	    count++;
	   return 0;
}

uint8_t openNotepad()
{
    static uint8_t count = 0;
	    if(count==0)
	        clearKeys();
	    else if(count == 1)
	        s_UsbDeviceHidKeyboard.buffer[2] = KEY_LEFT_GUI;
	    else if(2==count)
	        s_UsbDeviceHidKeyboard.buffer[3] = KEY_R;
	    else if(2==count)
	        clearKeys();
	    else if(4==count)
	    	s_UsbDeviceHidKeyboard.buffer[2] = KEY_N;
	    else if(5==count)
	    	s_UsbDeviceHidKeyboard.buffer[2] = KEY_O;
	    else if(6==count)
	    	s_UsbDeviceHidKeyboard.buffer[2] = KEY_T;
	    else if(7==count)
	    	s_UsbDeviceHidKeyboard.buffer[2] = KEY_E;
	    else if(8==count)
	    	s_UsbDeviceHidKeyboard.buffer[2] = KEY_P;
	    else if(9==count)
	    	s_UsbDeviceHidKeyboard.buffer[2] = KEY_A;
	    else if(10==count)
	    	s_UsbDeviceHidKeyboard.buffer[2] = KEY_D;
	    else if(11==count)
	    {
	        clearKeys();
	        count = 0;
	        return 1;
	    }
	    count++;
	   return 0;
}

uint8_t windowSideRight()
{
    static uint8_t count = 0;
	    if(count==0)
	        clearKeys();
	    else if(count == 1)
	    	s_UsbDeviceHidKeyboard.buffer[2] = KEY_LEFT_GUI;
	    else if(2==count)
	    	  s_UsbDeviceHidKeyboard.buffer[3] = KEY_RIGHTARROW;
	    else if(4==count)
	    {
	        clearKeys();
	        count = 0;
	        return 1;
	    }
	    count++;
	    return 0;
}

uint8_t windowSideLeft()
{
  static uint8_t count = 0;
  if(count==0)
            clearKeys();
    else if(count == 1)
        s_UsbDeviceHidKeyboard.buffer[2] = KEY_LEFT_GUI;
    else if(2==count)
        s_UsbDeviceHidKeyboard.buffer[3] = KEY_LEFTARROW;
    else if(4==count)
    {
        clearKeys();
        count = 0;
        return 1;
    }
    count++;
    return 0;
}

uint8_t writeHelloWorld()
{
	 static uint8_t count = 0;
	    if(count==0)
	        clearKeys();
	    else if(count == 1)
	    	s_UsbDeviceHidKeyboard.buffer[2] = KEY_H;
	    else if(count == 2)
			s_UsbDeviceHidKeyboard.buffer[2] = KEY_E;
	    else if(count == 3)
			s_UsbDeviceHidKeyboard.buffer[2] = KEY_L;
	    else if(count == 4)
	        s_UsbDeviceHidKeyboard.buffer[2] = 0;
        else if(count == 5)
			s_UsbDeviceHidKeyboard.buffer[2] = KEY_L;
	    else if(count == 6)
			s_UsbDeviceHidKeyboard.buffer[2] = KEY_O;
	    else if(count == 7)
	        s_UsbDeviceHidKeyboard.buffer[2] = KEY_SPACEBAR;
        else if(count == 8)
            s_UsbDeviceHidKeyboard.buffer[2] = 0;
	    else if(count == 9)
	    	s_UsbDeviceHidKeyboard.buffer[2] = KEY_W;
	    else if(count == 10)
			s_UsbDeviceHidKeyboard.buffer[2] = KEY_O;
	    else if(count == 11)
			s_UsbDeviceHidKeyboard.buffer[2] = KEY_R;
        else if(count == 12)
            s_UsbDeviceHidKeyboard.buffer[2] = 0;
	    else if(count == 13)
			s_UsbDeviceHidKeyboard.buffer[2] = KEY_L;
	    else if(count == 14)
			s_UsbDeviceHidKeyboard.buffer[2] = KEY_D;
	    else if(count == 15)
	        clearKeys();
	    else if(count == 16)
	    {
	        clearKeys();
	        count = 0;
	        return 1;
	    }
	    count++;
	    return 0;

    return 0;
}

uint8_t select()
{
    static uint8_t count = 0;
    if(count==0)
        clearKeys();
    else if(count == 1)
        s_UsbDeviceHidKeyboard.buffer[2] = KEY_LEFTCONTROL;
    else if(2==count)
        s_UsbDeviceHidKeyboard.buffer[3] = KEY_E;
    else if(3==count)
        clearKeys();
    else if(4==count)
    {
        clearKeys();
        count = 0;
        return 1;
    }
    count++;
    return 0;
}

uint8_t copy()
{
    static uint8_t count = 0;
    if(count==0)
        clearKeys();
    else if(count == 1)
        s_UsbDeviceHidKeyboard.buffer[2] = KEY_LEFTCONTROL;
    else if(2==count)
        s_UsbDeviceHidKeyboard.buffer[3] = KEY_C;
    else if(4==count)
        clearKeys();
    else if(5==count)
        s_UsbDeviceHidKeyboard.buffer[2] = KEY_RIGHTARROW;
    else if(6==count)
    {
        clearKeys();
        count = 0;
        return 1;
    }
    count++;
    return 0;
}

uint8_t paste()
{
    static uint8_t count = 0;
    if(count==0)
        clearKeys();
    else if(count == 1)
        s_UsbDeviceHidKeyboard.buffer[2] = KEY_LEFTCONTROL;
    else if(2==count)
        s_UsbDeviceHidKeyboard.buffer[3] = KEY_V;
    else if(3==count)
    {
        clearKeys();
        count = 0;
        return 1;
    }
    count++;
    return 0;
}

uint8_t enter()
{
    static uint8_t x = 0;
    if(0==x)
    {
        clearKeys();
    }
    else if(1==x)
    {
        s_UsbDeviceHidKeyboard.buffer[2] = KEY_ENTER;
    }
    else
    {
        clearKeys();
        x = 0;
        return 1;
    }
    x++;
    return 0;
}

uint8_t movePosition()
{
    static uint8_t x = 0;
    if(0==x)
    {
        clearKeys();
    }
    else if(1==x)
    {
    	s_UsbDeviceHidKeyboard.buffer[2] = KEY_LEFTALT;
        s_UsbDeviceHidKeyboard.buffer[3] = KEY_TAB;
    }
    else
    {
        clearKeys();
        x = 0;
        return 1;
    }
    x++;
    return 0;
}


uint8_t drawRectangle()
{
    static uint8_t  x=0;
    static uint8_t  y=0;
    static bool x_y = true;
    static bool state = true;
    static uint8_t status = 0;

    if(x_y)
    {
        if(state)
        {//RIGHT
            s_UsbDeviceHidMouse.buffer[0] = 1U;
            status = 0;
            s_UsbDeviceHidMouse.buffer[1] = 1U;
            s_UsbDeviceHidMouse.buffer[2] = 0U;
            x++;
            if(x>100U)
            {//DOWN
                x_y = false;
                state = true;
            }
        }
        else
        {//LEFT
            s_UsbDeviceHidMouse.buffer[1] = (uint8_t )(0xFFU);
            s_UsbDeviceHidMouse.buffer[2] = 0U;
            x--;
            if(x<1U)
            {//UP
                x_y = false;
                state = false;
            }
        }
    }
    else
    {
        if(state)
        {//DOWN
            s_UsbDeviceHidMouse.buffer[1] = 0U;
            s_UsbDeviceHidMouse.buffer[2] = 1U;
            y++;
            if(y>99U)
            {//LEFT
                x_y = true;
                state = false;
            }
        }
        else
        {//UP
            s_UsbDeviceHidMouse.buffer[1] = 0U;
            s_UsbDeviceHidMouse.buffer[2] = (uint8_t )(0xFFU);
            y--;
            if(y<1U)
            {
                x_y = true;
                state = true;
                status = 1;
                clearMouse();
                x = 0;
                y = 0;
            }
        }
    }

   return status;
}

uint8_t functionHandler(DEVICE device)
{
    if(FSM_SIZE == g_position)
    {
        clearMouse();
        clearKeys();
        return 1;
    }
    else if(device != FSM_Moore[g_position].device || wait)
    {
        return 1;
    }
    if(FSM_Moore[g_position].fptr())
    {
        g_position++;
        clearMouse();
        clearKeys();
    }

    if(KEYBOARD == device)
    {
        wait = true;
        xTimerStartFromISR(g_delay_timer, NULL);
    }
    return 0;
}
