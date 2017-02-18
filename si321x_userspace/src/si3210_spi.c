#include <linux/ioctl.h>
#include <linux/spi/spidev.h>
#include <sys/ioctl.h>
#include "si3210.h"


void writeDirectRegs(slic_dev_str *slic_dev, unsigned char startaddr, unsigned char *data, unsigned int counts) {

	uint8_t		*tx_buffer   =  NULL;
	int		trlen = 2 * counts;
	unsigned int	k, currentaddr;

	struct spi_ioc_transfer transfer = {
		.tx_buf        = 0,
		.rx_buf        = 0,
		.len           = 0,
		.delay_usecs   = 0,
		.speed_hz      = 0,
		.bits_per_word = 0,
	};

	if (tx_buffer = malloc(trlen) == NULL) {
		fprintf(stderr, "%s: not enough memory to allocate two %d bytes buffers\n",
		                argv[0], trlen);
		exit(EXIT_FAILURE);
	}
	
	memset(tx_buffer, 0, blocksize);

	transfer.tx_buf = (unsigned long)tx_buffer;
	transfer.len = trlen;

	currentaddr=startaddr;
	for(k=0;k<counts;k++) {
		tx_buffer[2*k]=startaddr++;
		tx_buffer[2*k+1]=data[k];
	}

	if (ioctl(slic_dev_str->sf, SPI_IOC_MESSAGE(1), &transfer) < 0) {
		perror("SPI_IOC_MESSAGE");
		break;
	}

	free(tx_buffer);

	return EXIT_SUCCESS;
}

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
