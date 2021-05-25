# 16CH_IR_Detector

## 设计目的

用于巡线小车的16路红外灰度传感器，输出为模拟量灰度，也可选择将ADC芯片拆除后安装比较器芯片和移位寄存器芯片，作为16路单阈值开关量传感器使用。

## 系统概况

### 光电传感器

光电传感器使用亿光的ITR8307/S17/TR8(B)，表贴一体红外对管，自带红外滤光，实测不会受到可见光的影响

### ADC

采用德州仪器(Texas Instruments, TI)的ADS7953 ADC芯片。该芯片为12位ADC，最大采样率为1MSps，有16路MUX输入，数字接口为SPI，本次采用的为TSSOP38封装。

### 基准

PCB上预留了REF5030(3.0V，实际采用)/REF5025(2.5V)/LT1009(2.5V)的芯片位置，并预留微调电位器位置，方便根据采购情况使用不同的基准芯片和对电压进行微调

### 比较器/移位寄存器

预留4个LM339比较器(共16路)和74HC165移位寄存器(2片级联，共16路输入)芯片位置，如有需要可将ADC部分元件空贴，将比较器和移位寄存器部分的元件装上，既可作为使用串行通信的16路单阈值的开关量传感器使用，可减轻MCU的运算负担。本次比赛中未使用此功能

### 电源

GND未进行划分，VCC划分为AVCC和DVCC，采用简单的PI滤波器进行隔离

## 使用方法

### 基本原理

#### 模拟量输出模式

通过SPI接口(软硬件均可)向ADS7953发送读取通道指令，读取1-16号输入的电压，即为1-16个传感器的灰度模拟量

#### 单阈值开关量输出模式

(未做验证)由于74HC165的时序也符合SPI，可直接利用SPI接口读取16位的方式进行通信，每一位对应一个传感器的输出

## 驱动使用说明

此处仅提供模拟量传感器的驱动，基于STM32平台，使用LL库操作SPI和GPIO

### Structure

#### 	ADS7953_Typedef

​		定义于ADS7953.h中

##### 	数据域

###### 		SPI_TypeDef* SPI_Port;

​			硬件SPI的SPI端口号

###### 		GPIO_TypeDef* NSS_GPIO_Port;

​			NSS引脚的所在的GPIO端口组

###### 		uint32_t NSS_GPIO_Pin;

​			NSS所在GPIO的Pin号

###### 		enum {ADS7953_Range_VREF=0, ADS7953_Range_2VREF=1} ADC_Range;

​			ADC的输入范围参数，决定满量程为VREF或者2倍VREF





### API

#### void ADS7953_ReadChannels(ADS7953_Typedef* ADCx, uint16_t bfr[16], uint16_t channel)

##### 功能

​	读取ADS7953的通道输入电压

##### 参数

###### 	ADS7953_Typedef* ADCx

​		ADS7953的定义结构体，包含ADS7953的配置参数

###### 	uint16_t bfr[16]

​		用于存储通道数据的数组，对应编号通道的数据存放在该数组对应下标处，如CH11的数据将在bfr[11]中存储

###### 	uint16_t channel

​		通道掩码输入

​		可选：

​			ADS7953_Channel_0

​			ADS7953_Channel_1

​			ADS7953_Channel_2

​			ADS7953_Channel_3

​			ADS7953_Channel_4

​			ADS7953_Channel_5

​			ADS7953_Channel_6

​			ADS7953_Channel_7

​			ADS7953_Channel_8

​			ADS7953_Channel_9

​			ADS7953_Channel_10

​			ADS7953_Channel_11

​			ADS7953_Channel_12

​			ADS7953_Channel_13

​			ADS7953_Channel_14

​			ADS7953_Channel_15

​			ADS7953_Channel_All

##### 返回值

​	无

##### 说明

​	掩码可以使用或运算符的形式输入，如 ADS7953_Channel_8 | ADS7953_Channel_14，将会读取8和14号通道

​	输出结果在数组中与下标一一对应，例如即使只读取一个ADS7953_Channel_14，也会将数据存储在bfr[14]中，而不是bfr[0]

##### 使用范例

```c
//SPI should be initialized before using
uint16_t ADS7953_F_bfr[16];

ADS7953_Typedef ADS7953_F;
ADS7953_F.ADC_Range = ADS7953_Range_VREF;
ADS7953_F.SPI_Port = SPI6;
ADS7953_F.NSS_GPIO_Pin = SPI6_NE1_Pin;
ADS7953_F.NSS_GPIO_Port = SPI6_NE1_GPIO_Port;

//get all 16 channels' data
ADS7953_ReadChannels(&ADS7953_F, ADS7953_F_bfr, ADS7953_Channel_All);
```





### Typedef

#### ADS7953_Channel_0

ADC通道0的掩码

#### ADS7953_Channel_1

ADC通道1的掩码

#### ADS7953_Channel_2

ADC通道2的掩码

#### ADS7953_Channel_3

ADC通道3的掩码

#### ADS7953_Channel_4

ADC通道4的掩码

#### ADS7953_Channel_5

ADC通道5的掩码

#### ADS7953_Channel_6

ADC通道6的掩码

#### ADS7953_Channel_7

ADC通道7的掩码

#### ADS7953_Channel_8

ADC通道8的掩码

#### ADS7953_Channel_9

ADC通道9的掩码

#### ADS7953_Channel_10

ADC通道10的掩码

#### ADS7953_Channel_11

ADC通道11的掩码

#### ADS7953_Channel_12

ADC通道12的掩码

#### ADS7953_Channel_13

ADC通道13的掩码

#### ADS7953_Channel_14

ADC通道14的掩码

#### ADS7953_Channel_15

ADC通道15的掩码

#### ADS7953_Channel_All

ADC全部通道的掩码





## 驱动移植说明

### 需要修改的函数

#### __INLINE void ADS7953_SPI_Enable(ADS7953_Typedef* ADCx)

##### 	功能

​		ADS7953使能(NSS线拉低)

##### 	参数

###### 		ADS7953_Typedef* ADCx

​			ADS7953配置结构体，存储了ADS7953的配置

##### 	返回值

​		无

#### __INLINE void ADS7953_SPI_Disable(ADS7953_Typedef* ADCx)

##### 	功能

​		ADS7953失能(NSS线拉高)

##### 	参数

###### 		ADS7953_Typedef* ADCx

​			ADS7953配置结构体，存储了ADS7953的配置

##### 	返回值

​		无

#### uint16_t ADS7953_SPI_ReadWriteFrame(ADS7953_Typedef* ADCx, uint16_t TxDat)

##### 	功能

​		ADS7953的SPI读写函数，向ADS7953发送或接收一帧(16bit)数据

##### 	参数

###### 		ADS7953_Typedef* ADCx

​			ADS7953配置结构体，存储了ADS7953的配置

###### 		uint16_t TxDat

​			发送给ADS7953的16bit数据

##### 	返回值

​		从ADS7953接收到的16bit数据

### 外部调用的函数

#### void delay(uint32_t t)

##### 	功能

​		简单的延时函数，用于提供ADS7953读取每一帧之间的延时

​		此处延时没有使用精确的时间单位，与芯片实际运行速度有关，可以修改ADS7953_INTER_FRAME_DELAY的值

​		在SPI速度不高时可以取消，请根据具体应用环境决定

##### 	参数

###### 		uint32_t t

​			延时时长

##### 	返回值

​		无

## 其他说明

