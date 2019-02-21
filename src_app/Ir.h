
#include "Define.h"

#ifndef _IR_H_
#define _IR_H_

/* definition and constants */

#define RC6_MODE_TOGGLE_BIT     (1 << 7)
#define RC5X_MODE_TOGGLE_BIT    (1 << 7)

/* RC6 definition */
#define RC6_LEVEL_BIT       (1)
#define RC6_LEVEL_HIGH      (1)
#define RC6_LEVEL_LOW       (0)

#define RC6_1T              (440 / 1)      // unit: 1 us
#define RC6_2T              (RC6_1T * 2)
#define RC6_3T              (RC6_1T * 3)
#define RC6_6T              (RC6_1T * 6)

#define RC6_1T_TOL          (RC6_1T / 4)		// 3--->4
#define RC6_2T_TOL          (RC6_1T_TOL * 2)
#define RC6_3T_TOL          (RC6_1T_TOL * 3)
#define RC6_6T_TOL          (RC6_1T_TOL * 6)

#define RC6_MB2_BIT         (1 << 2)
#define RC6_MB1_BIT         (1 << 1)
#define RC6_MB0_BIT         (1 << 0)

#define RC6_CTRL_MSB_BIT    (15)
#define RC6_CTRL7_BIT       (1 << 15)

/* RC5/RC5X definition */
#define RC5X_LEVEL_BIT      (1)
#define RC5X_LEVEL_HIGH     (1)
#define RC5X_LEVEL_LOW      (0)

#define RC5X_1T             (889 / 1)      // unit: 1 us
#define RC5X_2T             (RC5X_1T * 2)

#define RC5X_1T_TOL         (RC5X_1T / 6)  //3--->6
#define RC5X_2T_TOL         (RC5X_1T_TOL * 2)

#define RC5X_ADDR_MSB_BIT   (4 + 8)

#define RC5X_COMM_BIT6      (1 << 6)
#define RC5X_COMM_MSB_BIT   (5)

/* NEC definition */
#define NEC_LEVEL_BIT       (1)
#define NEC_LEVEL_HIGH      (1)
#define NEC_LEVEL_LOW       (0)

#define NEC_1T              (560 / 1)      // unit: 1 us
#define NEC_3T              (NEC_1T * 3)
#define NEC_8T              (NEC_1T * 8)
#define NEC_16T             (NEC_1T * 16)

#define NEC_1T_TOL          (NEC_1T / 3)
#define NEC_3T_TOL          (NEC_1T_TOL * 3)
#define NEC_8T_TOL          (NEC_1T_TOL * 8)
#define NEC_16T_TOL         (NEC_1T_TOL * 16)

/* NEC definition */
#define RCA_LEVEL_BIT       (1)
#define RCA_LEVEL_HIGH      (1)
#define RCA_LEVEL_LOW       (0)

#define RCA_1T              (500 / 1)      // unit: 1 us
#define RCA_2T              (RCA_1T * 2)
#define RCA_3T              (RCA_1T * 3)
#define RCA_4T              (RCA_1T * 4)
#define RCA_8T              (RCA_1T * 8)
#define RCA_16T             (RCA_1T * 16)

#define RCA_1T_TOL          (RCA_1T / 3)
#define RCA_2T_TOL          (RCA_2T / 3)
#define RCA_3T_TOL          (RCA_1T_TOL * 3)
#define RCA_4T_TOL          (RCA_1T_TOL * 4)
#define RCA_8T_TOL          (RCA_1T_TOL * 8)
#define RCA_16T_TOL         (RCA_1T_TOL * 16)
/* SONY definition */
#define SONY_LEVEL_BIT      (1)
#define SONY_LEVEL_HIGH     (1)
#define SONY_LEVEL_LOW      (0)

#define SONY_1T             (600 / 1)      // unit: 1 us
#define SONY_2T             (SONY_1T * 2)
#define SONY_4T             (SONY_1T * 4)

#define SONY_1T_TOL         (SONY_1T / 5)	// 3---> 5
#define SONY_2T_TOL         (SONY_1T_TOL * 2)
#define SONY_4T_TOL         (SONY_1T_TOL * 4)

/* SHARP definition */
#define SHARP_LEVEL_BIT       (1)
#define SHARP_LEVEL_HIGH      (1)
#define SHARP_LEVEL_LOW       (0)

#define SHARP_1T              (320 / 1) // unit: 1 us 	/16
#define SHARP_2T              (680 / 1)//(SHARP_1T * 2) 	/34		1000-320=680
#define SHARP_6T              (1680 / 1)//(SHARP_1T * 8)	/84		2000-320=1680
#define SHARP_125T            (40000/1)//(SHARP_1T * 125)	/2000	//40000/320=125

#define SHARP_1T_TOL          (SHARP_1T / 2)//8  /3--->/2
#define SHARP_2T_TOL          (SHARP_2T / 3)//11
#define SHARP_6T_TOL          (SHARP_6T / 3)//28
#define SHARP_125T_TOL        (SHARP_125T /3)//700

/* Matsushita definition */
#define MAT_LEVEL_BIT       (1)
#define MAT_LEVEL_HIGH      (1)
#define MAT_LEVEL_LOW       (0)

#define MAT_1T              (440 / 1)      // unit: 1 us
#define MAT_2T              (MAT_1T * 2)
#define MAT_6T              (MAT_1T * 6)
#define MAT_8T              (MAT_1T * 8)

#define MAT_1T_TOL          (MAT_1T / 5)
#define MAT_2T_TOL          (MAT_1T_TOL * 2)
#define MAT_6T_TOL          (MAT_1T_TOL * 6)
#define MAT_8T_TOL          (MAT_1T_TOL * 8)

/* PANA definition */
#define PANA_LEVEL_BIT       (1)
#define PANA_LEVEL_HIGH      (1)
#define PANA_LEVEL_LOW       (0)

#define PANA_1T              (436 / 1)      // unit: 1 us
#define PANA_3T              (PANA_1T * 3)
#define PANA_4T              (PANA_1T * 4)
#define PANA_8T              (PANA_1T * 8)
#define PANA_16T             (PANA_1T * 16)

#define PANA_1T_TOL          (PANA_1T / 4)		// 3--->4
#define PANA_3T_TOL          (PANA_1T_TOL * 3)
#define PANA_4T_TOL          (PANA_1T_TOL * 4)
#define PANA_8T_TOL          (PANA_1T_TOL * 8)
#define PANA_16T_TOL         (PANA_1T_TOL * 16)

/* RCMM definition */
#define RCMM_LEVEL_BIT       (1)
#define RCMM_LEVEL_HIGH      (1)
#define RCMM_LEVEL_LOW       (0)

#define RCMM_1T              (27.78 / 1)      // unit: 1 us
#define RCMM_3T              (RCMM_1T * 3)
#define RCMM_4T              (RCMM_1T * 4)
#define RCMM_6T              (RCMM_1T * 6)
#define RCMM_8T              (RCMM_1T * 8)
#define RCMM_10T             (RCMM_1T * 10)
#define RCMM_15T             (RCMM_1T * 15)
#define RCMM_16T             (RCMM_1T * 16)
#define RCMM_20T             (RCMM_1T * 20)
#define RCMM_22T             (RCMM_1T * 22)
#define RCMM_28T             (RCMM_1T * 28)
#define RCMM_34T             (RCMM_1T * 34)
#define RCMM_40T             (RCMM_1T * 40)

#define RCMM_1T_TOL          (RCMM_1T / 6)
#define RCMM_3T_TOL          (RCMM_1T_TOL * 3)
#define RCMM_4T_TOL          (RCMM_1T_TOL * 4)
#define RCMM_6T_TOL          (RCMM_1T_TOL * 6)
#define RCMM_8T_TOL          (RCMM_1T_TOL * 8)
#define RCMM_16T_TOL         (RCMM_1T_TOL * 16)
#define RCMM_20T_TOL         (RCMM_1T_TOL * 20)
#define RCMM_22T_TOL         (RCMM_1T_TOL * 22)
#define RCMM_28T_TOL         (RCMM_1T_TOL * 28)
#define RCMM_34T_TOL         (RCMM_1T_TOL * 34)
#define RCMM_40T_TOL         (RCMM_1T_TOL * 40)
#define RCMM_HEADER_TOL      (58)
#define RCMM_DATA_H_TOL      (86)
#define RCMM_00_TOL      	 (58)
#define RCMM_01_TOL      	 (58)
#define RCMM_10_TOL      	 (58)
#define RCMM_11_TOL      	 (58)
#define RCMM_GAP  	    	 (50000)

/* RC Encoder */
#define CONTER_OFFSET_RC5   10
#define CONTER_OFFSET_RC6   30
#define CONTER_OFFSET_NEC   30
#define CONTER_OFFSET_SHARP 30
#define CONTER_OFFSET_MAT   30
#define CONTER_OFFSET_PANA  30
#define CONTER_OFFSET_RCMM  30
#define CONTER_OFFSET_RCA  30

#define IR_TBL_MAX      250 //108 //72

#define IR_RAM_TBL_MAX  49 //48 /add one for PC direct code cmd

typedef struct _RC_TIME_TABLE
{
	uint32_t  wTimeStamp;  // width
    Byte  bLevel; // height
}sRC_TIME_TABLE;

typedef struct _SNVRAM_IR_CODE
{
	Byte Type;
	Byte Mode;		    
	Byte Address;
  	Byte Cmd;
  	Byte Repeat;
    Byte Rev0;
    Byte Rev1;
    Byte Rev2;
}sNVRAM_IR_INFO ;

typedef struct _IR_STATUS
{
	Byte IrType;
	Byte Mode;		    
  	Byte Repeat;
    Word IrKeyCode;
    Word IrKeyCmdCode;
  	Byte RepeatCountDown;
    Byte bToggle;
	Byte bKeyPressed; 

    Byte bScriptNum;
    Word wScriptTime;
    Word wScriptStepDelay;
    
    Byte bMarcoRun;
}sIR_STATUS_INFO;

typedef struct
{
    BYTE    uc_main_stage;
    BYTE    uc_sub_stage;
    WORD    ui2_mode_addr;
    WORD    ui2_ctrl_info;
    WORD    ui2_old_time_tick;
}st_IR;

enum _RC_TPE_
{
	RC_NULL	    = 0x00,
	RC5_INDEX	= 0x05,
	RC6_INDEX	= 0x06,
	NEC1_INDEX	= 0x07,
	NEC2_INDEX	= 0x08,
	SONY_INDEX	= 0x0A,
	SHARP_INDEX	= 0x09,
	MAT_INDEX	= 0x0B,
	PANA_INDEX	= 0x0C,
	RCMM_INDEX  = 0x0D,
	RCA_INDEX	= 0x0E
};

enum
{
    IR_RC6_STAGE_LS,        /* 0: Leader Symbol */
    IR_RC6_STAGE_SB,        /* 1: Start Bit */
    IR_RC6_STAGE_MB2,       /* 2: Mode Bit 2 */
    IR_RC6_STAGE_MB1,       /* 3: Mode Bit 1 */
    IR_RC6_STAGE_MB0,       /* 4: Mode Bit 0 */
    IR_RC6_STAGE_TR,        /* 5: Trailer Bit */
    IR_RC6_STAGE_CTRL7,     /* 6: Control Bit 7 */
    IR_RC6_STAGE_CTRL6,     /* 7: Control Bit 6 */
    IR_RC6_STAGE_CTRL5,     /* 8: Control Bit 5 */
    IR_RC6_STAGE_CTRL4,     /* 9: Control Bit 4 */
    IR_RC6_STAGE_CTRL3,     /* 10: Control Bit 3 */
    IR_RC6_STAGE_CTRL2,     /* 11: Control Bit 2 */
    IR_RC6_STAGE_CTRL1,     /* 12: Control Bit 1 */
    IR_RC6_STAGE_CTRL0,     /* 13: Control Bit 0 */
    IR_RC6_STAGE_INFO7,     /* 14: Infomation Bit 7 */
    IR_RC6_STAGE_INFO6,     /* 15: Infomation Bit 6 */
    IR_RC6_STAGE_INFO5,     /* 16: Infomation Bit 5 */
    IR_RC6_STAGE_INFO4,     /* 17: Infomation Bit 4 */
    IR_RC6_STAGE_INFO3,     /* 18: Infomation Bit 3 */
    IR_RC6_STAGE_INFO2,     /* 19: Infomation Bit 2 */
    IR_RC6_STAGE_INFO1,     /* 20: Infomation Bit 1 */
    IR_RC6_STAGE_INFO0,     /* 21: Infomation Bit 0 */
    IR_RC6_STAGE_FINISH     /* 22: */
};

enum
{
    IR_RC5X_STAGE_S1,       /* 0: Start Bit 1 */
    IR_RC5X_STAGE_S2,       /* 1: Start Bit 2 */
    IR_RC5X_STAGE_T,        /* 2: Toggle Bit */
    IR_RC5X_STAGE_ADDR4,    /* 3: Address Bit 4 */
    IR_RC5X_STAGE_ADDR3,    /* 4: Address Bit 3 */
    IR_RC5X_STAGE_ADDR2,    /* 5: Address Bit 2 */
    IR_RC5X_STAGE_ADDR1,    /* 6: Address Bit 1 */
    IR_RC5X_STAGE_ADDR0,    /* 7: Address Bit 0 */
    IR_RC5X_STAGE_COMM5,    /* 8: Command Bit 5 */
    IR_RC5X_STAGE_COMM4,    /* 9: Command Bit 4 */
    IR_RC5X_STAGE_COMM3,    /* 10: Command Bit 3 */
    IR_RC5X_STAGE_COMM2,    /* 11: Command Bit 2 */
    IR_RC5X_STAGE_COMM1,    /* 12: Command Bit 1 */
    IR_RC5X_STAGE_COMM0,    /* 13: Command Bit 0 */
    IR_RC5X_STAGE_FINISH    /* 14: */
};

enum
{
    IR_NEC_STAGE_LP,        /* 0: Leader Pulse */
    IR_NEC_STAGE_ADDR0,     /* 1: Address 0 */
    IR_NEC_STAGE_ADDR1,     /* 2: Address 1 */
    IR_NEC_STAGE_ADDR2,     /* 3: Address 2 */
    IR_NEC_STAGE_ADDR3,     /* 4: Address 3 */
    IR_NEC_STAGE_ADDR4,     /* 5: Address 4 */
    IR_NEC_STAGE_ADDR5,     /* 6: Address 5 */
    IR_NEC_STAGE_ADDR6,     /* 7: Address 6 */
    IR_NEC_STAGE_ADDR7,     /* 8: Address 7 */
    IR_NEC_STAGE_C_ADDR0,   /* 9:  Complement Address 0 */
    IR_NEC_STAGE_C_ADDR1,   /* 10: Complement Address 1 */
    IR_NEC_STAGE_C_ADDR2,   /* 11: Complement Address 2 */
    IR_NEC_STAGE_C_ADDR3,   /* 12: Complement Address 3 */
    IR_NEC_STAGE_C_ADDR4,   /* 13: Complement Address 4 */
    IR_NEC_STAGE_C_ADDR5,   /* 14: Complement Address 5 */
    IR_NEC_STAGE_C_ADDR6,   /* 15: Complement Address 6 */
    IR_NEC_STAGE_C_ADDR7,   /* 16: Complement Address 7 */
    IR_NEC_STAGE_COMM0,     /* 17: Command Bit 0 */
    IR_NEC_STAGE_COMM1,     /* 18: Command Bit 1 */
    IR_NEC_STAGE_COMM2,     /* 19: Command Bit 2 */
    IR_NEC_STAGE_COMM3,     /* 20: Command Bit 3 */
    IR_NEC_STAGE_COMM4,     /* 21: Command Bit 4 */
    IR_NEC_STAGE_COMM5,     /* 22: Command Bit 5 */
    IR_NEC_STAGE_COMM6,     /* 23: Command Bit 6 */
    IR_NEC_STAGE_COMM7,     /* 24: Command Bit 7 */
    IR_NEC_STAGE_C_COMM0,   /* 25: Complement Command Bit 0 */
    IR_NEC_STAGE_C_COMM1,   /* 26: Complement Command Bit 1 */
    IR_NEC_STAGE_C_COMM2,   /* 27: Complement  Command Bit 2 */
    IR_NEC_STAGE_C_COMM3,   /* 28: Complement Command Bit 3 */
    IR_NEC_STAGE_C_COMM4,   /* 29: Complement Command Bit 4 */
    IR_NEC_STAGE_C_COMM5,   /* 30: Complement Command Bit 5 */
    IR_NEC_STAGE_C_COMM6,   /* 31: Complement Command Bit 6 */
    IR_NEC_STAGE_C_COMM7,   /* 32: Complement Command Bit 7 */
    IR_NEC_STAGE_FINISH     /* 33: */
};

enum
{
    IR_RCA_STAGE_LP,        /* 0: Leader Pulse */
    IR_RCA_STAGE_ADDR0,     /* 1: Address 0 */
    IR_RCA_STAGE_ADDR1,     /* 2: Address 1 */
    IR_RCA_STAGE_ADDR2,     /* 3: Address 2 */
    IR_RCA_STAGE_ADDR3,     /* 4: Address 3 */
    IR_RCA_STAGE_ADDR4,     /* 5: Address 4 */
    IR_RCA_STAGE_ADDR5,     /* 6: Address 5 */
    IR_RCA_STAGE_ADDR6,     /* 7: Address 6 */
    IR_RCA_STAGE_ADDR7,     /* 8: Address 7 */
    IR_RCA_STAGE_C_ADDR0,   /* 9:  Complement Address 0 */
    IR_RCA_STAGE_C_ADDR1,   /* 10: Complement Address 1 */
    IR_RCA_STAGE_C_ADDR2,   /* 11: Complement Address 2 */
    IR_RCA_STAGE_C_ADDR3,   /* 12: Complement Address 3 */
    IR_RCA_STAGE_C_ADDR4,   /* 13: Complement Address 4 */
    IR_RCA_STAGE_C_ADDR5,   /* 14: Complement Address 5 */
    IR_RCA_STAGE_C_ADDR6,   /* 15: Complement Address 6 */
    IR_RCA_STAGE_C_ADDR7,   /* 16: Complement Address 7 */
    IR_RCA_STAGE_COMM0,     /* 17: Command Bit 0 */
    IR_RCA_STAGE_COMM1,     /* 18: Command Bit 1 */
    IR_RCA_STAGE_COMM2,     /* 19: Command Bit 2 */
    IR_RCA_STAGE_COMM3,     /* 20: Command Bit 3 */
    IR_RCA_STAGE_COMM4,     /* 21: Command Bit 4 */
    IR_RCA_STAGE_COMM5,     /* 22: Command Bit 5 */
    IR_RCA_STAGE_COMM6,     /* 23: Command Bit 6 */
    IR_RCA_STAGE_COMM7,     /* 24: Command Bit 7 */
    IR_RCA_STAGE_FINISH     /* 25: */
};

enum
{
    IR_SONY_STAGE_LP,        /* 0: Leader Pulse */
    IR_SONY_STAGE_COMM0,     /* 1: Command Bit 0 */
    IR_SONY_STAGE_COMM1,     /* 2: Command Bit 1 */
    IR_SONY_STAGE_COMM2,     /* 3: Command Bit 2 */
    IR_SONY_STAGE_COMM3,     /* 4: Command Bit 3 */
    IR_SONY_STAGE_COMM4,     /* 5: Command Bit 4 */
    IR_SONY_STAGE_COMM5,     /* 6: Command Bit 5 */
    IR_SONY_STAGE_COMM6,     /* 7: Command Bit 6 */
    IR_SONY_STAGE_ADDR0,     /* 8: Address 0 */
    IR_SONY_STAGE_ADDR1,     /* 9: Address 1 */
    IR_SONY_STAGE_ADDR2,     /* 10: Address 2 */
    IR_SONY_STAGE_ADDR3,     /* 11: Address 3 */
    IR_SONY_STAGE_ADDR4,     /* 12: Address 4 */
    IR_SONY_STAGE_FINISH     /* 13: */
};

enum
{
    IR_SHARP_STAGE_ADDR0,     	/* 0: Address 0 */
    IR_SHARP_STAGE_ADDR1,     	/* 1: Address 1 */
    IR_SHARP_STAGE_ADDR2,     	/* 2: Address 2 */
    IR_SHARP_STAGE_ADDR3,     	/* 3: Address 3 */
    IR_SHARP_STAGE_ADDR4,     	/* 4: Address 4 */
    IR_SHARP_STAGE_COM0,   		/* 5: Command Bit 0 */
    IR_SHARP_STAGE_COM1,   		/* 6: Command Bit 1 */
    IR_SHARP_STAGE_COM2,   		/* 7: Command Bit 2 */
    IR_SHARP_STAGE_COM3,   		/* 8: Command Bit 3 */
    IR_SHARP_STAGE_COM4,   		/* 9: Command Bit 4 */
    IR_SHARP_STAGE_COM5,   		/* 10: Command Bit 5 */
    IR_SHARP_STAGE_DEF0,   		/* 11: DEF Bit 0 */
    IR_SHARP_STAGE_DEF1,   		/* 12: DEF Bit 1 */
    IR_SHARP_STAGE_EXT0,   		/* 13: EXT Bit 0 */
    IR_SHARP_STAGE_EXT1,   		/* 14: EXT Bit 1 */
    IR_SHARP_STAGE_I_STP0,    	/* 15: Interval gap 0 */
    IR_SHARP_STAGE_ADDR0A,     	/* 16: Address 0 */
    IR_SHARP_STAGE_ADDR1A,     	/* 17: Address 1 */
    IR_SHARP_STAGE_ADDR2A,     	/* 18: Address 2 */
    IR_SHARP_STAGE_ADDR3A,     	/* 19: Address 3 */
    IR_SHARP_STAGE_ADDR4A,     	/* 20: Address 4 */
    IR_SHARP_STAGE_C_COMM0,   	/* 21: Complement Command Bit 0 */
    IR_SHARP_STAGE_C_COMM1,   	/* 22: Complement Command Bit 1 */
    IR_SHARP_STAGE_C_COMM2,   	/* 23: Complement Command Bit 2 */
    IR_SHARP_STAGE_C_COMM3,   	/* 24: Complement Command Bit 3 */
    IR_SHARP_STAGE_C_COMM4,   	/* 25: Complement Command Bit 4 */
    IR_SHARP_STAGE_C_COMM5,   	/* 26: Complement Command Bit 5 */
    IR_SHARP_STAGE_C_DEF0,   	/* 27: DEF Bit 0 */
    IR_SHARP_STAGE_C_DEF1,   	/* 28: DEF Bit 1 */
    IR_SHARP_STAGE_C_EXT0,   	/* 29: EXT Bit 0 */
    IR_SHARP_STAGE_C_EXT1,   	/* 30: EXT Bit 1 */
    IR_SHARP_STAGE_FINISH     	/* 31: */
};

enum
{
    IR_MAT_STAGE_LP,        /* 0: Leader Pulse */
    IR_MAT_STAGE_ADDR0,     /* 1: Address 0 */
    IR_MAT_STAGE_ADDR1,     /* 2: Address 1 */
    IR_MAT_STAGE_ADDR2,     /* 3: Address 2 */
    IR_MAT_STAGE_ADDR3,     /* 4: Address 3 */
    IR_MAT_STAGE_ADDR4,     /* 5: Address 4 */
    IR_MAT_STAGE_ADDR5,     /* 6: Address 5 */
    IR_MAT_STAGE_COMM0,     /* 7: Command Bit 0 */
    IR_MAT_STAGE_COMM1,     /* 8: Command Bit 1 */
    IR_MAT_STAGE_COMM2,     /* 9: Command Bit 2 */
    IR_MAT_STAGE_COMM3,     /* 10: Command Bit 3 */
    IR_MAT_STAGE_COMM4,     /* 11: Command Bit 4 */
    IR_MAT_STAGE_COMM5,     /* 12: Command Bit 5 */
    IR_MAT_STAGE_C_ADDR0,   /* 13:  Complement Address 0 */
    IR_MAT_STAGE_C_ADDR1,   /* 14: Complement Address 1 */
    IR_MAT_STAGE_C_ADDR2,   /* 15: Complement Address 2 */
    IR_MAT_STAGE_C_ADDR3,   /* 16: Complement Address 3 */
    IR_MAT_STAGE_C_ADDR4,   /* 17: Complement Address 4 */
    IR_MAT_STAGE_C_ADDR5,   /* 18: Complement Address 5 */
    IR_MAT_STAGE_C_COMM0,   /* 19: Complement Command Bit 0 */
    IR_MAT_STAGE_C_COMM1,   /* 20: Complement Command Bit 1 */
    IR_MAT_STAGE_C_COMM2,   /* 21: Complement  Command Bit 2 */
    IR_MAT_STAGE_C_COMM3,   /* 22: Complement Command Bit 3 */
    IR_MAT_STAGE_C_COMM4,   /* 23: Complement Command Bit 4 */
    IR_MAT_STAGE_C_COMM5,   /* 24: Complement Command Bit 5 */
    IR_MAT_STAGE_FINISH     /* 25: */
};

enum
{
    IR_PANA_STAGE_LP,       /* 0: Leader Pulse */
    IR_PANA_STAGE_MC0,     	/* 1: Manufacturer Code 0 */
    IR_PANA_STAGE_MC1,     	/* 2: Manufacturer Code 1 */
    IR_PANA_STAGE_MC2,     	/* 3: Manufacturer Code 2 */
    IR_PANA_STAGE_MC3,     	/* 4: Manufacturer Code 3 */
    IR_PANA_STAGE_MC4,     	/* 5: Manufacturer Code 4 */
    IR_PANA_STAGE_MC5,     	/* 6: Manufacturer Code 5 */
    IR_PANA_STAGE_MC6,     	/* 7: Manufacturer Code 6 */
    IR_PANA_STAGE_MC7,     	/* 8: Manufacturer Code 7 */
    IR_PANA_STAGE_MC8,     	/* 9: Manufacturer Code 8 */
    IR_PANA_STAGE_MC9,     	/* 10: Manufacturer Code 9 */
    IR_PANA_STAGE_MCA,     	/* 11: Manufacturer Code A */
    IR_PANA_STAGE_MCB,     	/* 12: Manufacturer Code B */
    IR_PANA_STAGE_MCC,     	/* 13: Manufacturer Code C */
    IR_PANA_STAGE_MCD,     	/* 14: Manufacturer Code D */
    IR_PANA_STAGE_MCE,     	/* 15: Manufacturer Code E */
    IR_PANA_STAGE_MCF,     	/* 16: Manufacturer Code F */
    IR_PANA_STAGE_MCP0,    	/* 17: Manufacturer Code Parity 0 */
    IR_PANA_STAGE_MCP1,    	/* 18: Manufacturer Code Parity 1 */
    IR_PANA_STAGE_MCP2,    	/* 19: Manufacturer Code Parity 2 */
    IR_PANA_STAGE_MCP3,    	/* 20: Manufacturer Code Parity 3 */
    IR_PANA_STAGE_SYS0,    	/* 21: System Bit 0 */
    IR_PANA_STAGE_SYS1,    	/* 22: System Bit 1 */
    IR_PANA_STAGE_SYS2,    	/* 23: System Bit 2 */
    IR_PANA_STAGE_SYS3,    	/* 24: System Bit 3 */
    IR_PANA_STAGE_EQUIP0,    /* 25: Equipment Bit 0 */
    IR_PANA_STAGE_EQUIP1,    /* 26: Equipment Bit 1 */
    IR_PANA_STAGE_EQUIP2,    /* 27: Equipment Bit 2 */
    IR_PANA_STAGE_EQUIP3,    /* 28: Equipment Bit 3 */
    IR_PANA_STAGE_EQUIP4,    /* 29: Equipment Bit 4 */
    IR_PANA_STAGE_EQUIP5,    /* 30: Equipment Bit 5 */
    IR_PANA_STAGE_EQUIP6,    /* 31: Equipment Bit 6 */
    IR_PANA_STAGE_EQUIP7,    /* 32: Equipment Bit 7 */
    IR_PANA_STAGE_COMM0,     /* 33: Command Bit 0 */
    IR_PANA_STAGE_COMM1,     /* 34: Command Bit 1 */
    IR_PANA_STAGE_COMM2,     /* 35: Command Bit 2 */
    IR_PANA_STAGE_COMM3,     /* 36: Command Bit 3 */
    IR_PANA_STAGE_COMM4,     /* 37: Command Bit 4 */
    IR_PANA_STAGE_COMM5,     /* 38: Command Bit 5 */
    IR_PANA_STAGE_COMM6,     /* 39: Command Bit 6 */
    IR_PANA_STAGE_COMM7,     /* 40: Command Bit 7 */
    IR_PANA_STAGE_PARITY0,    /* 41: Parity 0 */
    IR_PANA_STAGE_PARITY1,    /* 42: Parity 1 */
    IR_PANA_STAGE_PARITY2,    /* 43: Parity 2 */
    IR_PANA_STAGE_PARITY3,    /* 44: Parity 3 */
    IR_PANA_STAGE_PARITY4,    /* 45: Parity 4 */
    IR_PANA_STAGE_PARITY5,    /* 46: Parity 5 */
    IR_PANA_STAGE_PARITY6,    /* 47: Parity 6 */
    IR_PANA_STAGE_PARITY7,    /* 48: Parity 7 */
    IR_PANA_STAGE_FINISH     /* 49: */
};

enum
{
    IR_RCMM_STAGE_LP,       /* 0: Leader Pulse */
    IR_RCMM_STAGE_MC0,     	/* 1: Manufacturer Code 0 */
    IR_RCMM_STAGE_MC1,     	/* 2: Manufacturer Code 1 */
    IR_RCMM_STAGE_MC2,     	/* 3: Manufacturer Code 2 */
    IR_RCMM_STAGE_MC3,     	/* 4: Manufacturer Code 3 */
    IR_RCMM_STAGE_MC4,     	/* 5: Manufacturer Code 4 */
    IR_RCMM_STAGE_MC5,     	/* 6: Manufacturer Code 5 */
    IR_RCMM_STAGE_MC6,     	/* 7: Manufacturer Code 6 */
    IR_RCMM_STAGE_MC7,     	/* 8: Manufacturer Code 7 */
    IR_RCMM_STAGE_MC8,     	/* 9: Manufacturer Code 8 */
    IR_RCMM_STAGE_MC9,     	/* 10: Manufacturer Code 9 */
    IR_RCMM_STAGE_MCA,     	/* 11: Manufacturer Code 10 */
    IR_RCMM_STAGE_MCB,     	/* 12: Manufacturer Code 11 */
    IR_RCMM_STAGE_MCC,     	/* 13: Manufacturer Code 12 */
    IR_RCMM_STAGE_MCD,     	/* 14: Manufacturer Code 13 */
    IR_RCMM_STAGE_MCE,     	/* 15: Manufacturer Code 14 */
    IR_RCMM_STAGE_MCF,     	/* 16: Manufacturer Code 15 */
    IR_RCMM_STAGE_FINISH     /* 17: */
};

/* variables */
GLOBAL xdata sNVRAM_IR_INFO sIR_RAM_TBL[IR_RAM_TBL_MAX];
GLOBAL xdata sIR_STATUS_INFO sIR_STATUS; 
GLOBAL xdata sRC_TIME_TABLE sRC_TIMETABLE[IR_TBL_MAX];
GLOBAL Byte xdata  *bIndexRC;

GLOBAL Byte data bIrTimeIndexIn;
GLOBAL Byte data bIrTimeIndexOut;
GLOBAL xdata sRC_TIME_TABLE sRC_TIMETABLE_Output[IR_TBL_MAX];
GLOBAL Byte data bIrTimeIndexIn_Output;
GLOBAL Byte data bIrTimeIndexOut_Output;

GLOBAL Byte xdata bRCType;
GLOBAL Word xdata wTimeBase;
GLOBAL signed char xdata scTdiffer;
GLOBAL signed char xdata scCarrierdif;
GLOBAL Byte xdata bKEY_repeat_times;

GLOBAL xdata st_IR   tg_ir_rc5x_data;
GLOBAL xdata st_IR   tg_ir_rc6_data;
GLOBAL xdata st_IR   tg_ir_NEC_data;
GLOBAL xdata st_IR   tg_ir_SONY_data;
GLOBAL xdata st_IR   tg_ir_SHARP_data;
GLOBAL xdata st_IR   tg_ir_MAT_data;
GLOBAL xdata st_IR   tg_ir_PANA_data;
GLOBAL xdata st_IR   tg_ir_RCMM_data;
GLOBAL xdata st_IR   tg_ir_RCA_data;
GLOBAL Byte  ClearIrDecDataFlag;
GLOBAL Byte  ClearIrDAFlag;
GLOBAL Byte  bTail;
GLOBAL Word TimeUnit;
GLOBAL uint32_t 	Old_Longdata;
GLOBAL Byte RCMM_Key;


/* functions */
#ifdef _PORTING_ARM_
GLOBAL void IR_Carrier(uint32_t ulFre);
#else
GLOBAL void IR_Carrier(unsigned long int ulFre);
#endif // #ifdef _PORTING_ARM_
GLOBAL void IR_TimeTBL (signed char KeyIndex, Byte FirstKey);
GLOBAL void IR_TimeLevel( unsigned long int wTime, Byte bLevel);

GLOBAL void IrCodeCodeMix_RC5(Byte);
GLOBAL void IrCodeCodeMix_RC6(Byte);
GLOBAL void IrCodeCodeMix_NEC(Byte);
GLOBAL void IrCodeCodeMix_SONY(Byte);
GLOBAL void IrCodeCodeMix_SHARP(Byte);
GLOBAL void IrCodeCodeMix_MAT(Byte);
GLOBAL void IrCodeCodeMix_PANA(Byte);
GLOBAL void IrCodeCodeMix_RCMM(Byte);
GLOBAL void IrCodeCodeMix_RCA(Byte);

GLOBAL void IR_TimeTBL_RC5 (void);
GLOBAL void IR_TimeTBL_RC6 (void);
GLOBAL void IR_TimeTBL_NEC (void);
GLOBAL void IR_TimeTBL_NECRepeat (void);
GLOBAL void IR_TimeTBL_SONY (void);
GLOBAL void IR_TimeTBL_SHARP (void);
GLOBAL void IR_TimeTBL_MAT (void);
GLOBAL void IR_TimeTBL_PANA (void);
GLOBAL void IR_TimeTBL_RCMM (void);
GLOBAL void IR_TimeTBL_RCA (void);

GLOBAL void IrCodeDefault(void);
GLOBAL void IrRamInitial(void);
GLOBAL void IR_CarrierBase(void);
GLOBAL void IrTimerBase(void);
GLOBAL signed char Tdiff(void);
GLOBAL signed char CarrierDif(void);
GLOBAL Word wLastTime;

GLOBAL void Initial_ir_dec (void);
GLOBAL void Reset_ir_dec (void);
GLOBAL BOOL Get_ir_dec_rc6_code (WORD *p_ui2_mode_addr, WORD *p_ui2_ctrl_info);
GLOBAL BOOL Get_ir_dec_rc5x_code (WORD *p_ui2_mode_addr, WORD *p_ui2_ctrl_info);
GLOBAL BOOL Get_ir_dec_NEC_code (WORD *p_ui2_mode_addr, WORD *p_ui2_ctrl_info);
GLOBAL BOOL Get_ir_dec_SONY_code (WORD *p_ui2_mode_addr, WORD *p_ui2_ctrl_info);
GLOBAL BOOL Get_ir_dec_SHARP_code (WORD *p_ui2_mode_addr, WORD *p_ui2_ctrl_info);
GLOBAL BOOL Get_ir_dec_Matsushita_code (WORD *p_ui2_mode_addr, WORD *p_ui2_ctrl_info);
GLOBAL BOOL Get_ir_dec_PANA_code (WORD *p_ui2_mode_addr, WORD *p_ui2_ctrl_info);
GLOBAL BOOL Get_ir_dec_RCMM_code (WORD *p_ui2_mode_addr, WORD *p_ui2_ctrl_info);
GLOBAL BOOL Get_ir_dec_RCA_code (WORD *p_ui2_mode_addr, WORD *p_ui2_ctrl_info);
GLOBAL BOOL Ir_dec_rc6_process (BYTE bLevel, WORD wTimeStamp);
GLOBAL BOOL Ir_dec_rc5x_process (BYTE bLevel, WORD wTimeStamp);
GLOBAL BOOL Ir_dec_NEC_process (BYTE bLevel, WORD wTimeStamp);
GLOBAL BOOL Ir_dec_SONY_process (BYTE bLevel, WORD wTimeStamp);
GLOBAL BOOL Ir_dec_SHARP_process (BYTE bLevel, WORD wTimeStamp);
GLOBAL BOOL Ir_dec_Matsushita_process (BYTE bLevel, WORD wTimeStamp);
GLOBAL BOOL Ir_dec_PANA_process (BYTE bLevel, WORD wTimeStamp);
GLOBAL BOOL Ir_dec_RCMM_process (BYTE bLevel, WORD wTimeStamp);
GLOBAL BOOL Ir_dec_RCA_process (BYTE bLevel, WORD wTimeStamp);
//GLOBAL void ReplayWave(void);
GLOBAL void ScanIR(Byte bLevel, Word wTimeStamp );
GLOBAL void ShowIRCode(char *IR_Code, WORD mode_addr, WORD ctrl_info);
GLOBAL void ShowIRFrq(WORD IR_frq);

GLOBAL int abs(int value);

#endif
