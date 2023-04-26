/*
 * stepper.hpp
 *
 *  Created on: Apr 25, 2023
 *      Author: alexpetrov
 */

#ifndef INC_STEPPER_HPP_
#define INC_STEPPER_HPP_

#include "main.h"
#include "stm32f3xx.h"
#include <stdint.h>
#include <stdbool.h>

#include "gpio.hpp"

typedef enum Stepper_Status {
	STOP = 0, SPIN, SPIN_AT_SPEED, SLOW_DOWN,
} Stepper_Status;

typedef enum Stepper_PosType {
	ABSOLUTE = 0, RELATIVE,
} Stepper_PosType;

class Stepper {
public:
	uint32_t pos = 0;
	bool dir = 0;
	Stepper(uint16_t steps, TIM_TypeDef *tim, GPIO *dir, GPIO *en) {
		_en = en;
		_dir = dir;
		_tim = tim;
	}
	void init(void) {
		_en->init();		// init enable gpio
		_dir->init();		// init direction gpio

		_tim->CCER |= 1;
		_tim->DIER |= 1;	// enable tim update irq
		_enable();
	}
	void brake() {
		_status = STOP;
	}
	void pause() {
		if (_status == SPIN)
			_status = SLOW_DOWN;
	}
	Stepper_Status getStatus() {
		return _status;
	}
	bool ready() {
		if (_status == STOP && _ready) {
			_ready = 0;
			return 1;
		}
		return 0;
	}
	void irq_handler(void) {
		pos += dir ? 1 : -1;
	}

private:
	TIM_TypeDef *_tim;
	GPIO *_dir;
	GPIO *_en;

	Stepper_Status _status;
	bool _ready;

protected:
	void _enable(void) {
		_en->set(false);
		_start_step();
	}
	void _disable(void) {
		_en->set(true);
		_stop_step();
	}
	void _start_step(void) {
		_tim->CR1 |= 1;
		// TIMx->BreakDeatTime->MainOutputEneble set
		_tim->BDTR |= 1 << 15;
		_tim->CNT = 0;
	}
	void _stop_step(void) {
		_tim->CR1 &= ~1;
		// TIMx->BreakDeatTime->MainOutputEneble  reset
		_tim->BDTR &= ~(1 << 15);
	}
};

#endif /* INC_STEPPER_HPP_ */
