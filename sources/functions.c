/*
 * functions.c
 *
 *  Created on: May 11, 2018
 *      Author: lei-n
 */

#include "functions.h"

#include "hid_mouse.h"
#include "hid_keyboard.h"

extern usb_device_hid_mouse_struct_t s_UsbDeviceHidMouse;
extern usb_device_hid_keyboard_struct_t s_UsbDeviceHidKeyboard;

#define FSM_SIZE 3

const StateType FSM_Moore[FSM_SIZE] =
{
	{&openPaint,KEYBOARD},
	{&enter,KEYBOARD},

	{&drawRectangle,MOUSE},

	{&openNotepad,KEYBOARD},
	{&enter,KEYBOARD},

	{&windowSide(LEFT),KEYBOARD},

	{&openNotepad,KEYBOARD},
	{&enter,KEYBOARD},

	{&windowSide(RIGHT),KEYBOARD},

	/*falta mover mouse a la izquierda y hacer click*/
	{&writeHelloWorld,KEYBOARD},
	{&select,KEYBOARD},
	{&copy,KEYBOARD},

	/*falta mover mouse a la derecha y hacer click*/
	{&paste,KEYBOARD},


};

volatile uint8_t position = 0;

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
	    if(count==0)
	        clearKeys();
	    else if(count == 1)
	        s_UsbDeviceHidKeyboard.buffer[2] = KEY_LEFT_GUI;
	    else if(2==count)
	        s_UsbDeviceHidKeyboard.buffer[3] = KEY_R;
	    else if(4==count){
	    	s_UsbDeviceHidKeyboard.buffer[2] = KEY_M;
	        clearKeys();
	    }
	    else if(5==count){
	    	s_UsbDeviceHidKeyboard.buffer[3] = KEY_S;
	    	s_UsbDeviceHidKeyboard.buffer[4] = KEY_P;
	    	s_UsbDeviceHidKeyboard.buffer[5] = KEY_A;
	    	s_UsbDeviceHidKeyboard.buffer[6] = KEY_I;
	    	s_UsbDeviceHidKeyboard.buffer[7] = KEY_N;
	    	s_UsbDeviceHidKeyboard.buffer[2] = KEY_T;
	    }
	    else if(6==count)
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
	    else if(4==count){
	    	s_UsbDeviceHidKeyboard.buffer[2] = KEY_N;
	        clearKeys();
	    }
	    else if(5==count){
	    	s_UsbDeviceHidKeyboard.buffer[3] = KEY_O;
	    	s_UsbDeviceHidKeyboard.buffer[4] = KEY_T;
	    	s_UsbDeviceHidKeyboard.buffer[5] = KEY_E;
	    	s_UsbDeviceHidKeyboard.buffer[6] = KEY_P;
	    	s_UsbDeviceHidKeyboard.buffer[7] = KEY_A;
	    	s_UsbDeviceHidKeyboard.buffer[2] = KEY_D;
	    }
	    else if(6==count)
	    {
	        clearKeys();
	        count = 0;
	        return 1;
	    }
	    count++;
	   return 0;
}

uint8_t notePad()
{
    static uint8_t status = 0;
    if(0==status)
    {
//        s_UsbDeviceHidKeyboard.buffer[2] = KEY_LEFTALT;
        s_UsbDeviceHidKeyboard.buffer[2] = KEY_C;
        status++;
    }
//    else if(1==status)
//    {
//        s_UsbDeviceHidKeyboard.buffer[2] = KEY_LEFTALT;
//        s_UsbDeviceHidKeyboard.buffer[3] = KEY_KEYPAD_TAB;
//        status++;
//    }
    else
    {
        s_UsbDeviceHidKeyboard.buffer[2] = 0;
//        s_UsbDeviceHidKeyboard.buffer[3] = 0;
        status=0;
        return 1;
    }
    return 0;
}

uint8_t windowSide(SIDE side)
{
    switch(side)
    {
        case RIGHT:{
        	 s_UsbDeviceHidKeyboard.buffer[2] = KEY_LEFT_GUI;
        	 s_UsbDeviceHidKeyboard.buffer[2] = KEY_RIGHTARROW;
            break;
        }
        case LEFT:{
        	 s_UsbDeviceHidKeyboard.buffer[2] = KEY_LEFT_GUI;
        	 s_UsbDeviceHidKeyboard.buffer[2] = KEY_LEFTARROW;
            break;
        }
        case UP:
        case DOWN:
        default:
            break;
    }
    return 1;
}

uint8_t writeHelloWorld()
{
	 static uint8_t count = 0;
	    if(count==0)
	        clearKeys();
	    else if(count == 1){
	    	s_UsbDeviceHidKeyboard.buffer[3] = KEY_H;
			s_UsbDeviceHidKeyboard.buffer[4] = KEY_E;
			s_UsbDeviceHidKeyboard.buffer[5] = KEY_L;
			s_UsbDeviceHidKeyboard.buffer[6] = KEY_L;
			s_UsbDeviceHidKeyboard.buffer[7] = KEY_O;
	    }
	    else if(2==count)
	        s_UsbDeviceHidKeyboard.buffer[2] = KEY_SPACEBAR;
	    else if(3==count){
	    	s_UsbDeviceHidKeyboard.buffer[3] = KEY_W;
			s_UsbDeviceHidKeyboard.buffer[4] = KEY_O;
			s_UsbDeviceHidKeyboard.buffer[5] = KEY_R;
			s_UsbDeviceHidKeyboard.buffer[6] = KEY_L;
			s_UsbDeviceHidKeyboard.buffer[7] = KEY_D;
	    }
	    else if(4==count)
	        clearKeys();
	    else if(5==count)
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
    if(FSM_SIZE == position)
    {
        clearMouse();
        clearKeys();
        return 1;
    }
    else if(device != FSM_Moore[position].device)
    {
        return 1;
    }
    else if(FSM_Moore[position].fptr())
        position++;

    return 0;
}
