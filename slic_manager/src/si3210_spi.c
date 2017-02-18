#include "si3210.h"

void InitSpi(slic_dev_str *slic_dev) {	
	pthread_mutex_init(&(slic_dev->SpiMutex), NULL);
}

void DestroySpi(slic_dev_str *slic_dev) {
	pthread_mutex_destroy(&(slic_dev->SpiMutex));
}

void waitForIndirectReg(slic_dev_str *slic_dev) {
	unsigned char cmd;
	
	cmd = 0x80|I_STATUS;	
	pthread_mutex_lock (&(slic_dev->SpiMutex));
	do fputc(cmd, slic_dev->sf); while (fgetc(slic_dev->sf));		
	pthread_mutex_unlock (&(slic_dev->SpiMutex));
}

unsigned char readDirectReg(slic_dev_str *slic_dev, unsigned char address) {
	unsigned char cmd, ret;

	cmd = 0x80|address;
	pthread_mutex_lock (&(slic_dev->SpiMutex));
	fputc (cmd, slic_dev->sf);
	ret=(unsigned char)(fgetc(slic_dev->sf));
	pthread_mutex_unlock (&(slic_dev->SpiMutex));
	
	return ret;
}

void writeDirectReg(slic_dev_str *slic_dev, unsigned char address, unsigned char data) {
	unsigned char cmd;
	
	cmd = address;
	pthread_mutex_lock (&(slic_dev->SpiMutex));
	fputc (cmd, slic_dev->sf);
	cmd = data;
	fputc (cmd, slic_dev->sf);
	pthread_mutex_unlock (&(slic_dev->SpiMutex));
}

unsigned short readIndirectReg(slic_dev_str *slic_dev, unsigned char address) {
	unsigned short ret;
	
	waitForIndirectReg(slic_dev);
	writeDirectReg(slic_dev, IAA,address); 
	waitForIndirectReg(slic_dev);
	ret=( readDirectReg(slic_dev, IDA_LO) | (readDirectReg (slic_dev, IDA_HI))<<8);
	return ret;
}

void writeIndirectReg(slic_dev_str *slic_dev, unsigned char address, unsigned short data) {	
	
	waitForIndirectReg(slic_dev);	
	writeDirectReg(slic_dev, IDA_LO,(unsigned char)(data & 0xFF));
	writeDirectReg(slic_dev, IDA_HI,(unsigned char)((data & 0xFF00)>>8));
	writeDirectReg(slic_dev, IAA,address);	
}
