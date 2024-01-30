#ifndef _COMMUNICATION_H_
#define _COMMUNICATION_H_


/*! Initializes the SPI communication peripheral. */
unsigned char AD7124_SPI_Init(unsigned char lsbFirst,
                       unsigned long clockFreq,
                       unsigned char clockPol,
                       unsigned char clockEdg);

/*! Reads data from SPI. */
unsigned char AD7124_SPI_Read(unsigned char slaveDeviceId,
                       unsigned char* data,
                       unsigned char bytesNumber);

/*! Writes data to SPI. */
unsigned char AD7124_SPI_Write(unsigned char slaveDeviceId,
                        unsigned char* data,
                        unsigned char bytesNumber);

#endif /* _COMMUNICATION_H_ */
