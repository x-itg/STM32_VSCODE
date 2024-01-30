#ifndef _SPI_HW_
#define	_SPI_HW_
#include "spi_core.h"
#include "sys.h"
//stm32f1 hardware spi
extern void stm32f1xx_spi_init(struct spi_bus_device *spi0,unsigned char byte_size0);

#endif
