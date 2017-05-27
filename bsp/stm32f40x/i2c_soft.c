#include "i2c_soft.h"

rt_err_t rt_i2c_b_add_bus(struct rt_i2c_bus_device *bus,
                            const char               *bus_name);

void RCC_GPIO_init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	I2C_InitTypeDef I2C_InitStruct;
	
	RCC_APB1PeriphClockCmd(I2C1_APB, ENABLE);
	RCC_AHB1PeriphClockCmd(I2C1_AHB, ENABLE);
	
	GPIO_InitStruct.GPIO_Pin = I2C1_SCL | I2C1_SDA; // we are going to use PB6 and PB7
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;			// set pins to alternate function
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;		// set GPIO speed
	GPIO_InitStruct.GPIO_OType = GPIO_OType_OD;			// set output to open drain --> the line has to be only pulled low, not driven high
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
	
	GPIO_Init(I2C1_GPIO, &GPIO_InitStruct);
	
		// Connect I2C1 pins to AF  
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource6, GPIO_AF_I2C1);	// SCL
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource7, GPIO_AF_I2C1); // SDA
	
		// configure I2C1 
	I2C_InitStruct.I2C_ClockSpeed = 100000; 		// 100kHz
	I2C_InitStruct.I2C_Mode = I2C_Mode_I2C;			// I2C mode
	I2C_InitStruct.I2C_DutyCycle = I2C_DutyCycle_2;	// 50% duty cycle --> standard
	I2C_InitStruct.I2C_OwnAddress1 = 0x00;			// own address, not relevant in master mode
	I2C_InitStruct.I2C_Ack = I2C_Ack_Disable;		// disable acknowledge when reading (can be changed later on)
	I2C_InitStruct.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit; // set address length to 7 bit addresses
	I2C_Init(I2C1, &I2C_InitStruct);				// init I2C1
	
	// enable I2C1
	//I2C_Cmd(I2C1, ENABLE);
}

void DMA_init(void)
{
	DMA_InitTypeDef DMA_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1, ENABLE);
	
	DMA_ClearFlag(DMA1_Stream0,
                DMA_FLAG_TCIF0 | DMA_FLAG_FEIF0 |
                DMA_FLAG_DMEIF0 | DMA_FLAG_TEIF0 |
                DMA_FLAG_HTIF0);

  DMA_Cmd(DMA1_Stream0, DISABLE);
  DMA_DeInit(DMA1_Stream0);

  DMA_InitStructure.DMA_Channel = DMA_Channel_1;
  /* Address of data register. */
  DMA_InitStructure.DMA_PeripheralBaseAddr = I2C1_BASE + I2C_Register_DR;
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
  DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
  DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;
  DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Enable;
  DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;
  DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
  DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;

  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;
  DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)&mpu6050_reg_buffer;
  DMA_InitStructure.DMA_BufferSize = 14;
  DMA_DeInit(DMA1_Stream0);
  DMA_Init(DMA1_Stream0, &DMA_InitStructure);
	
	DMA_ITConfig(DMA1_Stream0, DMA_IT_TC, DISABLE);
  NVIC_InitStructure.NVIC_IRQChannel = DMA1_Stream0_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 10;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
  DMA_ITConfig(DMA1_Stream0, DMA_IT_TC, ENABLE);
	
	
	
}
	


rt_err_t rt_hw_i2c1_init(void)
{
	static struct rt_i2c_bus_device stm32_i2c;
	
	RCC_GPIO_init();
	DMA_init();
	
	rt_memset((void *)&stm32_i2c, 0, sizeof(struct rt_i2c_bus_device));
	//stm32_i2c.priv = (void *)&stm32_
	
	return rt_i2c_b_add_bus(&stm32_i2c, "i2c1");
	//return rt_i2c_bus_device_register(&stm32_i2c, "i2c1");
}

static rt_size_t i2c_send_bytes(struct rt_i2c_bus_device *bus,
                                struct rt_i2c_msg        *msg)
{
    rt_int32_t ret;
    rt_size_t bytes = 0;
    const rt_uint8_t *ptr = msg->buf;
    rt_int32_t count = msg->len;
    rt_uint16_t ignore_nack = msg->flags & RT_I2C_IGNORE_NACK;
    
	  while (count > 0)
    {
			I2C_SendData(MPU6050_I2C, *ptr);
			ptr++;
    }
    return bytes;
}

static rt_size_t i2c_recv_bytes(struct rt_i2c_bus_device *bus,
                                struct rt_i2c_msg        *msg)
{
    rt_int32_t val;
    rt_int32_t bytes = 0;   /* actual bytes */
    rt_uint8_t *ptr = msg->buf;
    rt_int32_t count = msg->len;
    const rt_uint32_t flags = msg->flags;

    while(count)
    {
        if (count == 1)
        {
            /* Disable Acknowledgement */
            I2C_AcknowledgeConfig(MPU6050_I2C, DISABLE);

            /* Send STOP Condition */
            I2C_GenerateSTOP(MPU6050_I2C, ENABLE);
        }
        /* Test on EV7 and clear it */
        if (I2C_CheckEvent(MPU6050_I2C, I2C_EVENT_MASTER_BYTE_RECEIVED))
        {
            /* Read a byte from the MPU6050 */
            *ptr = I2C_ReceiveData(MPU6050_I2C);

            /* Point to the next location where the byte read will be saved */
             ptr++;

            /* Decrement the read bytes counter */
             count--;
        }
    }
    return bytes;
}

static rt_size_t i2c_xfer(struct rt_i2c_bus_device *bus,
                              struct rt_i2c_msg         msgs[],
                              rt_uint32_t               num)
{
    struct rt_i2c_msg *msg;
    struct rt_i2c_bit_ops *ops = bus->priv;
    rt_int32_t i, ret;
    rt_uint16_t ignore_nack;
	
    /* While the bus is busy */
    while (I2C_GetFlagStatus(MPU6050_I2C, I2C_FLAG_BUSY));

    /* Send START condition */
    I2C_GenerateSTART(MPU6050_I2C, ENABLE);
	
    for (i = 0; i < num; i++)
    {
        msg = &msgs[i];
        ignore_nack = msg->flags & RT_I2C_IGNORE_NACK;
        if (!(msg->flags & RT_I2C_NO_START))
        {
            if (i)
            {
                I2C_GenerateSTART(MPU6050_I2C, ENABLE);
            }
						    /* Test on EV5 and clear it */
            while (!I2C_CheckEvent(MPU6050_I2C, I2C_EVENT_MASTER_MODE_SELECT));
            //ret = i2c_bit_send_address(bus, msg);
						I2C_Send7bitAddress(MPU6050_I2C, msg->addr, I2C_Direction_Transmitter);
            /* Test on EV6 and clear it */
            while (!I2C_CheckEvent(MPU6050_I2C, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));
						if ((ret != RT_EOK) && !ignore_nack)
            {
                rt_kprintf("receive NACK from device addr 0x%02x msg %d\n",
                        msgs[i].addr, i);
                goto out;
            }
        }
        if (msg->flags & RT_I2C_RD)
        {
            //ret = i2c_recv_bytes(bus, msg);
            if (ret >= 1)
                rt_kprintf("read %d byte%s\n", ret, ret == 1 ? "" : "s");
            if (ret < msg->len)
            {
                if (ret >= 0)
                    ret = -RT_EIO;
                goto out;
            }
        }
        else
        {
            ret = i2c_send_bytes(bus, msg);
            if (ret >= 1)
                rt_kprintf("write %d byte%s\n", ret, ret == 1 ? "" : "s");
            if (ret < msg->len)
            {
                if (ret >= 0)
                    ret = -RT_ERROR;
                goto out;
            }
        }
    }
    ret = i;

out:
    I2C_GenerateSTOP(MPU6050_I2C, ENABLE);

    return ret;
}

static const struct rt_i2c_bus_device_ops i2c_bit_bus_ops =
{
    i2c_xfer,
    RT_NULL,
    RT_NULL
};

rt_err_t rt_i2c_b_add_bus(struct rt_i2c_bus_device *bus,
                            const char               *bus_name)
{
    bus->ops = &i2c_bit_bus_ops;

    return rt_i2c_bus_device_register(bus, bus_name);
}

	
	