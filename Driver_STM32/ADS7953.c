#include "ADS7953.h"

#include "spi.h"
#include "delay.h"


__INLINE void ADS7953_SPI_Enable(ADS7953_Typedef* ADCx);
__INLINE void ADS7953_SPI_Disable(ADS7953_Typedef* ADCx);
uint16_t ADS7953_SPI_ReadWriteFrame(ADS7953_Typedef* ADCx, uint16_t TxDat);


void ADS7953_ReadChannels(ADS7953_Typedef* ADCx, uint16_t bfr[16], uint16_t channel)
{
	uint8_t chNum = 0, i;
	uint16_t RxFrame, ch=channel;
	while(ch)
	{
		if (ch & 0x1)
			chNum++;
		ch >>= 1;
	}
	
	ADS7953_SPI_Enable(ADCx);
	ADS7953_SPI_ReadWriteFrame(ADCx, 0x8000); //Enter Auto-1 Programming Mode
	ADS7953_SPI_Disable(ADCx);
	delay(ADS7953_INTER_FRAME_DELAY);
	
	ADS7953_SPI_Enable(ADCx);
	ADS7953_SPI_ReadWriteFrame(ADCx, channel); //Send Channel Mask
	ADS7953_SPI_Disable(ADCx);
	delay(ADS7953_INTER_FRAME_DELAY);
	
	for(i=0; i<chNum; i++)
	{
		ADS7953_SPI_Enable(ADCx);
		RxFrame = ADS7953_SPI_ReadWriteFrame(ADCx, ADCx->ADC_Range == ADS7953_Range_2VREF ? 0x2840 : 0x2800);
		ADS7953_SPI_Disable(ADCx);
		bfr[RxFrame >> 12] = RxFrame & 0xfff;
		delay(ADS7953_INTER_FRAME_DELAY);
	}
}


uint16_t ADS7953_SPI_ReadWriteFrame(ADS7953_Typedef* ADCx, uint16_t TxDat)
{
	uint16_t Rx;
	Rx  = SPI_ReadWriteByte(ADCx->SPI_Port, TxDat >> 8) << 8;
	Rx |= SPI_ReadWriteByte(ADCx->SPI_Port, TxDat & 0xff);
	return Rx;
}


__INLINE void ADS7953_SPI_Enable(ADS7953_Typedef* ADCx)
{
	LL_GPIO_ResetOutputPin(ADCx->NSS_GPIO_Port, ADCx->NSS_GPIO_Pin);
}


__INLINE void ADS7953_SPI_Disable(ADS7953_Typedef* ADCx)
{
	LL_GPIO_SetOutputPin(ADCx->NSS_GPIO_Port, ADCx->NSS_GPIO_Pin);
}
