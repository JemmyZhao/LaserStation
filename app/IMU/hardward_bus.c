#include "hardward_bus.h"
//#include "gpio.h"

#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_dma.h"

/*** SPI1 BUS and device
SPI1_MOSI: PA7
SPI1_MISO: PA6
SPI1_SCK : PA5
SPI1_NSS : PA4
*/
void rt_hw_spi1_init(void)
{
	/* register SPI bus */
	static struct stm32_spi_bus stm32_spi;
	
	//SPI_Cmd(SPI1, DISABLE);

  /* SPI1 configure */
  {
      GPIO_InitTypeDef GPIO_InitStructure;

      /* Enable GPIO Periph clock */
      RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA , ENABLE);
        
      GPIO_PinAFConfig(GPIOA, GPIO_PinSource5, GPIO_AF_SPI1);
      GPIO_PinAFConfig(GPIOA, GPIO_PinSource6, GPIO_AF_SPI1);
      GPIO_PinAFConfig(GPIOA, GPIO_PinSource7, GPIO_AF_SPI1);

      GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;
      GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
      GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
      GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL; 

      /* Configure SPI1 pins */
      GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
      GPIO_Init(GPIOA, &GPIO_InitStructure);
      GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
      GPIO_Init(GPIOA, &GPIO_InitStructure);
      GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
      GPIO_Init(GPIOA, &GPIO_InitStructure);
  } /* SPI1 configuration */

	/* register SPI1 */
	stm32_spi_register(SPI1, &stm32_spi, "spi1");

  /* attach spi10 */
  {
      static struct rt_spi_device rt_spi_device_10;    //it must be add static
      static struct stm32_spi_cs  stm32_spi_cs_10;     //it must be add static
        
      stm32_spi_cs_10.GPIOx    = GPIOB;
      stm32_spi_cs_10.GPIO_Pin = GPIO_Pin_8;
        
      RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);

      GPIO_InitTypeDef GPIO_InitStructure;
      GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;
      GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
      GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
      GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
      GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_8;
      GPIO_Init(GPIOB, &GPIO_InitStructure);
      GPIO_SetBits(GPIOB, GPIO_Pin_8);
        
      rt_spi_bus_attach_device(&rt_spi_device_10, "mpu65", "spi1", (void*)&stm32_spi_cs_10);//set spi_device->bus
      /* config spi */
      {
          struct rt_spi_configuration cfg;
          cfg.data_width = 8;
          cfg.mode = RT_SPI_MODE_3 | RT_SPI_MSB; /* SPI Compatible Modes 3 and SPI_FirstBit_MSB in lis302dl datasheet */
            
          //APB2=168M/2=84M, SPI1 = 84/2,4,8,16,32 = 42M, 21M, 10.5M, 5.25M, 2.625M ...
          cfg.max_hz = 1000*11000; /* SPI_BaudRatePrescaler_16=84000000/16=5.25MHz. The max_hz of lis302dl is 10MHz in datasheet */ 
          rt_spi_configure(&rt_spi_device_10, &cfg);
      } /* config spi */    
  } /* attach spi10 */  
}

void rt_hw_spi2_init(void)
{
	/* register SPI bus */
	static struct stm32_spi_bus stm32_spi;
	
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE); 
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_Init(GPIOB,&GPIO_InitStructure);

	/* register SPI1 */
	stm32_spi_register(SPI2, &stm32_spi, "spi2");

    static struct rt_spi_device rt_spi_device_20;
    static struct stm32_spi_cs  stm32_spi_cs_20;
    
    stm32_spi_cs_20.GPIOx = GPIOB;
    stm32_spi_cs_20.GPIO_Pin = GPIO_Pin_12;

    rt_spi_bus_attach_device(&rt_spi_device_20, "hmc", "spi2", (void*)&stm32_spi_cs_20);
    
    struct rt_spi_configuration cfg;
    cfg.data_width = 8;                        // 数据宽度8位
    cfg.mode = RT_SPI_MODE_3 | RT_SPI_MSB;     // SPI Compatible Modes 3
    
    //SPI3 = 84M/4,8,16,32 = 21M, 10.5M, 5.25M,
    cfg.max_hz = 1000*6000; /* 8000kbit/s */
    rt_spi_configure(&rt_spi_device_20, &cfg);
}


