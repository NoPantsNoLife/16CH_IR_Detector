#ifndef __ADS7953_H__
#define __ADS7953_H__


#include "main.h"

typedef struct
{
	SPI_TypeDef* SPI_Port;
	GPIO_TypeDef* NSS_GPIO_Port;
	uint32_t NSS_GPIO_Pin;
	enum {ADS7953_Range_VREF=0, ADS7953_Range_2VREF=1} ADC_Range;
} ADS7953_Typedef;

#define ADS7953_INTER_FRAME_DELAY 2

#define ADS7953_Channel_0 0x1
#define ADS7953_Channel_1 0x2
#define ADS7953_Channel_2 0x4
#define ADS7953_Channel_3 0x8
#define ADS7953_Channel_4 0x10
#define ADS7953_Channel_5 0x20
#define ADS7953_Channel_6 0x40
#define ADS7953_Channel_7 0x80
#define ADS7953_Channel_8 0x100
#define ADS7953_Channel_9 0x200
#define ADS7953_Channel_10 0x400
#define ADS7953_Channel_11 0x800
#define ADS7953_Channel_12 0x1000
#define ADS7953_Channel_13 0x2000
#define ADS7953_Channel_14 0x4000
#define ADS7953_Channel_15 0x8000
#define ADS7953_Channel_All 0xffff

void ADS7953_ReadChannels(ADS7953_Typedef* ADCx, uint16_t bfr[16], uint16_t channel);


#endif