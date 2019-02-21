/*
 * Common.h
 *
 *  Created on: 2013/1/22
 *      Author: Jeremy.Hsiao
 */

#ifndef COMMON_H_
#define COMMON_H_

extern inline uint16_t INC_INDEX16(uint16_t index, uint16_t max);
extern inline uint8_t IN_RANGE16(uint16_t value, uint16_t min, uint16_t max);
extern inline uint8_t CHECK_LEVEL_RANGE16(uint8_t level, uint8_t expected_level, uint16_t value, uint16_t min, uint16_t max);
extern inline uint32_t TIMER_DIFF32(uint32_t current, uint32_t previous);
extern inline uint16_t TIMER_DIFF16(uint16_t current, uint16_t previous);
extern inline uint8_t INC_INDEX8(uint8_t index, uint32_t max);

#endif /* COMMON_H_ */
