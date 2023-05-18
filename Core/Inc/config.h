/*
 * config.h
 *
 *  Created on: May 2, 2023
 *      Author: alexpetrov
 */

#ifndef INC_CONFIG_H_
#define INC_CONFIG_H_

#define BTN GPIOA, 0

#define RIGHT_STEPPER_EN GPIOD, 5
#define RIGHT_STEPPER_DIR GPIOD, 7

#define LEFT_STEPPER_EN GPIOD, 6
#define LEFT_STEPPER_DIR GPIOB, 3

#include <core.h>
Core core(72000000);


#include "gpio.hpp"
GPIO led(GPIOE, 8, OUTPUT, NO);
GPIO btn(BTN, INPUT, NO);

GPIO right_dir(RIGHT_STEPPER_DIR, OUTPUT);
GPIO right_en(RIGHT_STEPPER_EN, OUTPUT);

#endif /* INC_CONFIG_H_ */
