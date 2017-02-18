#define _BSD_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "si3210.h"
#include "config.h"

//static char * project = "si3210";

slic_dev_str	slic_dev;

void mvOsDelay(int delay_val) {
	usleep(1000*delay_val);
}

void version(slic_dev_str *slic_dev) {
	slic_dev->version=(0xf & readDirectReg(slic_dev, 0));
}

void device_identification(slic_dev_str *slic_dev) {
	if ((readDirectReg(slic_dev, 1) & 0x80)==0x80) {
		slic_dev->si_model=si3215;	
		if ((0x30 & readDirectReg(slic_dev, 0)) == 0x30)
			slic_dev->si_model=si3215M;
	} else {	
		slic_dev->si_model=si3210;
		if ((0x30 & readDirectReg(slic_dev, 0)) == 0x10)
			slic_dev->si_model=si3211;
		if ((0x30 & readDirectReg(slic_dev, 0)) == 0x30)
			slic_dev->si_model=si3210M;
	}
}

int verifyIndirectReg(slic_dev_str *slic_dev, unsigned char address, unsigned short should_be_value) { 
	int error_flag ;
	unsigned short value;
		value = readIndirectReg(slic_dev, address);
		error_flag = (should_be_value != value);
		
		if ( error_flag )
		{
			printf("\n   iREG %d = %X  should be %X ",address,value,should_be_value );			
		}	
		return error_flag;
}

void initializeIndirectRegisters(slic_dev_str *slic_dev)	{	
	if (slic_dev->si_model!=si3215) {
	writeIndirectReg(slic_dev, 	0	,	INIT_IR0		);	//	0x55C2	DTMF_ROW_0_PEAK
	writeIndirectReg(slic_dev, 	1	,	INIT_IR1		);	//	0x51E6	DTMF_ROW_1_PEAK
	writeIndirectReg(slic_dev, 	2	,	INIT_IR2		);	//	0x4B85	DTMF_ROW2_PEAK
	writeIndirectReg(slic_dev, 	3	,	INIT_IR3		);	//	0x4937	DTMF_ROW3_PEAK
	writeIndirectReg(slic_dev, 	4	,	INIT_IR4		);	//	0x3333	DTMF_COL1_PEAK
	writeIndirectReg(slic_dev, 	5	,	INIT_IR5		);	//	0x0202	DTMF_FWD_TWIST
	writeIndirectReg(slic_dev, 	6	,	INIT_IR6		);	//	0x0202	DTMF_RVS_TWIST
	writeIndirectReg(slic_dev, 	7	,	INIT_IR7		);	//	0x0198	DTMF_ROW_RATIO
	writeIndirectReg(slic_dev, 	8	,	INIT_IR8		);	//	0x0198	DTMF_COL_RATIO
	writeIndirectReg(slic_dev, 	9	,	INIT_IR9		);	//	0x0611	DTMF_ROW_2ND_ARM
	writeIndirectReg(slic_dev, 	10	,	INIT_IR10		);	//	0x0202	DTMF_COL_2ND_ARM
	writeIndirectReg(slic_dev, 	11	,	INIT_IR11		);	//	0x00E5	DTMF_PWR_MIN_
	writeIndirectReg(slic_dev, 	12	,	INIT_IR12		);	//	0x0A1C	DTMF_OT_LIM_TRES
	}									
	writeIndirectReg(slic_dev, 	13	,	INIT_IR13		);	//	0x7b30	OSC1_COEF
	writeIndirectReg(slic_dev, 	14	,	INIT_IR14		);	//	0x0063	OSC1X
	writeIndirectReg(slic_dev, 	15	,	INIT_IR15		);	//	0x0000	OSC1Y
	writeIndirectReg(slic_dev, 	16	,	INIT_IR16		);	//	0x7870	OSC2_COEF
	writeIndirectReg(slic_dev, 	17	,	INIT_IR17		);	//	0x007d	OSC2X
	writeIndirectReg(slic_dev, 	18	,	INIT_IR18		);	//	0x0000	OSC2Y
	writeIndirectReg(slic_dev, 	19	,	INIT_IR19		);	//	0x0000	RING_V_OFF
	writeIndirectReg(slic_dev, 	20	,	INIT_IR20		);	//	0x7EF0	RING_OSC
	writeIndirectReg(slic_dev, 	21	,	INIT_IR21		);	//	0x0160	RING_X
	writeIndirectReg(slic_dev, 	22	,	INIT_IR22		);	//	0x0000	RING_Y
	writeIndirectReg(slic_dev, 	23	,	INIT_IR23		);	//	0x2000	PULSE_ENVEL
	writeIndirectReg(slic_dev, 	24	,	INIT_IR24		);	//	0x2000	PULSE_X
	writeIndirectReg(slic_dev, 	25	,	INIT_IR25		);	//	0x0000	PULSE_Y
	writeIndirectReg(slic_dev, 	26	,	INIT_IR26		);	//	0x4000	RECV_DIGITAL_GAIN
	writeIndirectReg(slic_dev, 	27	,	INIT_IR27		);	//	0x4000	XMIT_DIGITAL_GAIN
	writeIndirectReg(slic_dev, 	28	,	INIT_IR28		);	//	0x1000	LOOP_CLOSE_TRES
	writeIndirectReg(slic_dev, 	29	,	INIT_IR29		);	//	0x3600	RING_TRIP_TRES
	writeIndirectReg(slic_dev, 	30	,	INIT_IR30		);	//	0x1000	COMMON_MIN_TRES
	writeIndirectReg(slic_dev, 	31	,	INIT_IR31		);	//	0x0200	COMMON_MAX_TRES
	writeIndirectReg(slic_dev, 	32	,	INIT_IR32		);	//	0x7c0  	PWR_ALARM_Q1Q2
	writeIndirectReg(slic_dev, 	33	,	INIT_IR33		);	//	0x2600	PWR_ALARM_Q3Q4
	writeIndirectReg(slic_dev, 	34	,	INIT_IR34		);	//	0x1B80	PWR_ALARM_Q5Q6
	writeIndirectReg(slic_dev, 	35	,	INIT_IR35		);	//	0x8000	LOOP_CLSRE_FlTER
	writeIndirectReg(slic_dev, 	36	,	INIT_IR36		);	//	0x0320	RING_TRIP_FILTER
	writeIndirectReg(slic_dev, 	37	,	INIT_IR37		);	//	0x08c	TERM_LP_POLE_Q1Q2
	writeIndirectReg(slic_dev, 	38	,	INIT_IR38		);	//	0x0100	TERM_LP_POLE_Q3Q4
	writeIndirectReg(slic_dev, 	39	,	INIT_IR39		);	//	0x0010	TERM_LP_POLE_Q5Q6
	writeIndirectReg(slic_dev, 	40	,	INIT_IR40		);	//	0x0C00	CM_BIAS_RINGING
	writeIndirectReg(slic_dev, 	41	,	INIT_IR41		);	//	0x0C00	DCDC_MIN_V
	writeIndirectReg(slic_dev, 	43	,	INIT_IR43		);	//	0x1000	LOOP_CLOSE_TRES Low
	writeIndirectReg(slic_dev, 	99	,	INIT_IR99		);	//	0x00DA	FSK 0 FREQ PARAM
	writeIndirectReg(slic_dev, 	100	,	INIT_IR100		);	//	0x6B60	FSK 0 AMPL PARAM
	writeIndirectReg(slic_dev, 	101	,	INIT_IR101		);	//	0x0074	FSK 1 FREQ PARAM
	writeIndirectReg(slic_dev, 	102	,	INIT_IR102		);	//	0x79C0	FSK 1 AMPl PARAM
	writeIndirectReg(slic_dev, 	103	,	INIT_IR103		);	//	0x1120	FSK 0to1 SCALER
	writeIndirectReg(slic_dev, 	104	,	INIT_IR104		);	//	0x3BE0	FSK 1to0 SCALER
	writeIndirectReg(slic_dev, 	97	,	INIT_IR97		);	//	0x0000	TRASMIT_FILTER
}	

int verifyIndirectRegisters(slic_dev_str *slic_dev) {		
	int error=0;
	
	if (slic_dev->si_model!=si3215) {
	error |= verifyIndirectReg(slic_dev   ,    	0	,	INIT_IR0		);	//	0x55C2	DTMF_ROW_0_PEAK
	error |= verifyIndirectReg(slic_dev   ,    	1	,	INIT_IR1		);	//	0x51E6	DTMF_ROW_1_PEAK
	error |= verifyIndirectReg(slic_dev   ,    	2	,	INIT_IR2		);	//	0x4B85	DTMF_ROW2_PEAK
	error |= verifyIndirectReg(slic_dev   ,    	3	,	INIT_IR3		);	//	0x4937	DTMF_ROW3_PEAK
	error |= verifyIndirectReg(slic_dev   ,    	4	,	INIT_IR4		);	//	0x3333	DTMF_COL1_PEAK
	error |= verifyIndirectReg(slic_dev   ,    	5	,	INIT_IR5		);	//	0x0202	DTMF_FWD_TWIST
	error |= verifyIndirectReg(slic_dev   ,    	6	,	INIT_IR6		);	//	0x0202	DTMF_RVS_TWIST
	error |= verifyIndirectReg(slic_dev   ,    	7	,	INIT_IR7		);	//	0x0198	DTMF_ROW_RATIO
	error |= verifyIndirectReg(slic_dev   ,    	8	,	INIT_IR8		);	//	0x0198	DTMF_COL_RATIO
	error |= verifyIndirectReg(slic_dev   ,    	9	,	INIT_IR9		);	//	0x0611	DTMF_ROW_2ND_ARM
	error |= verifyIndirectReg(slic_dev   ,    	10	,	INIT_IR10		);	//	0x0202	DTMF_COL_2ND_ARM
	error |= verifyIndirectReg(slic_dev   ,    	11	,	INIT_IR11		);	//	0x00E5	DTMF_PWR_MIN_
	error |= verifyIndirectReg(slic_dev   ,    	12	,	INIT_IR12		);	//	0x0A1C	DTMF_OT_LIM_TRES
	}
	error |= verifyIndirectReg(slic_dev   ,    	13	,	INIT_IR13		);	//	0x7b30	OSC1_COEF
	error |= verifyIndirectReg(slic_dev   ,    	14	,	INIT_IR14		);	//	0x0063	OSC1X
	error |= verifyIndirectReg(slic_dev   ,    	15	,	INIT_IR15		);	//	0x0000	OSC1Y
	error |= verifyIndirectReg(slic_dev   ,    	16	,	INIT_IR16		);	//	0x7870	OSC2_COEF
	error |= verifyIndirectReg(slic_dev   ,    	17	,	INIT_IR17		);	//	0x007d	OSC2X
	error |= verifyIndirectReg(slic_dev   ,    	18	,	INIT_IR18		);	//	0x0000	OSC2Y
	error |= verifyIndirectReg(slic_dev   ,    	19	,	INIT_IR19		);	//	0x0000	RING_V_OFF
	error |= verifyIndirectReg(slic_dev   ,    	20	,	INIT_IR20		);	//	0x7EF0	RING_OSC
	error |= verifyIndirectReg(slic_dev   ,    	21	,	INIT_IR21		);	//	0x0160	RING_X
	error |= verifyIndirectReg(slic_dev   ,    	22	,	INIT_IR22		);	//	0x0000	RING_Y
	error |= verifyIndirectReg(slic_dev   ,    	23	,	INIT_IR23		);	//	0x2000	PULSE_ENVEL
	error |= verifyIndirectReg(slic_dev   ,    	24	,	INIT_IR24		);	//	0x2000	PULSE_X
	error |= verifyIndirectReg(slic_dev   ,    	25	,	INIT_IR25		);	//	0x0000	PULSE_Y
	error |= verifyIndirectReg(slic_dev   ,    	26	,	INIT_IR26		);	//	0x4000	RECV_DIGITAL_GAIN
	error |= verifyIndirectReg(slic_dev   ,    	27	,	INIT_IR27		);	//	0x4000	XMIT_DIGITAL_GAIN
	error |= verifyIndirectReg(slic_dev   ,    	28	,	INIT_IR28		);	//	0x1000	LOOP_CLOSE_TRES
	error |= verifyIndirectReg(slic_dev   ,    	29	,	INIT_IR29		);	//	0x3600	RING_TRIP_TRES
	error |= verifyIndirectReg(slic_dev   ,    	30	,	INIT_IR30		);	//	0x1000	COMMON_MIN_TRES
	error |= verifyIndirectReg(slic_dev   ,    	31	,	INIT_IR31		);	//	0x0200	COMMON_MAX_TRES
	error |= verifyIndirectReg(slic_dev   ,    	32	,	INIT_IR32		);	//	0x7c0  	PWR_ALARM_Q1Q2
	error |= verifyIndirectReg(slic_dev   ,    	33	,	INIT_IR33		);	//	0x2600	PWR_ALARM_Q3Q4
	error |= verifyIndirectReg(slic_dev   ,    	34	,	INIT_IR34		);	//	0x1B80	PWR_ALARM_Q5Q6
	error |= verifyIndirectReg(slic_dev   ,    	35	,	INIT_IR35		);	//	0x8000	LOOP_CLSRE_FlTER
	error |= verifyIndirectReg(slic_dev   ,    	36	,	INIT_IR36		);	//	0x0320	RING_TRIP_FILTER
	error |= verifyIndirectReg(slic_dev   ,    	37	,	INIT_IR37		);	//	0x08c	TERM_LP_POLE_Q1Q2
	error |= verifyIndirectReg(slic_dev   ,    	38	,	INIT_IR38		);	//	0x0100	TERM_LP_POLE_Q3Q4
	error |= verifyIndirectReg(slic_dev   ,    	39	,	INIT_IR39		);	//	0x0010	TERM_LP_POLE_Q5Q6
	error |= verifyIndirectReg(slic_dev   ,    	40	,	INIT_IR40		);	//	0x0C00	CM_BIAS_RINGING
	error |= verifyIndirectReg(slic_dev   ,    	41	,	INIT_IR41		);	//	0x0C00	DCDC_MIN_V
	error |= verifyIndirectReg(slic_dev   ,    	43	,	INIT_IR43		);	//	0x1000	LOOP_CLOSE_TRES Low
	error |= verifyIndirectReg(slic_dev   ,    	99	,	INIT_IR99		);	//	0x00DA	FSK 0 FREQ PARAM
	error |= verifyIndirectReg(slic_dev   ,    	100	,	INIT_IR100		);	//	0x6B60	FSK 0 AMPL PARAM
	error |= verifyIndirectReg(slic_dev   ,    	101	,	INIT_IR101		);	//	0x0074	FSK 1 FREQ PARAM
	error |= verifyIndirectReg(slic_dev   ,    	102	,	INIT_IR102		);	//	0x79C0	FSK 1 AMPl PARAM
	error |= verifyIndirectReg(slic_dev   ,    	103	,	INIT_IR103		);	//	0x1120	FSK 0to1 SCALER
	error |= verifyIndirectReg(slic_dev   ,    	104	,	INIT_IR104		);	//	0x3BE0	FSK 1to0 SCALER
	
	return error;
}

void initializeDirectRegisters(slic_dev_str *slic_dev) {

	writeDirectReg(slic_dev, 1,	INIT_DR1	);//0X08	PCM Mode - INIT TO disable
	writeDirectReg(slic_dev, 2,	INIT_DR2	);//0X00	PCM TX Clock Slot Low Byte (1 PCLK cycle/LSB)
	writeDirectReg(slic_dev, 3,	INIT_DR3	);//0x00	PCM TX Clock Slot High Byte
	writeDirectReg(slic_dev, 4,	INIT_DR4	);//0x00	PCM RX Clock Slot Low Byte (1 PCLK cycle/LSB)
	writeDirectReg(slic_dev, 5,	INIT_DR5	);//0x00	PCM RX Clock Slot High Byte
	writeDirectReg(slic_dev, 8,	INIT_DR8	);//0X00	Loopbacks (digital loopback default)
	writeDirectReg(slic_dev, 9,	INIT_DR9	);//0x00	Transmit and receive path gain and control
	writeDirectReg(slic_dev, 10,	INIT_DR10	);//0X28	Initialization Two-wire impedance (600  and enabled)
	writeDirectReg(slic_dev, 11,	INIT_DR11	);//0x33	Transhybrid Balance/Four-wire Return Loss
	writeDirectReg(slic_dev, 18,	INIT_DR18	);//0xff	Normal Oper. Interrupt Register 1 (clear with 0xFF)
	writeDirectReg(slic_dev, 19,	INIT_DR19	);//0xff	Normal Oper. Interrupt Register 2 (clear with 0xFF)
	writeDirectReg(slic_dev, 20,	INIT_DR20	);//0xff	Normal Oper. Interrupt Register 3 (clear with 0xFF)
	writeDirectReg(slic_dev, 21,	INIT_DR21	);//0xff	Interrupt Mask 1
	writeDirectReg(slic_dev, 22,	INIT_DR22	);//0xff	Initialization Interrupt Mask 2
	writeDirectReg(slic_dev, 23,	INIT_DR23	);//0xff	 Initialization Interrupt Mask 3
	writeDirectReg(slic_dev, 32,	INIT_DR32	);//0x00	Oper. Oscillator 1 Controltone generation
	writeDirectReg(slic_dev, 33,	INIT_DR33	);//0x00	Oper. Oscillator 2 Controltone generation
	writeDirectReg(slic_dev, 34,	INIT_DR34	);//0X18	34 0x22 0x00 Initialization Ringing Oscillator Control
	writeDirectReg(slic_dev, 35,	INIT_DR35	);//0x00	Oper. Pulse Metering Oscillator Control
	writeDirectReg(slic_dev, 36,	INIT_DR36	);//0x00	36 0x24 0x00 Initialization OSC1 Active Low Byte (125 µs/LSB)
	writeDirectReg(slic_dev, 37,	INIT_DR37	);//0x00	37 0x25 0x00 Initialization OSC1 Active High Byte (125 µs/LSB)
	writeDirectReg(slic_dev, 38,	INIT_DR38	);//0x00	38 0x26 0x00 Initialization OSC1 Inactive Low Byte (125 µs/LSB)
	writeDirectReg(slic_dev, 39,	INIT_DR39	);//0x00	39 0x27 0x00 Initialization OSC1 Inactive High Byte (125 µs/LSB)
	writeDirectReg(slic_dev, 40,	INIT_DR40	);//0x00	40 0x28 0x00 Initialization OSC2 Active Low Byte (125 µs/LSB)
	writeDirectReg(slic_dev, 41,	INIT_DR41	);//0x00	41 0x29 0x00 Initialization OSC2 Active High Byte (125 µs/LSB)
	writeDirectReg(slic_dev, 42,	INIT_DR42	);//0x00	42 0x2A 0x00 Initialization OSC2 Inactive Low Byte (125 µs/LSB)
	writeDirectReg(slic_dev, 43,	INIT_DR43	);//0x00	43 0x2B 0x00 Initialization OSC2 Inactive High Byte (125 µs/LSB)
	writeDirectReg(slic_dev, 44,	INIT_DR44	);//0x00	44 0x2C 0x00 Initialization Pulse Metering Active Low Byte (125 µs/LSB)
	writeDirectReg(slic_dev, 45,	INIT_DR45	);//0x00	45 0x2D 0x00 Initialization Pulse Metering Active High Byte (125 µs/LSB)
	writeDirectReg(slic_dev, 46,	INIT_DR46	);//0x00	46 0x2E 0x00 Initialization Pulse Metering Inactive Low Byte (125 µs/LSB)
	writeDirectReg(slic_dev, 47,	INIT_DR47	);//0x00	47 0x2F 0x00 Initialization Pulse Metering Inactive High Byte (125 µs/LSB)
	writeDirectReg(slic_dev, 48,	INIT_DR48	);//0X80	48 0x30 0x00 0x80 Initialization Ringing Osc. Active Timer Low Byte (2 s,125 µs/LSB)
	writeDirectReg(slic_dev, 49,	INIT_DR49	);//0X3E	49 0x31 0x00 0x3E Initialization Ringing Osc. Active Timer High Byte (2 s,125 µs/LSB)
	writeDirectReg(slic_dev, 50,	INIT_DR50	);//0X00	50 0x32 0x00 0x00 Initialization Ringing Osc. Inactive Timer Low Byte (4 s, 125 µs/LSB)
	writeDirectReg(slic_dev, 51,	INIT_DR51	);//0X7D	51 0x33 0x00 0x7D Initialization Ringing Osc. Inactive Timer High Byte (4 s, 125 µs/LSB)
	writeDirectReg(slic_dev, 52,	INIT_DR52	);//0X00	52 0x34 0x00 Normal Oper. FSK Data Bit
	writeDirectReg(slic_dev, 63,	INIT_DR63	);//0X54	63 0x3F 0x54 Initialization Ringing Mode Loop Closure Debounce Interval
	writeDirectReg(slic_dev, 64,	INIT_DR64	);//0x00	64 0x40 0x00 Normal Oper. Mode Byte—primary control
	writeDirectReg(slic_dev, 65,	INIT_DR65	);//0X61	65 0x41 0x61 Initialization External Bipolar Transistor Settings
	writeDirectReg(slic_dev, 66,	INIT_DR66	);//0X03	66 0x42 0x03 Initialization Battery Control
	writeDirectReg(slic_dev, 67,	INIT_DR67	);//0X1F	67 0x43 0x1F Initialization Automatic/Manual Control
	writeDirectReg(slic_dev, 69,	INIT_DR69	);//0X0C	69 0x45 0x0A 0x0C Initialization Loop Closure Debounce Interval (1.25 ms/LSB)
	writeDirectReg(slic_dev, 70,	INIT_DR70	);//0X0A	70 0x46 0x0A Initialization Ring Trip Debounce Interval (1.25 ms/LSB)
	writeDirectReg(slic_dev, 71,	INIT_DR71	);//0X01	71 0x47 0x00 0x01 Initialization Off-Hook Loop Current Limit (20 mA + 3 mA/LSB)
	writeDirectReg(slic_dev, 72,	INIT_DR72	);//0X20	72 0x48 0x20 Initialization On-Hook Voltage (open circuit voltage) = 48 V(1.5 V/LSB)
	writeDirectReg(slic_dev, 73,	INIT_DR73	);//0X02	73 0x49 0x02 Initialization Common Mode VoltageVCM = 3 V(1.5 V/LSB)
	writeDirectReg(slic_dev, 74,	INIT_DR74	);//0X32	74 0x4A 0x32 Initialization VBATH (ringing) = 75 V (1.5 V/LSB)
	writeDirectReg(slic_dev, 75,	INIT_DR75	);//0X10	75 0x4B 0x10 Initialization VBATL (off-hook) = 24 V (TRACK = 0)(1.5 V/LSB)
	if (slic_dev->si_model==si3210M)
		writeDirectReg(slic_dev, 92,	INIT_SI3210M_DR92	);//0x7f	92 0x5C 0xFF 7F Initialization DCDC Converter PWM Period (61.035 ns/LSB)
	else
		writeDirectReg(slic_dev, 92,	INIT_DR92	);//0x7f	92 0x5C 0xFF 7F Initialization DCDC Converter PWM Period (61.035 ns/LSB)

	writeDirectReg(slic_dev, 93,	INIT_DR93	);//0x14	93 0x5D 0x14 0x19 Initialization DCDC Converter Min. Off Time (61.035 ns/LSB)
	writeDirectReg(slic_dev, 96,	INIT_DR96	);//0x00	96 0x60 0x1F Initialization Calibration Control Register 1(written second and starts calibration)
	writeDirectReg(slic_dev, 97,	INIT_DR97	);//0X1F	97 0x61 0x1F Initialization Calibration Control Register 2(written before Register 96)
	writeDirectReg(slic_dev, 98,	INIT_DR98	);//0X10	98 0x62 0x10 Informative Calibration result (see data sheet)
	writeDirectReg(slic_dev, 99,	INIT_DR99	);//0X10	99 0x63 0x10 Informative Calibration result (see data sheet)
	writeDirectReg(slic_dev, 100,	INIT_DR100	);//0X11	100 0x64 0x11 Informative Calibration result (see data sheet)
	writeDirectReg(slic_dev, 101,	INIT_DR101	);//0X11	101 0x65 0x11 Informative Calibration result (see data sheet)
	writeDirectReg(slic_dev, 102,	INIT_DR102	);//0x08	102 0x66 0x08 Informative Calibration result (see data sheet)
	writeDirectReg(slic_dev, 103,	INIT_DR103	);//0x88	103 0x67 0x88 Informative Calibration result (see data sheet)
	writeDirectReg(slic_dev, 104,	INIT_DR104	);//0x00	104 0x68 0x00 Informative Calibration result (see data sheet)
	writeDirectReg(slic_dev, 105,	INIT_DR105	);//0x00	105 0x69 0x00 Informative Calibration result (see data sheet)
	writeDirectReg(slic_dev, 106,	INIT_DR106	);//0x20	106 0x6A 0x20 Informative Calibration result (see data sheet)
	writeDirectReg(slic_dev, 107,	INIT_DR107	);//0x08	107 0x6B 0x08 Informative Calibration result (see data sheet)
	writeDirectReg(slic_dev, 108,	INIT_DR108	);//0xEB	108 0x63 0x00 0xEB Initialization Feature enhancement register
}
   
void clearAlarmBits(slic_dev_str *slic_dev) {
	writeDirectReg(slic_dev, 19,0xFC); //Clear Alarm bits
}

void goActive(slic_dev_str *slic_dev) {
	writeDirectReg(slic_dev, 64,1);	/* LOOP STATE REGISTER SET TO ACTIVE */
				/* Active works for on-hook and off-hook see spec. */
				/* The phone hook-switch sets the off-hook and on-hook substate*/
}

unsigned char loopStatus(slic_dev_str *slic_dev) {
static int ReadBack;

	// check for power alarms
	if ((readDirectReg(slic_dev, 19) & 0xfc) != 0) {
		printf ("Power alarm = %x\n",(readDirectReg(slic_dev, 19) & 0xfc));
		clearAlarmBits (slic_dev);
	} 
	ReadBack = readDirectReg (slic_dev, 68);
	return (ReadBack & 0x3);

}

void clearInterrupts(slic_dev_str *slic_dev) {
	writeDirectReg(slic_dev, 	18	,	INIT_DR18	);//0xff	Normal Oper. Interrupt Register 1 (clear with 0xFF)
	writeDirectReg(slic_dev, 	19	,	INIT_DR19	);//0xff	Normal Oper. Interrupt Register 2 (clear with 0xFF)
	writeDirectReg(slic_dev, 	20	,	INIT_DR20	);//0xff	Normal Oper. Interrupt Register 3 (clear with 0xFF)
}


unsigned short manualCalibrate(slic_dev_str *slic_dev) { 
unsigned char x,y,i,progress=0; // progress contains individual bits for the Tip and Ring Calibrations

	//Initialized DR 98 and 99 to get consistant results.
	// 98 and 99 are the results registers and the search should have same intial conditions.
	writeDirectReg(slic_dev, 98,0x10); // This is necessary if the calibration occurs other than at reset time
	writeDirectReg(slic_dev, 99,0x10);

	for ( i=0x1f; i>0; i--)
	{
		writeDirectReg(slic_dev, 98,i);
		mvOsDelay(200);
		if((readDirectReg(slic_dev, 88)) == 0)
		{	progress|=1;
		x=i;
		break;
		}
	} // for



	for ( i=0x1f; i>0; i--)
	{
		writeDirectReg(slic_dev, 99,i);
		mvOsDelay(200);
		if((readDirectReg(slic_dev, 89)) == 0){
			progress|=2;
			y=i;
		break;
		}
	
	}//for

	return progress;
}

int calibrate(slic_dev_str *slic_dev) { 
	unsigned short i=0;
	volatile unsigned char   DRvalue;
	int timeOut,nCalComplete;

	/* Do Flush durring powerUp and calibrate */
	writeDirectReg(slic_dev, 21,DISABLE_ALL_DR21);//(0)  Disable all interupts in DR21
	writeDirectReg(slic_dev, 22,DISABLE_ALL_DR22);//(0)	Disable all interupts in DR21
	writeDirectReg(slic_dev, 23,DISABLE_ALL_DR23);//(0)	Disabel all interupts in DR21
	writeDirectReg(slic_dev, 64,OPEN_DR64);//(0)
  
	writeDirectReg(slic_dev, 97,0x1e); //(0x18)Calibrations without the ADC and DAC offset and without common mode calibration.
	writeDirectReg(slic_dev, 96,0x47); //(0x47)	Calibrate common mode and differential DAC mode DAC + ILIM
	
	printf("Calibration vBat = %x\n ", readDirectReg(slic_dev, 82));
 
	i=0;
	do 
	{
		DRvalue = readDirectReg(slic_dev, 96);
		nCalComplete = DRvalue==CAL_COMPLETE_DR96;// (0)  When Calibration completes DR 96 will be zero
		timeOut= i++>MAX_CAL_PERIOD;
		mvOsDelay(1);
	}
	while (!nCalComplete&&!timeOut);

	if (timeOut) {
	    printf ("Error in Caliberatation: timeOut\n");
	    return 0;
	}        

	/* Perform manual calibration anyway */
	printf("Performing Manual Calibration\n");
	manualCalibrate (slic_dev);
	

    
/*Initialized DR 98 and 99 to get consistant results.*/
/* 98 and 99 are the results registers and the search should have same intial conditions.*/
/*******The following is the manual gain mismatch calibration******/
/*******This is also available as a function **********************/
	// Wait for any power alarms to settle. 
	mvOsDelay(1000);

	writeIndirectReg(slic_dev, 88,0);
	writeIndirectReg(slic_dev, 89,0);
	writeIndirectReg(slic_dev, 90,0);
	writeIndirectReg(slic_dev, 91,0);
	writeIndirectReg(slic_dev, 92,0);
	writeIndirectReg(slic_dev, 93,0);


	goActive(slic_dev);

	if  (loopStatus(slic_dev) & 4) {
		printf ("Error in Caliberate:  ERRORCODE_LONGBALCAL\n");
		return 0 ;
	}

	writeDirectReg(slic_dev, 64,OPEN_DR64);

	writeDirectReg(slic_dev, 23,ENB2_DR23);  // enable interrupt for the balance Cal
	writeDirectReg(slic_dev, 97,BIT_CALCM_DR97); // this is a singular calibration bit for longitudinal calibration
	writeDirectReg(slic_dev, 96,0x40);

	DRvalue = readDirectReg(slic_dev, 96);
	i=0;
	do 
	{
       	DRvalue = readDirectReg(slic_dev, 96);
        nCalComplete = DRvalue==CAL_COMPLETE_DR96;// (0)  When Calibration completes DR 96 will be zero
		timeOut= i++>MAX_CAL_PERIOD;// (800) MS
		mvOsDelay(1);
	}
	while (!nCalComplete&&!timeOut);
	  
	if (timeOut) {
	    printf ("Error in Caliberate:  timeOut\n");
	    return 0;
	}

	mvOsDelay(1000);

	for (i=88; i<=95; i++) {
		writeIndirectReg (slic_dev, i, 0);
	}
	writeIndirectReg (slic_dev, 97, 0);

	for (i=193; i<=211; i++) {
		writeIndirectReg (slic_dev, i, 0);
	}
    		
   	writeDirectReg(slic_dev, 21,INIT_DR21);
	writeDirectReg(slic_dev, 22,INIT_DR22);
	writeDirectReg(slic_dev, 23,INIT_DR23);

/**********************************The preceding is the longitudinal Balance Cal***********************************/


	printf ("Caliberation done\n");
	return(1);

}// End of calibration

   
unsigned char powerUp(slic_dev_str *slic_dev)
{ 
	unsigned char vBat ; 
	int i=0;

	if ((slic_dev->si_model == si3210M)||(slic_dev->si_model == si3215M))  // M version correction
	{
		writeDirectReg(slic_dev, 92,INIT_SI3210M_DR92);// M version
		writeDirectReg(slic_dev, 93,INIT_SI3210M_DR93);// M version
	} else {
		writeDirectReg(slic_dev, 93, 12); 
		writeDirectReg(slic_dev, 92, 0xff); /* set the period of the DC-DC converter to 1/64 kHz  START OUT SLOW*/
	}

	writeDirectReg(slic_dev, 14, 0); /* Engage the DC-DC converter */
  
	while ((vBat=readDirectReg(slic_dev, 82)) < 0xc0)
	{ 
		mvOsDelay (1000);
		++i;
		if (i > 200) return 0;
	}
  	
	/*if (chipType(slic_dev) == 3)  // M version correction
	{
		writeDirectReg(slic_dev, 92,0x80 +INIT_SI3210M_DR92);// M version
	}
	else*/
		writeDirectReg(slic_dev, 93, 0x8c);  /* DC-DC Calibration  */ /* MA */

	while(0x80 & readDirectReg(slic_dev, 93));  // Wait for DC-DC Calibration to complete

	return vBat;
}
	
	
	
int slicStart(slic_dev_str *slic_dev)
{
	volatile unsigned char t,v;
	volatile unsigned short i;

	/*  Another Quarter of a Second */
	//if (!selfTest(slic_dev))
	//	return 0;

	initializeIndirectRegisters(slic_dev);

	i=verifyIndirectRegisters (slic_dev);
	if (i != 0) {
		printf ("verifyIndirect failed\n");
		return 0;
	}

	writeDirectReg (slic_dev, 8, 0);
	if (v == 5)
		writeDirectReg (slic_dev, 108, 0xeb); /* turn on Rev E features. */

	if ((slic_dev->si_model == si3210)||(slic_dev->si_model == si3210M)) // Si3210 not the Si3211 or Si3212	
	{
		writeDirectReg(slic_dev, 67,0x17); // Make VBat switch not automatic 
		// The above is a saftey measure to prevent Q7 from accidentaly turning on and burning out.
		//  It works in combination with the statement below.  Pin 34 DCDRV which is used for the battery switch on the
		//  Si3211 & Si3212 
	
		writeDirectReg(slic_dev, 66,1);  //    Q7 should be set to OFF for si3210
	}

	if (slic_dev->version <=2 ) {  //  REVISION B   
		writeDirectReg(slic_dev, 73,4);  // set common mode voltage to 6 volts
	}

	// PCm Start Count. 
	writeDirectReg (slic_dev, 2, 0);
	writeDirectReg (slic_dev, 4, 0);


	/* Do Flush durring powerUp and calibrate */
	if ((slic_dev->si_model == si3210)||(slic_dev->si_model == si3210M)) {
		printf("Powerup the Slic\n");

		// Turn on the DC-DC converter and verify voltage.
		if (!powerUp(slic_dev)) {
			printf("Failed to powerup the Slic\n");
			return 0;
		}
	}

	initializeDirectRegisters(slic_dev);

	if (!calibrate(slic_dev)) {
		printf("calibrate failed\n");
		return 0;
	}

	printf ("Register 98 = %x\n", readDirectReg(slic_dev, 98));
	printf ("Register 99 = %x\n", readDirectReg(slic_dev, 99));

	clearInterrupts(slic_dev);

	printf("Slic is Active\n");
	goActive(slic_dev);

	return 1;

}

void standardRinging(slic_dev_str *slic_dev) { 	
	// Enables ringing mode on ProSlic for standard North American ring
	//	RING_ON__LO	48
	//	RING_ON_HI	49
	//	RING_OFF_LO	50
	//	RING_OFF_HI	51
	// Active Timer

	writeDirectReg(slic_dev,  RING_ON__LO, 0x80); // low reg 48
	writeDirectReg(slic_dev,  RING_ON_HI, 0x3E); // hi reg 49
	// Inactive Timer
	writeDirectReg(slic_dev,  RING_OFF_LO, 0x00); // low reg 50
	writeDirectReg(slic_dev,  RING_OFF_HI, 0x7D); // hi reg 51
	// Enable timers for ringing oscillator
	writeDirectReg(slic_dev,  34, 0x18);

}

void activateRinging(slic_dev_str *slic_dev) {
	writeDirectReg(slic_dev,  LINE_STATE, RING_LINE); // REG 64,4
}

void stopRinging(slic_dev_str *slic_dev) {
	
	if ((0xf & readDirectReg(slic_dev, 0))<=2 )  // if REVISION B  
        writeDirectReg(slic_dev, 69,10);   // Loop Debounce Register  = initial value
    
	goActive(slic_dev);
	
}

void stopTone(slic_dev_str *slic_dev) 
{
  writeDirectReg(slic_dev, 32, INIT_DR32	);//0x00	Oper. Oscillator 1 Controltone generation
  writeDirectReg(slic_dev, 33, INIT_DR33	);//0x00	Oper. Oscillator 2 Controltone generation

  writeIndirectReg(slic_dev, 13,INIT_IR13);
  writeIndirectReg(slic_dev, 14,INIT_IR14);
  writeIndirectReg(slic_dev, 16,INIT_IR16);
  writeIndirectReg(slic_dev, 17,INIT_IR17);
  writeDirectReg(slic_dev, 36,  INIT_DR36);
  writeDirectReg(slic_dev, 37,  INIT_DR37);
  writeDirectReg(slic_dev, 38,  INIT_DR38);
  writeDirectReg(slic_dev, 39,  INIT_DR39);
  writeDirectReg(slic_dev, 40,  INIT_DR40);
  writeDirectReg(slic_dev, 41,  INIT_DR41);
  writeDirectReg(slic_dev, 42,  INIT_DR42);
  writeDirectReg(slic_dev, 43,  INIT_DR43);

//  writeDirectReg(slic_dev, 32, INIT_DR32	);//0x00	Oper. Oscillator 1 Controltone generation
//  writeDirectReg(slic_dev, 33, INIT_DR33	);//0x00	Oper. Oscillator 2 Controltone generation
}
void dialTone(slic_dev_str *slic_dev)
{
  if ((slic_dev->si_model == si3215)||(slic_dev->si_model == si3215M))
  {
	  writeIndirectReg(slic_dev, 13,SI3215_DIALTONE_IR13);
	  writeIndirectReg(slic_dev, 14,SI3215_DIALTONE_IR14);
	  writeIndirectReg(slic_dev, 16,SI3215_DIALTONE_IR16);
	  writeIndirectReg(slic_dev, 17,SI3215_DIALTONE_IR17);
  }
  else
  {
	  writeIndirectReg(slic_dev, 13,SI3210_DIALTONE_IR13);
	  writeIndirectReg(slic_dev, 14,SI3210_DIALTONE_IR14);
	  writeIndirectReg(slic_dev, 16,SI3210_DIALTONE_IR16);
	  writeIndirectReg(slic_dev, 17,SI3210_DIALTONE_IR17);
  }

  writeDirectReg(slic_dev, 36,  DIALTONE_DR36);
  writeDirectReg(slic_dev, 37,  DIALTONE_DR37);
  writeDirectReg(slic_dev, 38,  DIALTONE_DR38);
  writeDirectReg(slic_dev, 39,  DIALTONE_DR39);
  writeDirectReg(slic_dev, 40,  DIALTONE_DR40);
  writeDirectReg(slic_dev, 41,  DIALTONE_DR41);
  writeDirectReg(slic_dev, 42,  DIALTONE_DR42);
  writeDirectReg(slic_dev, 43,  DIALTONE_DR43);

  writeDirectReg(slic_dev, 32,  DIALTONE_DR32);
  writeDirectReg(slic_dev, 33,  DIALTONE_DR33);
}


void busyTone(slic_dev_str *slic_dev)
{
  if ((slic_dev->si_model == si3215)||(slic_dev->si_model == si3215M))
  {
	  writeIndirectReg(slic_dev, 13,SI3215_BUSYTONE_IR13);
	  writeIndirectReg(slic_dev, 14,SI3215_BUSYTONE_IR14);
	  writeIndirectReg(slic_dev, 16,SI3215_BUSYTONE_IR16);
	  writeIndirectReg(slic_dev, 17,SI3215_BUSYTONE_IR17);
  }
  else
  {
	  writeIndirectReg(slic_dev, 13,SI3210_BUSYTONE_IR13);
	  writeIndirectReg(slic_dev, 14,SI3210_BUSYTONE_IR14);
	  writeIndirectReg(slic_dev, 16,SI3210_BUSYTONE_IR16);
	  writeIndirectReg(slic_dev, 17,SI3210_BUSYTONE_IR17);
  }
  writeDirectReg(slic_dev, 36,  BUSYTONE_DR36);
  writeDirectReg(slic_dev, 37,  BUSYTONE_DR37);
  writeDirectReg(slic_dev, 38,  BUSYTONE_DR38);
  writeDirectReg(slic_dev, 39,  BUSYTONE_DR39);
  writeDirectReg(slic_dev, 40,  BUSYTONE_DR40);
  writeDirectReg(slic_dev, 41,  BUSYTONE_DR41);
  writeDirectReg(slic_dev, 42,  BUSYTONE_DR42);
  writeDirectReg(slic_dev, 43,  BUSYTONE_DR43);
  
  writeDirectReg(slic_dev, 32,  BUSYTONE_DR32);
  writeDirectReg(slic_dev, 33,  BUSYTONE_DR33);
}

void PhoneStatus(slic_dev_str *slic_dev) {
	int dtmf_digit;
	int onhook, k;

	for(k=0;k<30;k++) {
		dtmf_digit=readDirectReg(slic_dev, 24);
		onhook=readDirectReg(slic_dev, 68);
		if (onhook==0x04)
			printf ("on-hook, dtmf %d\n", dtmf_digit&0x0f);
		else
			printf ("off-hook, dtmf %d\n", dtmf_digit&0x0f);
		sleep(1);
	}
	printf ("\n");
}

int main (int argc, char * argv[])
{
	slic_dev_str	*pslic_dev;
	unsigned int	k, addr, data;
	char		device[] = "/dev/spidev1.0";
	
	pslic_dev=&slic_dev;
	pslic_dev->sf=fopen(device, "r+b");
	if (pslic_dev->sf == NULL) {
		printf ("cannot open %s\n", device);
		return EXIT_FAILURE;
	}

	setvbuf ( pslic_dev->sf , NULL , _IONBF , 1024 );	
	
	device_identification(pslic_dev);	
	
	if (argc==2) {
		if (!strcmp( argv[1], "init" )) {
			printf("Init slic\n");
			slicStart(pslic_dev);			
		}
		if (!strcmp( argv[1], "startring" )) {
			printf("Start Ring\n");
			activateRinging(pslic_dev);
		}		
		if (!strcmp( argv[1], "stopring" )) {
			printf("Stop Ring\n");
			stopRinging(pslic_dev);
		}		
		if (!strcmp( argv[1], "stoptone" )) {
			printf("Stop Tone\n");
			stopTone(pslic_dev);
		}		
		if (!strcmp( argv[1], "dialtone" )) {
			printf("Dial Tone\n");
			dialTone(pslic_dev);
		}		
		if (!strcmp( argv[1], "busytone" )) {
			printf("Busy Tone\n");
			busyTone(pslic_dev);
		}		
		if (!strcmp( argv[1], "status" )) {
			PhoneStatus(pslic_dev);
		}		
		
	}

/*	for(k=0;k<SI3210_REG_NUM;k++)
		siregs[k]=readDirectReg(pslic_dev, k);
	dump_reg(siregs);

	if (argc==2) {
		addr=strtol (argv[1],NULL,0);
		printf("reg[%03d]: 0x%02x\n", addr, readDirectReg(pslic_dev, addr));
	}

	if (argc==3) {
		addr=strtol (argv[1],NULL,0);
		data=strtol (argv[2],NULL,0);
		writeDirectReg(pslic_dev, addr, data);
		printf("reg[%03d]: 0x%02x\n", addr, data);
	}*/
		
	fclose(pslic_dev->sf);

	return EXIT_SUCCESS;
}

