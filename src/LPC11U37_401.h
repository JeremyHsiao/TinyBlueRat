#ifndef _LPC11U37_401_H
#define _LPC11U37_401_H

// For more descriptive coding
#define PORT0	0
#define PORT1 	1
#define MATCH_0		0
#define MATCH_1		1
#define MATCH_2		2
#define MATCH_3		3
#define CAPTURE_0	0
#define CAPTURE_1	1

// For LPC11U38/401 (LQPF48)

//
// Timer 32 number 0
//
//#define TIMER32_B0_CAP0_PORT	0
//#define TIMER32_B0_CAP0_PIN	17
#define TIMER32_B0_CAP0_PORT	1
#define TIMER32_B0_CAP0_PIN		28

//#define TIMER32_B0_CAP1_PORT	1
//#define TIMER32_B0_CAP1_PIN		29

//#define TIMER32_B0_MAT0_PORT	0
//#define TIMER32_B0_MAT0_PIN		18
//#define TIMER32_B0_MAT0_PORT	1
//#define TIMER32_B0_MAT0_PIN		24

//#define TIMER32_B0_MAT1_PORT	0
//#define TIMER32_B0_MAT1_PIN		19
//#define TIMER32_B0_MAT1_PORT	1
//#define TIMER32_B0_MAT1_PIN		25

//#define TIMER32_B0_MAT2_PORT	0
//#define TIMER32_B0_MAT2_PIN		1
//#define TIMER32_B0_MAT2_PORT	1
//#define TIMER32_B0_MAT2_PIN		26

//#define TIMER32_B0_MAT3_PORT	0
//#define TIMER32_B0_MAT3_PIN		11
//#define TIMER32_B0_MAT3_PORT	1
//#define TIMER32_B0_MAT3_PIN		27

//
// Timer 32 number 1
//
//#define TIMER32_B0_CAP0_PORT	0
//#define TIMER32_B0_CAP0_PIN	12

#define TIMER32_B1_MAT0_PORT	0
#define TIMER32_B1_MAT0_PIN		13

//#define TIMER32_B1_MAT1_PORT	0
//#define TIMER32_B1_MAT1_PIN		14
//
//#define TIMER32_B1_MAT2_PORT	0
//#define TIMER32_B1_MAT2_PIN		15
//
//#define TIMER32_B1_MAT3_PORT	0
//#define TIMER32_B1_MAT3_PIN		16

//
// Timer 16 number 0
//
//#define TIMER16_B0_CAP0_PORT	0
//#define TIMER16_B0_CAP0_PIN	2
#define TIMER16_B0_CAP0_PORT	1
#define TIMER16_B0_CAP0_PIN		16

//#define TIMER16_B0_MAT0_PORT	0
//#define TIMER16_B0_MAT0_PIN		8
//#define TIMER16_B0_MAT0_PORT	1
//#define TIMER16_B0_MAT0_PIN		13
//
//#define TIMER16_B0_MAT1_PORT	0
//#define TIMER16_B0_MAT1_PIN		9
//#define TIMER16_B0_MAT1_PORT	1
//#define TIMER16_B0_MAT1_PIN		14
//
//#define TIMER16_B0_MAT2_PORT	0
//#define TIMER16_B0_MAT2_PIN		10
//#define TIMER16_B0_MAT2_PORT	1
//#define TIMER16_B0_MAT2_PIN		15

//
// Timer 16 number 1
//
#define TIMER16_B1_CAP0_PORT	0
#define TIMER16_B1_CAP0_PIN		20

//#define TIMER16_B1_MAT0_PORT	0
//#define TIMER16_B1_MAT0_PIN		21
//
//#define TIMER16_B1_MAT1_PORT	0
//#define TIMER16_B1_MAT1_PIN		22
//#define TIMER16_B1_MAT1_PORT	0
//#define TIMER16_B1_MAT1_PIN		23

//
// Register Mode/Function
//
#define PIO0_11_AD0_FILT			(0x02)|((0x2)<<3)|((0x1)<<8)

#define PIO1_28_FUNC_CT32B0_CAP0	(0x1)
#define PIO1_28_DEFAULT				((0x2)<<3)|((0x1)<<7)

#define PIO1_15_FUNC_PIO1_15		(0x0)
#define PIO1_15_HYSTERESIS_ENABLE	((0x1)<<5)
#define PIO1_15_DEFAULT				((0x2)<<3)|((0x1)<<7)

#define PIO1_16_FUNC_CT32B0_CAP0	(0x2)
#define PIO1_16_DEFAULT				((0x2)<<3)|((0x1)<<7)

#define PIO0_13_FUNC_CT32B1_MAT0	(0x3)
#define PIO0_13_DEFAULT				((0x2)<<3)|((0x1)<<7)

#define PIO0_18_FUNC_RX				(0x1)
#define PIO0_18_DEFAULT				((0x2)<<3)|((0x1)<<7)

#define PIO0_19_FUNC_TX				(0x1)
#define PIO0_19_DEFAULT				((0x2)<<3)|((0x1)<<7)

#define PIO0_20_FUNC_CT32B0_CAP0	(0x1)
#define PIO0_20_DEFAULT				((0x2)<<3)|((0x1)<<7)

#define PIO0_ALL_DIGITAL_MODE		((0x01)<<7)
#define PIO1_ALL_RESERVED			((0x01)<<7)

//
// Commented unused to prevent misusing
//


#endif // _LPC11U37_401_H
