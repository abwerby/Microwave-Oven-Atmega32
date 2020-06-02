/*
 * microwave_int.h
 *
 * Created: 6/2/2020 8:16:32 AM
 *  Author: abdelrhman
 */ 


#ifndef MICROWAVE_INT_H_
#define MICROWAVE_INT_H_

void (*state)();
void (*last_state)();

void IDLE();
void HEAT();
void PAUSE();


/* OUTPUTS BUTTON PINS */
#define GREEN_PIN 0
#define RED_PIN 1
#define YELLOW_PIN 2
#define MOTOR_PIN 3
#define HEATER_PIN 4
/* INPUTS BUTTON PINS */
#define START_PIN 0
#define STOP_PIN 1
#define SS_PIN 2
#define MM_PIN 3
#define MM10_PIN 4

#endif /* MICROWAVE_INT_H_ */