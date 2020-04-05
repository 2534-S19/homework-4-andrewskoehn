/*
 * homework4.h
 *
 *  Created on: 4/4/2020
 *      Author: Andrew Koehn
 */

#ifndef HOMEWORK4_H_
#define HOMEWORK4_H_

// This function initializes the board by turning off the Watchdog Timer.
void initBoard();

//FSM for detecting character sequence (returns true if the correct sequence is detected; false if other)
bool charFSM(char rChar);

// TODO: Define any constants that are local to homework.c using #define
#define NO_CHAR 0xFF
typedef enum{SX, S2, S25, S253} codeState;


#endif /* HOMEWORK4_H_ */
