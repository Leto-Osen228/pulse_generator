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
#include <math.h>

#include "config.h"

typedef enum Stepper_Status {
	STOP = 0, SPIN, SPIN_AT_SPEED, SLOW_DOWN,
} Stepper_Status;

typedef enum Stepper_PosType {
	ABSOLUTE = 0, RELATIVE,
} Stepper_PosType;

class Stepper {
public:
	int32_t pos = 0;
	int32_t set_pos = 0;
	int32_t start_pos, end_pos;
	bool dir = 0;
	uint32_t max_speed = _steps * 10;

	Stepper(uint16_t steps, TIM_TypeDef *tim, GPIO *dir, GPIO *en) {
		_en_io = en;
		_dir_io = dir;
		_tim = tim;

		_steps = steps;
	}
	void init(void) {
		_en_io->init();		// init enable gpio
		_dir_io->init();		// init direction gpio

		_tim->CCER |= 1;
		_tim->DIER |= 1;	// enable tim update irq
		_enable();
	}
	void setPos(int32_t pos, Stepper_PosType pos_type = ABSOLUTE){
		pos <<= 1;
		if (pos_type == RELATIVE){
			pos += this->set_pos;
		}
		//uint16_t arr, psc;
		//uint32_t a = core.freq / abs(velocity);
		//a+=1;
		if (pos < 0)
			dir = 1;
		else
			dir = 0;
		this->start_pos = this->set_pos;
		this->end_pos = pos;
		//s = end_pos - start_pos;
		_dir_io->set(dir);
		this->set_pos = pos;
		this->_enable();
	}
	void setSpeed(int32_t speed){
		_tim->PSC = core.freq / (2 * (_tim->ARR + 1) * speed) - 1;
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
	uint16_t get_deg(void){
		return pos % _steps * 360 / _steps;
	}
	void irq_handler(void) {
		pos += dir ? -1 : 1;
		if ((dir ? -1 : 1) * (pos - set_pos) >= 0)
			_disable();
		/*else {
			int32_t s = (end_pos - start_pos) / 2;
			if (start_pos + s > pos)
				_tim->PSC -= 10;
			else
				_tim->PSC += 10;
		}*/
	}

private:
	TIM_TypeDef *_tim;
	GPIO *_dir_io;
	GPIO *_en_io;

	uint16_t _steps = 3200;

	Stepper_Status _status;
	bool _ready;

protected:
	void _enable(void) {
		_en_io->set(false);
		_start_step();
	}
	void _disable(void) {
		_en_io->set(true);
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
