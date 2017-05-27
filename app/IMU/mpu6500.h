#ifndef __MPU6500_H__
#define __MPU6500_H__

#include <rtthread.h>
#include <stm32f4xx.h>
#include "stm32f40x_spi.h"

// MPU 6500 registers
#define MPUREG_WHOAMI			                    0x75
#define MPUREG_SMPLRT_DIV		                    0x19
#define MPUREG_CONFIG			                    0x1A
#define MPUREG_GYRO_CONFIG		                    0x1B
#define MPUREG_ACCEL_CONFIG		                    0x1C
#define MPUREG_FIFO_EN			                    0x23
#define MPUREG_INT_PIN_CFG		                    0x37
#define MPUREG_INT_ENABLE		                    0x38
#define MPUREG_INT_STATUS		                    0x3A
#define MPUREG_ACCEL_XOUT_H		                    0x3B
#define MPUREG_ACCEL_XOUT_L		                    0x3C
#define MPUREG_ACCEL_YOUT_H		                    0x3D
#define MPUREG_ACCEL_YOUT_L		                    0x3E
#define MPUREG_ACCEL_ZOUT_H		                    0x3F
#define MPUREG_ACCEL_ZOUT_L		                    0x40
#define MPUREG_TEMP_OUT_H		                    0x41
#define MPUREG_TEMP_OUT_L		                    0x42
#define MPUREG_GYRO_XOUT_H		                    0x43
#define MPUREG_GYRO_XOUT_L		                    0x44
#define MPUREG_GYRO_YOUT_H		                    0x45
#define MPUREG_GYRO_YOUT_L		                    0x46
#define MPUREG_GYRO_ZOUT_H		                    0x47
#define MPUREG_GYRO_ZOUT_L		                    0x48
#define MPUREG_USER_CTRL		                    0x6A
#define MPUREG_PWR_MGMT_1		                    0x6B
#define MPUREG_PWR_MGMT_2		                    0x6C
#define MPUREG_FIFO_COUNTH		                    0x72
#define MPUREG_FIFO_COUNTL		                    0x73
#define MPUREG_FIFO_R_W			                    0x74
#define MPUREG_PRODUCT_ID		                    0x0C
#define MPUREG_TRIM1			                    0x0D
#define MPUREG_TRIM2			                    0x0E
#define MPUREG_TRIM3			                    0x0F
#define MPUREG_TRIM4			                    0x10

// Configuration bits MPU 3000 and MPU 6500 (not revised)
#define BIT_SLEEP			                        0x40
#define BIT_H_RESET			                        0x80
#define BITS_CLKSEL			                        0x07
#define MPU_CLK_SEL_PLLGYROX	                    0x01
#define MPU_CLK_SEL_PLLGYROZ	                    0x03
#define MPU_EXT_SYNC_GYROX		                    0x02
#define BITS_GYRO_ST_X			                    0x80
#define BITS_GYRO_ST_Y			                    0x40
#define BITS_GYRO_ST_Z			                    0x20

#define BITS_FS_250DPS			                    0x00
#define BITS_FS_500DPS			                    0x08
#define BITS_FS_1000DPS			                    0x10
#define BITS_FS_2000DPS			                    0x18

#define BITS_FS_2G                                  0x00
#define BITS_FS_4G                                  0x08
#define BITS_FS_8G                                  0x10
#define BITS_FS_16G                                 0x18
#define BITS_FS_MASK			                    0x18

#define BITS_DLPF_CFG_256HZ_NOLPF2	                0x00
#define BITS_DLPF_CFG_188HZ		                    0x01
#define BITS_DLPF_CFG_98HZ		                    0x02
#define BITS_DLPF_CFG_42HZ		                    0x03
#define BITS_DLPF_CFG_20HZ		                    0x04
#define BITS_DLPF_CFG_10HZ		                    0x05
#define BITS_DLPF_CFG_5HZ		                    0x06
#define BITS_DLPF_CFG_2100HZ_NOLPF	                0x07
#define BITS_DLPF_CFG_MASK		                    0x07
#define BIT_INT_ANYRD_2CLEAR	                    0x10
#define BIT_RAW_RDY_EN		                        0x01
#define BIT_I2C_IF_DIS			                    0x10
#define BIT_INT_STATUS_DATA		                    0x01
                                                  
#define READ_FLAG                                   0x80

// Product ID Description for MPU6500
// high 4 bits 	low 4 bits
// Product Name	Product Revision
#define MPU6500ES_REV_C4		                    0x14
#define MPU6500ES_REV_C5		                    0x15
#define MPU6500ES_REV_D6		                    0x16
#define MPU6500ES_REV_D7		                    0x17
#define MPU6500ES_REV_D8		                    0x18
#define MPU6500_REV_C4			                    0x54
#define MPU6500_REV_C5			                    0x55
#define MPU6500_REV_D6			                    0x56
#define MPU6500_REV_D7			                    0x57
#define MPU6500_REV_D8			                    0x58
#define MPU6500_REV_D9			                    0x59
#define MPU6500_REV_D10			                    0x5A

#define MPU6500_ACCEL_DEFAULT_RANGE_G			    8
#define MPU6500_ACCEL_DEFAULT_RATE			        1000
#define MPU6500_ACCEL_DEFAULT_DRIVER_FILTER_FREQ	30

#define MPU6500_GYRO_DEFAULT_RANGE_G			    8
#define MPU6500_GYRO_DEFAULT_RATE			        1000
#define MPU6500_GYRO_DEFAULT_DRIVER_FILTER_FREQ		30

#define MPU6500_DEFAULT_ONCHIP_FILTER_FREQ		    42

#define MPU6500_ONE_G					            9.80665f

/*
  the MPU6500 can only handle high SPI bus speeds on the sensor and
  interrupt status registers. All other registers have a maximum 1MHz
  SPI speed
 */
#define MPU6500_LOW_BUS_SPEED				   1000*1000      // 1MHz���������û����Ĵ���
#define MPU6500_HIGH_BUS_SPEED				   11*1000*1000   // ����������10.4 MHz, MPU6500SPI����
#define GYRO_INTERATIONS                       2000

#define CONSTANTS_ONE_G                   9.81
#define PI                            3.141592654
#define DEG_TO_RAD                    PI/180

#ifdef __cplusplus

class Vector3f
{
	public:
		float x;
	  float y;
		float z;
	  Vector3f() { this->x = this->y = this->z = 0; };
	  Vector3f(float x, float y, float z) { this->x = x; this->y = y; this->z = z; }; 
};

class Vector3i
{
	public:
		int x;
	  int y;
	  int z;
};

class MPU6500
{
private:
    struct rt_spi_device    *mpu6500_device;
    uint8_t                  mpu_recv_buf[14];
    uint8_t                  mpu_send_buf[1];
    const char              *mpu_device_name;

public:
    MPU6500(const char *mpu_name);
    float                 accel_scale;
    float                 gyro_scale;
    
    Vector3f              gyro_offset;
    // calibration parameter
    Vector3f           accel_offs;
	  Vector3f           accel_T;
    rt_uint8_t               imu_rotation;

    Vector3i              Acc_I16;
	  Vector3i              Gyro_I16;
    int16_t               Tempreature;
    
    Vector3f              Gyro_rad;
    Vector3f              Acc_ms2;
    Vector3f              Acc_correct;

    float                 temp_deg;

    // MPU6500��ʼ��
    void mpu_init(uint8_t sample_rate_div,uint8_t low_pass_filter);
    void disable_I2C();
    void reSetMPU6500();
    uint8_t getClockSource();
    void setClockSource(uint8_t source);
    void setSampleRate(uint8_t sample_rate);
    uint8_t getSampleRate();
    void setDLPF(uint8_t low_pass);
    void disableInterrupt();
    uint8_t set_acc_scale(uint8_t scale);
    uint16_t set_gyro_scale(uint8_t scale);
    
    void gyroOffsetCalibration(void);
    
    void read_Data(void);
    void read_acc(int16_t *x,int16_t *y,int16_t *z);
    void read_gyro(int16_t *x,int16_t *y,int16_t *z);

    void set_rotation(uint8_t rotation);
    // ����SPI����
    rt_bool_t testConnect();
};

#endif

#endif
