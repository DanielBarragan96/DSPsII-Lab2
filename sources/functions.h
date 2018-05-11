/*
 * functions.h
 *
 *  Created on: May 11, 2018
 *      Author: lei-n
 */

#ifndef FUNCTIONS_H_
#define FUNCTIONS_H_


#include "hid_mouse.h"
#include "hid_keyboard.h"

typedef enum
{
    MOUSE,
    KEYBOARD
}DEVICE;

typedef struct
{
    uint8_t (*fptr)();
    DEVICE device;
}StateType;

uint8_t paintCommand();

uint8_t notePad();

uint8_t windowSide(SIDE side);

uint8_t writeHelloWorld();

uint8_t copy();

uint8_t drawRectangle();

uint8_t functionHandler(DEVICE device);

#endif /* FUNCTIONS_H_ */