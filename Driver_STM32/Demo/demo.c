#include "ADS7953.h"

void main()
{
	uint16_t ADS7953_F_bfr[16];  //buffer for ADC Data
	
	ADS7953_Typedef ADS7953_F;  //struct for ADS7953 Configuration
	ADS7953_F.ADC_Range = ADS7953_Range_VREF;
	ADS7953_F.SPI_Port = SPI6;
	ADS7953_F.NSS_GPIO_Pin = SPI6_NE1_Pin;
	ADS7953_F.NSS_GPIO_Port = SPI6_NE1_GPIO_Port;
	
	
	SPI_Init();
	//SPI should be initialized before using
	
	//you don't need to initialize ADS7953
	
	while(1)
	{

		ADS7953_ReadChannels(&ADS7953_F, ADS7953_F_bfr, ADS7953_Channel_All);
		//get all 16 channels' data
		
		//do something here
	}
}