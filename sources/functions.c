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
    {&drawRectangle,MOUSE},
    {&drawRectangle,MOUSE},
    {&notePad,KEYBOARD}
};

volatile uint8_t position = 0;

uint8_t paintCommand()
{
    s_UsbDeviceHidKeyboard.buffer[2] = KEY_PAGEUP;
    return 1;
}

uint8_t notePad()
{
    static uint8_t status = 0;
    if(0==status)
    {
        s_UsbDeviceHidKeyboard.buffer[2] = KEY_LEFTALT;
        s_UsbDeviceHidKeyboard.buffer[3] = KEY_KEYPAD_TAB;
        status++;
    }
    else if(1==status)
    {
        s_UsbDeviceHidKeyboard.buffer[2] = KEY_LEFTALT;
        s_UsbDeviceHidKeyboard.buffer[3] = KEY_KEYPAD_TAB;
        status++;
    }
    else
    {
        s_UsbDeviceHidKeyboard.buffer[2] = 0;
        s_UsbDeviceHidKeyboard.buffer[3] = 0;
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

            break;
        }
        case LEFT:{

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

    return 0;
}

uint8_t copy()
{
    s_UsbDeviceHidKeyboard.buffer[2] = KEY_COPY;
    return 1;
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
                s_UsbDeviceHidMouse.buffer[0] = 0U;
                status = 1;
                s_UsbDeviceHidMouse.buffer[1] = 0U;
                s_UsbDeviceHidMouse.buffer[2] = 0U;
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
        s_UsbDeviceHidMouse.buffer[0] = 0U;
        s_UsbDeviceHidMouse.buffer[1] = 0U;
        s_UsbDeviceHidMouse.buffer[2] = 0U;

        s_UsbDeviceHidKeyboard.buffer[1] = 0U;
        s_UsbDeviceHidKeyboard.buffer[2] = 0U;
        s_UsbDeviceHidKeyboard.buffer[3] = 0U;
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
