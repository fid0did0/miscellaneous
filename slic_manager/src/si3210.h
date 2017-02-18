#define _BSD_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <pthread.h>
#include <poll.h>
#include "config.h"

#define I_STATUS		31
#define IDA_LO			28
#define IDA_HI			29
#define IAA				30

#define	INIT_IR0		0x55C2	// DTMF_ROW_0_PEAK	
#define	INIT_IR1		0x51E6  //	DTMF_ROW_0_PEAK,	
#define	INIT_IR2		0x4B85	//	DTMF_ROW_1_PEAK,	
#define	INIT_IR3		0x4937	//	DTMF_ROW2_PEAK,	
#define	INIT_IR4		0x3333	//	DTMF_ROW3_PEAK,	
#define	INIT_IR5		0x0202	//	DTMF_COL1_PEAK,	
#define	INIT_IR6		0x0202	//	DTMF_FWD_TWIST,	
#define	INIT_IR7		0x0198	//	DTMF_RVS_TWIST,	
#define	INIT_IR8		0x0198	//	DTMF_ROW_RATIO,	
#define	INIT_IR9		0x0611	//	DTMF_COL_RATIO,	
#define	INIT_IR10		0x0202	//	DTMF_ROW_2ND_ARM,	
#define	INIT_IR11		0x00E5	//	DTMF_COL_2ND_ARM,	
#define	INIT_IR12		0x0A1C	//	DTMF_PWR_MIN_,	
#define	INIT_IR13		0x7b30	//	DTMF_OT_LIM_TRES,	
#define	INIT_IR14		0x0063	//	OSC1_COEF,	
#define	INIT_IR15		0x0000	//	OSC1X,	
#define	INIT_IR16		0x7870	//	OSC1Y,	
#define	INIT_IR17		0x007d	//	OSC2_COEF,	
#define	INIT_IR18		0x0000	//	OSC2X,	
#define	INIT_IR19		0x0000	//	OSC2Y,	
#define	INIT_IR20		0x7EF0	//	RING_V_OFF,	
#define	INIT_IR21		0x0160	//	RING_OSC,	
#define	INIT_IR22		0x0000	//	RING_X,	
#define	INIT_IR23		0x2000	//	RING_Y,	
#define	INIT_IR24		0x2000	//	PULSE_ENVEL,	
#define	INIT_IR25		0x0000	//	PULSE_X,	
#define	INIT_IR26		0x4000	//	PULSE_Y,	
#define	INIT_IR27		0x4000	//	RECV_DIGITAL_GAIN,	
#define	INIT_IR28		0x1000	//	XMIT_DIGITAL_GAIN,	
#define	INIT_IR29		0x3600	//	LOOP_CLOSE_TRES,	
#define	INIT_IR30		0x1000	//	RING_TRIP_TRES,	
#define	INIT_IR31		0x0200	//	COMMON_MIN_TRES,	
#define	INIT_IR32		0x7c0  	//	COMMON_MAX_TRES,	
#define	INIT_IR33		0x2600	//	PWR_ALARM_Q1Q2,	
#define	INIT_IR34		0x1B80	//	PWR_ALARM_Q3Q4,	
#define	INIT_IR35		0x8000	//	PWR_ALARM_Q5Q6,	
#define	INIT_IR36		0x0320	//	LOOP_CLSRE_FlTER,	
#define	INIT_IR37		0x08c	//	RING_TRIP_FILTER,	
#define	INIT_IR38		0x0100	//	TERM_LP_POLE_Q1Q2,	
#define	INIT_IR39		0x010	//	TERM_LP_POLE_Q3Q4,	
#define	INIT_IR40		0x0001  // MA 0x0C00	//	TERM_LP_POLE_Q5Q6,	
#define	INIT_IR41		0x0C00	//	CM_BIAS_RINGING,	
#define	INIT_IR42		0x1000	//	DCDC_MIN_V,	
#define	INIT_IR43		0x00DA	//	"LOOP_CLOSE_TRES Low
#define	INIT_IR99		0x00DA	// FSK 0 FREQ PARAM
#define	INIT_IR100		0x6B60	// FSK 0 AMPL PARAM
#define	INIT_IR101		0x0074	// FSK 1 FREQ PARAM
#define	INIT_IR102		0x79C0	// FSK 1 AMPl PARAM
#define	INIT_IR103		0x1120	// FSK 0to1 SCALER
#define	INIT_IR104		0x3BE0	// FSK 1to0 SCALER
#define	INIT_IR97		0x0000	// TRASMIT_FILTER

#define	INIT_DR0_DC	0x80	//	Serial Interface
#define	INIT_DR0	0x00	//	Serial Interface
#define	INIT_DR1	0x08	//	PCM Mode
#define	INIT_DR2	0x00	//	PCM TX Clock Slot Low Byte (1 PCLK cycle/LSB)
#define	INIT_DR3	0x00	//	PCM TX Clock Slot High Byte
#define	INIT_DR4	0x00	//	PCM RX Clock Slot Low Byte (1 PCLK cycle/LSB)
#define	INIT_DR5	0x00	//	PCM RX Clock Slot High Byte
#define	INIT_DR6	0x00	//	DIO Control (external battery operation, Si3211/12)
#define	INIT_DR8	0x00	//	Loopbacks (digital loopback default)
#define	INIT_DR9	0x04/*0x00*/	//	Transmit and receive path gain and control
#define	INIT_DR10	0x28	//	Initialization Two-wire impedance (600  and enabled)
#define	INIT_DR11	0x33	//	Transhybrid Balance/Four-wire Return Loss
#define	INIT_DR14	0x10	//	Powerdown Control 1
#define	INIT_DR15	0x00	//	Initialization Powerdown Control 2
#define	INIT_DR18	0xff	//	Normal Oper. Interrupt Register 1 (clear with 0xFF)
#define	INIT_DR19	0xff	//	Normal Oper. Interrupt Register 2 (clear with 0xFF)
#define	INIT_DR20	0xff	//	Normal Oper. Interrupt Register 3 (clear with 0xFF)
#define	INIT_DR21	0xff	//	Interrupt Mask 1
#define	INIT_DR22	0xff	//	Initialization Interrupt Mask 2
#define	INIT_DR23	0xff	//	 Initialization Interrupt Mask 3
#define	INIT_DR32	0x00	//	Oper. Oscillator 1 Control—tone generation
#define	INIT_DR33	0x00	//	Oper. Oscillator 2 Control—tone generation
#define	INIT_DR34	0x18	//	34 0x22 0x00 Initialization Ringing Oscillator Control
#define	INIT_DR35	0x00	//	Oper. Pulse Metering Oscillator Control
#define	INIT_DR36	0x00	//	36 0x24 0x00 Initialization OSC1 Active Low Byte (125 µs/LSB)
#define	INIT_DR37	0x00	//	37 0x25 0x00 Initialization OSC1 Active High Byte (125 µs/LSB)
#define	INIT_DR38	0x00	//	38 0x26 0x00 Initialization OSC1 Inactive Low Byte (125 µs/LSB)
#define	INIT_DR39	0x00	//	39 0x27 0x00 Initialization OSC1 Inactive High Byte (125 µs/LSB)
#define	INIT_DR40	0x00	//	40 0x28 0x00 Initialization OSC2 Active Low Byte (125 µs/LSB)
#define	INIT_DR41	0x00	//	41 0x29 0x00 Initialization OSC2 Active High Byte (125 µs/LSB)
#define	INIT_DR42	0x00	//	42 0x2A 0x00 Initialization OSC2 Inactive Low Byte (125 µs/LSB)
#define	INIT_DR43	0x00	//	43 0x2B 0x00 Initialization OSC2 Inactive High Byte (125 µs/LSB)
#define	INIT_DR44	0x00	//	44 0x2C 0x00 Initialization Pulse Metering Active Low Byte (125 µs/LSB)
#define	INIT_DR45	0x00	//	45 0x2D 0x00 Initialization Pulse Metering Active High Byte (125 µs/LSB)
#define	INIT_DR46	0x00	//	46 0x2E 0x00 Initialization Pulse Metering Inactive Low Byte (125 µs/LSB)
#define	INIT_DR47	0x00	//	47 0x2F 0x00 Initialization Pulse Metering Inactive High Byte (125 µs/LSB)
#define	INIT_DR48	0x80	//	48 0x30 0x00 0x80 Initialization Ringing Osc. Active Timer Low Byte (2 s,125 µs/LSB)
#define	INIT_DR49	0x3E	//	49 0x31 0x00 0x3E Initialization Ringing Osc. Active Timer High Byte (2 s,125 µs/LSB)
#define	INIT_DR50	0x00	//	50 0x32 0x00 0x00 Initialization Ringing Osc. Inactive Timer Low Byte (4 s, 125 µs/LSB)
#define	INIT_DR51	0x7D	//	51 0x33 0x00 0x7D Initialization Ringing Osc. Inactive Timer High Byte (4 s, 125 µs/LSB)
#define	INIT_DR52	0x00	//	52 0x34 0x00 Normal Oper. FSK Data Bit
#define	INIT_DR63	0x54	//	63 0x3F 0x54 Initialization Ringing Mode Loop Closure Debounce Interval
#define	INIT_DR64	0x00	//	64 0x40 0x00 Normal Oper. Mode Byte—primary control
#define	INIT_DR65	0x61	//	65 0x41 0x61 Initialization External Bipolar Transistor Settings
#define	INIT_DR66	0x03	//	66 0x42 0x03 Initialization Battery Control
#define	INIT_DR67	0x1F	//	67 0x43 0x1F Initialization Automatic/Manual Control
#define	INIT_DR69	0x0C	//	69 0x45 0x0A 0x0C Initialization Loop Closure Debounce Interval (1.25 ms/LSB)
#define	INIT_DR70	0x0A	//	70 0x46 0x0A Initialization Ring Trip Debounce Interval (1.25 ms/LSB)
#define	INIT_DR71	0x01	//	71 0x47 0x00 0x01 Initialization Off-Hook Loop Current Limit (20 mA + 3 mA/LSB)
#define	INIT_DR72	0x20	//	72 0x48 0x20 Initialization On-Hook Voltage (open circuit voltage) = 48 V(1.5 V/LSB)
#define	INIT_DR73	0x02	//	73 0x49 0x02 Initialization Common Mode Voltage—VCM = –3 V(–1.5 V/LSB)
#define	INIT_DR74	0x2f    // MA 0x32	//	74 0x4A 0x32 Initialization VBATH (ringing) = –75 V (–1.5 V/LSB)
#define	INIT_DR75	0x10	//	75 0x4B 0x10 Initialization VBATL (off-hook) = –24 V (TRACK = 0)(–1.5 V/LSB)
#define	INIT_DR92	0xff    // MA 0x7f	//	92 0x5C  7F Initialization DC–DC Converter PWM Period (61.035 ns/LSB)
#define	INIT_DR93	0x0c    // MA 0x14	//	93 0x5D 0x14 0x19 Initialization DC–DC Converter Min. Off Time (61.035 ns/LSB)
#define	INIT_DR96	0x00	//	96 0x60 0x1F Initialization Calibration Control Register 1(written second and starts calibration)
#define	INIT_DR97	0x1F	//	97 0x61 0x1F Initialization Calibration Control Register 2(written before Register 96)
#define	INIT_DR98	0x10	//	98 0x62 0x10 Informative Calibration result (see data sheet)
#define	INIT_DR99	0x10	//	99 0x63 0x10 Informative Calibration result (see data sheet)
#define	INIT_DR100	0x11	//	100 0x64 0x11 Informative Calibration result (see data sheet)
#define	INIT_DR101	0x11	//	101 0x65 0x11 Informative Calibration result (see data sheet)
#define	INIT_DR102	0x08	//	102 0x66 0x08 Informative Calibration result (see data sheet)
#define	INIT_DR103	0x88	//	103 0x67 0x88 Informative Calibration result (see data sheet)
#define	INIT_DR104	0x00	//	104 0x68 0x00 Informative Calibration result (see data sheet)
#define	INIT_DR105	0x00	//	105 0x69 0x00 Informative Calibration result (see data sheet)
#define	INIT_DR106	0x20	//	106 0x6A 0x20 Informative Calibration result (see data sheet)
#define	INIT_DR107	0x08	//	107 0x6B 0x08 Informative Calibration result (see data sheet)
#define	INIT_DR108	0xEB	//	108 0x63 0x00 0xEB Initialization Feature enhancement register
#define INIT_SI3210M_DR92 0x60  //  92 0x60 Initialization DC–DC Converter PWM Period (61.035 ns/LSB)
#define INIT_SI3210M_DR93 0x38  //  92 0x60 Initialization DC–DC Converter PWM Period (61.035 ns/LSB)

#define DISABLE_ALL_DR21 0
#define DISABLE_ALL_DR22 0
#define DISABLE_ALL_DR23 0

#define	BIT_CALM1_DR97	0x10	//	CALM1 Monitor ADC Calibration 1.
#define	BIT_CALM2_DR97	0x08	//	CALM2 Monitor ADC Calibration 2.
#define	BIT_CALDAC_DR97	0x04	//	CALDAC DAC Calibration.
#define	BIT_ADC_DR97	0x02	//	CALADC ADC Calibration.
#define	BIT_CALCM_DR97	0x01	//	CALCM Common Mode Balance Calibration.
#define	STANDARD_CAL_DR97	0x18	//	Calibrations without the ADC and DAC offset and without common mode calibration.
#define	MASK_CAL_DR96	0x40	//	CAL Calibration Status Bit.
#define	BIT_CAL_DR96	0x40	//	CAL Calibration Control
#define	BIT_CALR_DR96	0x10	//	CALR RING Gain Mismatch Calibration.
#define	BIT_CALT_DR96	0x08	//	CALT TIP Gain Mismatch Calibration.
#define	BIT_CALD_DR96	0x04	//	CALD Differential DAC Gain Calibration.
#define	BIT_CALC_DR96	0x02	//	CALC Common Mode DAC Gain Calibration.
#define	BIT_CALIL_DR96	0x01	//	CALIL ILIM Calibration.
#define	STANDARD_CAL_DR96	0x47	//	Calibrate common mode and differential DAC mode DAC + ILIM
#define	BIT_IRQE_CMCE	0x04	//	Calibrate common mode and differential mode DAC + ILIM
#define CAL_COMPLETE_DR96 0     //  Value in dr96 after calibration is completed
#define MAX_CAL_PERIOD 800		// The longest period in ms. for a calibration to complete.

#define	OPEN_DR64	0	//	
#define	FORWARD_ACTIVE_DR64	1	//	
#define	OHT_DR64	2	//	
#define	TIP_OPEN_DR64	3	//	
#define	RINGING_DR64	4	//	
#define	REVERSE_ACTIVE_DR64	5	//	
#define	REVERSE_OHT_DR64	6	//	
#define	RING_OPEN_DR64	7	//	

#define ENB2_DR23  1<<2 // enable interrupt for the balance Cal

#define	AUDIO_LOOPBACK	8
#define	AUDIO_GAIN	9
#define	LINE_IMPEDANCE	10
#define	HYBRID	11
#define	RESERVED12	12
#define	RESERVED13	13
#define	PWR_DOWN1	14
#define	PWR_DOWN2	15
#define	RESERVED16	16
#define	RESERVED17	17
#define	INTRPT_STATUS1	18
#define	INTRPT_STATUS2	19
#define	INTRPT_STATUS3	20
#define	INTRPT_MASK1	21
#define	INTRPT_MASK2	22
#define	INTRPT_MASK3	23
#define	DTMF_DIGIT	24
#define	RESERVED25	25
#define	RESERVED26	26
#define	RESERVED27	27
#define	I_DATA_LOW	28
#define	I_DATA_HIGH	29
#define	I_ADDRESS	30
#define	I_STATUS	31
#define	OSC1	32
#define	OSC2	33
#define	RING_OSC_CTL	34
#define	PULSE_OSC	35
#define	OSC1_ON__LO	36
#define	OSC1_ON_HI	37
#define	OSC1_OFF_LO	38
#define	OSC1_OFF_HI	39
#define	OSC2_ON__LO	40
#define	OSC2_ON_HI	41
#define	OSC2_OFF_LO	42
#define	OSC2_OFF_HI	43
#define	PULSE_ON__LO	44
#define	PULSE_ON_HI	45
#define	PULSE_OFF_LO	46
#define	PULSE_OFF_HI	47
#define	RING_ON__LO	48
#define	RING_ON_HI	49
#define	RING_OFF_LO	50
#define	RING_OFF_HI	51
#define	FSK_DATA	52	/*		0								fsk_data	*/
#define	RESERVED53	53
#define	RESERVED54	54
#define	RESERVED55	55
#define	RESERVED56	56
#define	RESERVED57	57
#define	RESERVED58	58
#define	RESERVED59	59
#define	RESERVED60	60
#define	RESERVED61	61
#define	RESERVED62	62
#define	RESERVED63	63
#define	LINE_STATE	64
#define			ACTIVATE_LINE 0x11
#define			RING_LINE     0x44
#define	BIAS_SQUELCH	65
#define	BAT_FEED	66
#define	AUTO_STATE	67
#define	LOOP_STAT	68
#define	LOOP_DEBOUCE	69
#define	RT_DEBOUCE	70
#define	LOOP_I_LIMIT	71
#define	OFF_HOOK_V	72
#define	COMMON_V	73
#define	BAT_V_HI	74
#define	BAT_V_LO	75
#define	PWR_STAT_DEV	76
#define	PWR_STAT	77
#define	LOOP_V_SENSE	78
#define	LOOP_I_SENSE	79
#define	TIP_V_SENSE	80
#define	RING_V_SENSE	81
#define	BAT_V_HI_SENSE	82
#define	BAT_V_LO_SENSE	83
#define	IQ1	84
#define	IQ2	85
#define	IQ3	86
#define	IQ4	87
#define	IQ5	88
#define	IQ6	89
#define	RESERVED90	90
#define	RESERVED91	91
#define	DCDC_PWM_OFF	92
#define	DCDC	93
#define	DCDC_PW_OFF	94
#define	RESERVED95	95
#define	CALIBR1	96
#define CALIBRATE_LINE 0x78
#define NORMAL_CALIBRATION_COMPLETE 0x20
#define	CALIBR2	97
#define	RING_GAIN_CAL	98
#define	TIP_GAIN_CAL	99
#define	DIFF_I_CAL	100
#define	COMMON_I_CAL	101
#define	I_LIMIT_GAIN_CAL	102
#define	ADC_OFFSET_CAL	103
#define	DAC_ADC_OFFSET	104
#define	DAC_OFFSET_CAL	105
#define	COMMON_BAL_CAL	106
#define	DC_PEAK_CAL	107

#define	SI3210_BUSYTONE_IR13	0x7700	//tone_struct BusySignal = { /* OSC1= 480  OSC2 = 620 .0975 Voltz -18 dBm 8*/
#define	SI3210_BUSYTONE_IR14	0x0089	
#define	SI3210_BUSYTONE_IR16	0x7120	
#define	SI3210_BUSYTONE_IR17	0x00B2	

#define	SI3215_BUSYTONE_IR13	0x7DBB	//tone_struct BusySignal = { /* OSC1= 480  OSC2 = 620 .0975 Voltz -18 dBm 8*/
#define	SI3215_BUSYTONE_IR14	0x0044	
#define	SI3215_BUSYTONE_IR16	0x7C39	
#define	SI3215_BUSYTONE_IR17	0x0058	

#define	BUSYTONE_DR32	0x1E	
#define	BUSYTONE_DR33	0x1E	
#define	BUSYTONE_DR36	0xa0	
#define	BUSYTONE_DR37	0x0f	
#define	BUSYTONE_DR38	0xa0	
#define	BUSYTONE_DR39	0x0f	
#define	BUSYTONE_DR40	0xa0	
#define	BUSYTONE_DR41	0x0f	
#define	BUSYTONE_DR42	0xa0	
#define	BUSYTONE_DR43	0x0f	

#define	SI3210_DIALTONE_IR13	0x7b30	//tone_struct DialTone = {  /* OSC1= 350 Hz OSC2= 440 Hz .0975 Volts -18 dBm */
#define	SI3210_DIALTONE_IR14	0x0063	
#define	SI3210_DIALTONE_IR16	0x7870	
#define	SI3210_DIALTONE_IR17	0x007d	

#define	SI3215_DIALTONE_IR13	0x7eca	//tone_struct DialTone = {  /* OSC1= 350 Hz OSC2= 440 Hz .0975 Volts -18 dBm */
#define	SI3215_DIALTONE_IR14	0x0031	
#define	SI3215_DIALTONE_IR16	0x7e18	
#define	SI3215_DIALTONE_IR17	0x003e	
	
#define	DIALTONE_DR32	6	
#define	DIALTONE_DR33	6	
#define	DIALTONE_DR36	0	
#define	DIALTONE_DR37	0	
#define	DIALTONE_DR38	0	
#define	DIALTONE_DR39	0	
#define	DIALTONE_DR40	0	
#define	DIALTONE_DR41	0	
#define	DIALTONE_DR42	0	
#define	DIALTONE_DR43	0

#define	DIALTONE_SI3216_IR13	0x7ed0	
#define	DIALTONE_SI3216_IR14	0x0031	
#define	DIALTONE_SI3216_IR16	0x7e20	
#define	DIALTONE_SI3216_IR17	0x003d	
#define	DIALTONE_SI3216_DR32	6	
#define	DIALTONE_SI3216_DR33	6	
#define	DIALTONE_SI3216_DR36	0	
#define	DIALTONE_SI3216_DR37	0	
#define	DIALTONE_SI3216_DR38	0	
#define	DIALTONE_SI3216_DR39	0	
#define	DIALTONE_SI3216_DR40	0	
#define	DIALTONE_SI3216_DR41	0	
#define	DIALTONE_SI3216_DR42	0	
#define	DIALTONE_SI3216_DR43	0	

typedef enum { si3210, si3210M, si3211, si3215, si3215M } si_model_type;
typedef struct {uint8_t	addr; uint16_t data;} IndRegs;
typedef struct {uint8_t	addr; uint8_t data;} DirRegs;
typedef struct {
	FILE 		*sf;
	pthread_mutex_t SpiMutex;
//	struct pollfd 	*fdset;
	si_model_type	si_model;
	unsigned char	version;
//	unsigned char	txSample;
//	unsigned char	rxSample;
//	unsigned char	dcval; /* daisy chain value */
} slic_dev_str;

void InitSpi(slic_dev_str *slic_dev);
void DestroySpi(slic_dev_str *slic_dev);
	
void waitForIndirectReg(slic_dev_str *pslic_dev);
unsigned char readDirectReg(slic_dev_str *pslic_dev, unsigned char address);
void writeDirectReg(slic_dev_str *pslic_dev, unsigned char address, unsigned char data);
unsigned short readIndirectReg(slic_dev_str *pslic_dev, unsigned char address);
void writeIndirectReg(slic_dev_str *pslic_dev, unsigned char address, unsigned short data);

void version(slic_dev_str *slic_dev);
void device_identification(slic_dev_str *slic_dev);

void clearInterrupts(slic_dev_str *slic_dev);
int slicStart(slic_dev_str *slic_dev);
void powerDown(slic_dev_str *slic_dev);

void stopTone(slic_dev_str *slic_dev);
void dialTone(slic_dev_str *slic_dev);
void busyTone(slic_dev_str *slic_dev);