/*
 * core.h
 *
 *  Created on: 2 мая 2023 г.
 *      Author: alexpetrov
 */

#ifndef INC_CORE_H_
#define INC_CORE_H_

#include <stdint.h>

class Core {
public:
	Core(uint32_t);
	uint32_t freq;
};

#endif /* INC_CORE_H_ */
