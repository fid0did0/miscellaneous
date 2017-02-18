#include "si3210.h"

void waitForIndirectReg(slic_dev_str *slic_dev) {
	while (readDirectReg(slic_dev, I_STATUS));
}

unsigned char readDirectReg(slic_dev_str *slic_dev, unsigned char address) {
	unsigned char cmd;

	cmd = 0x80|address;
	fputc (cmd, slic_dev->sf);
	
	return (unsigned char)(fgetc(slic_dev->sf));
}

void writeDirectReg(slic_dev_str *slic_dev, unsigned char address, unsigned char data) {
	unsigned char cmd;
	
	cmd = address;
	fputc (cmd, slic_dev->sf);
	cmd = data;
	fputc (cmd, slic_dev->sf);
}

unsigned short readIndirectReg(slic_dev_str *slic_dev, unsigned char address) {
	
	waitForIndirectReg(slic_dev);
	writeDirectReg(slic_dev, IAA,address); 
	waitForIndirectReg(slic_dev);
	return ( readDirectReg(slic_dev, IDA_LO) | (readDirectReg (slic_dev, IDA_HI))<<8);
}

void writeIndirectReg(slic_dev_str *slic_dev, unsigned char address, unsigned short data) {

	waitForIndirectReg(slic_dev);
	writeDirectReg(slic_dev, IDA_LO,(unsigned char)(data & 0xFF));
	writeDirectReg(slic_dev, IDA_HI,(unsigned char)((data & 0xFF00)>>8));
	writeDirectReg(slic_dev, IAA,address);
}
