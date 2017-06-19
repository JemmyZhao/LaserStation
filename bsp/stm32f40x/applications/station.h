#include "mpu6500.h"
#include "ESP8266.h"
#include "Script/MyESPListener.h"
#include "stdio.h"
#include "chProtocol.h"
#include <rtdevice.h>
#include "imu.h"


class Station
{
	private:
		MPU6500* imu;
	  ESP8266* esp;
		MyESPListener* espListener;
	  SerialPort* uart;

	
	public:
		
		Station();
	  
		Attitude* attitude;
	  Codec* codec;
		uint8_t tx_buf[MAX_MAV_LENTH];
	
	  void init_station();
	  void update_compute(void);
	  void run_motor(void);
	  void cal_position(void);
	  void get_attitude(float* quat);
	
	  void send_attitude(uint32_t delay_time);
};
