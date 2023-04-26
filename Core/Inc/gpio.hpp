/*
 * gpio.hpp
 *
 *  Created on: Apr 25, 2023
 *      Author: alexpetrov
 */

#ifndef SRC_GPIO_HPP_
#define SRC_GPIO_HPP_

#include "main.h"
#include "stm32f3xx.h"
#include <stdint.h>
#include <stdbool.h>

typedef enum GPIO_Type {
	INPUT = 0b00, OUTPUT, ALTERNATIVE, ANALOG,
} GPIO_Type;

typedef enum GPIO_Pull_Mode {
	NO = 0b00, UP, DOWN,
} Pull_Mode;

class GPIO {
public:
	GPIO(GPIO_TypeDef *gpio, uint8_t pin, GPIO_Type type = INPUT,
			GPIO_Pull_Mode pull = NO) {
		_gpio = gpio;
		_pin = pin;
		_pin_mask = 1 << pin;
		_pull = pull;
		_type = type;
	}

	void init(void) {
		RCC->AHBENR |= RCC_AHBENR_GPIOAEN + GPIO_GET_INDEX(_gpio);

		_gpio->PUPDR &= ~(0b11 << (_pin << 1));
		_gpio->PUPDR |= _pull << (_pin << 1);

		_gpio->MODER &= ~(0b11 << (_pin << 1));
		_gpio->MODER |= _type << (_pin << 1);
	}

	void set(bool val) {
		if (val)
			_gpio->ODR |= _pin_mask;
		else
			_gpio->ODR &= ~_pin_mask;
	}
	bool get(void) {
		return _gpio->IDR & _pin_mask;
	}
private:
	GPIO_TypeDef *_gpio;
	uint32_t _pin;
	uint32_t _pin_mask;
	GPIO_Type _type;
	GPIO_Pull_Mode _pull;
};

#endif /* SRC_GPIO_HPP_ */
