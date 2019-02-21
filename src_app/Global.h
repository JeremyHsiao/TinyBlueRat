
#ifndef _GLOBAL_H
#define _GLOBAL_H


#define   FALSE   0
#define   TRUE    1

#define RX_BUFFER_X_MAX					10
#define RX_BUFFER_Y_MAX					8
#define TX_BUFFER_X_MAX					10
#define TX_BUFFER_Y_MAX					8

GLOBAL Byte XDATA MessageCounter;
GLOBAL Byte XDATA RCRepeatCounter;
GLOBAL WORD XDATA ReceiverMessageData;
GLOBAL Word XDATA RCKeyCode;
GLOBAL WORD XDATA PreviousRCKey;
GLOBAL Bit  fManchesterCoded;
GLOBAL Bit  fIR_Dec_Mode;
GLOBAL BYTE  b_decoding;

GLOBAL WORD  data  KeyScanTimer;

GLOBAL Word wdt_counter;
//DATA
GLOBAL Byte bKeyCodeRepeatCounter;

//Char XDATA
GLOBAL char XDATA TurboRotary;

//Byte XDATA
GLOBAL Word XDATA SystemTimer10ms;
GLOBAL Byte XDATA SystemTimer10msCnt;
GLOBAL Byte XDATA SystemTimer10msCnt1;
GLOBAL Byte XDATA SystemTimer100msCnt;
GLOBAL Byte XDATA IRResetTimerCnt;
GLOBAL Word XDATA IRDataResetTimerCnt;
GLOBAL Word XDATA FreqAverage;

GLOBAL Word wTimer10ms;

GLOBAL Byte XDATA IRResetTimerCnt;
GLOBAL Word XDATA IRDataResetTimerCnt;
GLOBAL Word XDATA FreqAverage;

//GLOBAL Byte BaudRate9600;
//GLOBAL Byte fBaudRate9600_flag;

#endif  //_GLOBAL_H
