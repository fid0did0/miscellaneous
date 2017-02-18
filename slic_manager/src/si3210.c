#include "si3210.h"

IndRegs si3210IR_init[] = {{0x00, 0x55C2}, {0x01, 0x51E6}, 	// 00-01
				{0x02, 0x4B85}, {0x03, 0x4937}, // 02-03
				{0x04, 0x3333}, {0x05, 0x0202}, // 04-05
				{0x06, 0x0202}, {0x07, 0x0198}, // 06-07
				{0x08, 0x0198}, {0x09, 0x0611}, // 08-09
				{0x0A, 0x0202}, {0x0B, 0x00E5}, // 10-11
				{0x0C, 0x0A1C}, {0x0D, 0x7b30}, // 12-13
				{0x0E, 0x0063}, {0x0F, 0x0000}, // 14-15
				{0x10, 0x7870}, {0x11, 0x007d}, // 16-17
				{0x12, 0x0000}, {0x13, 0x0000}, // 18-19
				{0x14, 0x7EF0}, {0x15, 0x0160}, // 20-21
				{0x16, 0x0000}, {0x17, 0x2000}, // 22-23
				{0x18, 0x2000}, {0x19, 0x0000}, // 24-25
				{0x1A, 0x4000}, {0x1B, 0x4000}, // 26-27
				{0x1C, 0x1000}, {0x1D, 0x3600}, // 28-29
				{0x1E, 0x1000}, {0x1F, 0x0200}, // 30-31
				{0x20, 0x07c0}, {0x21, 0x2600}, // 32-33
				{0x22, 0x1B80}, {0x23, 0x8000}, // 34-35
				{0x24, 0x0320}, {0x25, 0x008c}, // 36-37
				{0x26, 0x0100}, {0x27, 0x0010}, // 38-39
				{0x28, 0x0001}, {0x29, 0x0C00}, // 40-41
				{0x2A, 0x1000}, {0x2B, 0x00DA}, // 42-43
	//			{0x61, 0x0000}, 		// 97
				{0x63, 0x00DA}, {0x64, 0x6B60}, // 99-100
				{0x65, 0x0074}, {0x66, 0x79C0}, // 101-102
				{0x67, 0x1120}, {0x68, 0x3BE0}};// 103-104

DirRegs si3210DR_init[] = {{0x00, 0x00}, {0x01, 0x08}, 		// 00-01
				{0x02, 0x00}, {0x03, 0x00}, 	// 02-03
				{0x04, 0x00}, {0x05, 0x00}, 	// 04-05
				{0x06, 0x00}, 		 	// 06-07
				{0x08, 0x00}, {0x09, 0x04}, 	// 08-09
				{0x0A, 0x28}, {0x0B, 0x33}, 	// 10-11
				
				{0x0E, 0x00}, {0x0F, 0x00}, 	// 14-15
				
				{0x12, 0xff}, {0x13, 0x00}, 	// 18-19
				{0x14, 0xff}, {0x15, 0xff}, 	// 20-21
				{0x16, 0xff}, {0x17, 0xff}, 	// 22-23
				
				{0x20, 0x00}, {0x21, 0xff}, 	// 32-33
				{0x22, 0x18}, {0x23, 0x00}, 	// 34-35
				{0x24, 0x00}, {0x25, 0x00}, 	// 36-37
				{0x26, 0x00}, {0x27, 0x00}, 	// 38-39
				{0x28, 0x00}, {0x29, 0x00}, 	// 40-41
				{0x2A, 0x00}, {0x2B, 0x00}, 	// 42-43
				{0x2C, 0x00}, {0x2D, 0x00}, 	// 44-45
				{0x2E, 0x00}, {0x2F, 0x00}, 	// 46-47
				{0x30, 0x80}, {0x31, 0x3E}, 	// 48-49
				{0x32, 0x00}, {0x33, 0x7D}, 	// 50-51
				{0x34, 0x00}, 		 	// 52-53	
					      {0x3F, 0x54}, 	// 62-63
				{0x40, 0x00}, {0x41, 0x61}, 	// 64-65
				{0x42, 0x03}, {0x43, 0x1F}, 	// 66-67
					      {0x45, 0x0C}, 	// 68-69
				{0x46, 0x0A}, {0x47, 0x01}, 	// 70-71
				{0x48, 0x20}, {0x49, 0x02}, 	// 72-73
				{0x4A, 0x2f}, {0x4B, 0x10}, 	// 74-75
				
				{0x5C, 0xff}, {0x5D, 0x0c}, 	// 92-93
				
				{0x60, 0x00}, {0x61, 0x1F}, 	// 96-97
				{0x62, 0x10}, {0x63, 0x10}, 	// 98-99
				{0x64, 0x11}, {0x65, 0x11}, 	// 100-101
				{0x66, 0x08}, {0x67, 0x88}, 	// 102-103
				{0x68, 0x00}, {0x69, 0x00}, 	// 104-105
				{0x6A, 0x20}, {0x6B, 0x08}, 	// 106-107
				{0x6C, 0xEB}};
	
slic_dev_str	slic_dev;

void mvOsDelay(int delay_val) {
	usleep(1000*delay_val);
}

void version(slic_dev_str *slic_dev) {
	slic_dev->version=(0xf & readDirectReg(slic_dev, 0));
	printf("device version %d\n", slic_dev->version);
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

void initializeIndirectRegisters(slic_dev_str *slic_dev) {	
	unsigned int k;
	
	printf("writing %d iregs\n", sizeof(si3210IR_init)/sizeof(IndRegs));
	
	if (slic_dev->si_model!=si3215)
		for(k=0;k<13;k++)
			writeIndirectReg(slic_dev, si3210IR_init[k].addr, si3210IR_init[k].data);
	
	for(k=13;k<sizeof(si3210IR_init)/sizeof(IndRegs);k++)
		writeIndirectReg(slic_dev, si3210IR_init[k].addr, si3210IR_init[k].data);

	writeIndirectReg(slic_dev, 97, 0);

}	

int verifyIndirectRegisters(slic_dev_str *slic_dev) {
	unsigned int k;
	int error=0;
	
	if (slic_dev->si_model!=si3215)
		for(k=0;k<13;k++)
			error |= verifyIndirectReg(slic_dev, si3210IR_init[k].addr, si3210IR_init[k].data);
		
	for(k=13;k<sizeof(si3210IR_init)/sizeof(IndRegs);k++)	
		error |= verifyIndirectReg(slic_dev, si3210IR_init[k].addr, si3210IR_init[k].data);
	
	return error;
}

void initializeDirectRegisters(slic_dev_str *slic_dev) {
	unsigned int k;
	
	printf("writing %d dregs\n", sizeof(si3210IR_init)/sizeof(IndRegs));

	for(k=0;k<sizeof(si3210DR_init)/sizeof(DirRegs);k++)
		writeDirectReg(slic_dev, si3210DR_init[k].addr, si3210DR_init[k].data);
	
	if (slic_dev->si_model==si3210M)
		writeDirectReg(slic_dev, 92, INIT_SI3210M_DR92 ); //0x7f	92 0x5C 0xFF 7F Initialization DCDC Converter PWM Period (61.035 ns/LSB)
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
	writeDirectReg(slic_dev, 	20	,	INIT_DR20	);//0xff	Normal Oper. Interrupt Register 3 (clear with 0xFF)
}


unsigned short manualCalibrate(slic_dev_str *slic_dev) { 
unsigned char i,progress=0; // progress contains individual bits for the Tip and Ring Calibrations

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
		break;
		}
	} // for



	for ( i=0x1f; i>0; i--)
	{
		writeDirectReg(slic_dev, 99,i);
		mvOsDelay(200);
		if((readDirectReg(slic_dev, 89)) == 0){
			progress|=2;			
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
  
	vBat=0x0;
	while (vBat < 0x80)
	{ 
		vBat=readDirectReg(slic_dev, 82);
		printf("vbat: %d\n", vBat);
		mvOsDelay (1000);		
		++i;
		if (i > 200) return 0;
	}
  	
	printf("start DC-DC cal\n");
	/*if (chipType(slic_dev) == 3)  // M version correction
	{
		writeDirectReg(slic_dev, 92,0x80 +INIT_SI3210M_DR92);// M version
	}
	else*/
		writeDirectReg(slic_dev, 93, 0x8c);  /* DC-DC Calibration  */ /* MA */

	while(0x80 & readDirectReg(slic_dev, 93));  // Wait for DC-DC Calibration to complete
	
	printf("done DC-DC cal\n");

	return vBat;
}
	
void powerDown(slic_dev_str *slic_dev) { 
	writeDirectReg(slic_dev, 14, 0x10);
}
	
int slicStart(slic_dev_str *slic_dev) {
	//volatile unsigned char v;
	
	//if (!selfTest(slic_dev)) return 0;

	initializeIndirectRegisters(slic_dev);
	if (verifyIndirectRegisters (slic_dev)) {
		printf ("verifyIndirect failed\n");
		return 0;
	}

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

	//printf ("Register 98 = %x\n", readDirectReg(slic_dev, 98));
	//printf ("Register 99 = %x\n", readDirectReg(slic_dev, 99));

	// enable IRQ
	writeDirectReg(slic_dev, 21, 0x00);
	writeDirectReg(slic_dev, 22, 0x02);
	writeDirectReg(slic_dev, 23, 0x01);
	
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

/*
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

	fclose(pslic_dev->sf);

	return EXIT_SUCCESS;
}
*/
