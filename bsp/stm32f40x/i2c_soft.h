#include <stm32f4xx.h>
#include <stm32f4xx_i2c.h>
#include "i2c.h"

#define MPU6050_I2C I2C1
#define I2C1_APB    RCC_APB1Periph_I2C1
#define I2C1_AHB    RCC_AHB1Periph_GPIOB
#define I2C1_SCL 		GPIO_Pin_6
#define I2C1_SDA 		GPIO_Pin_7
#define I2C1_GPIO 	GPIOB

static uint8_t mpu6050_reg_buffer[14];
	

rt_err_t rt_hw_i2c1_init(void);




	
	
	
