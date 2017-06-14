#include "mpu6500.h"
#include "ESP8266.h"
#include "Script/MyESPListener.h"
#include "stdio.h"
#include "chProtocol.h"


class Station
{
	private:
		MPU6500* imu;
	  ESP8266* esp;
		MyESPListener* espListener;
	  SerialPort* uart;
	
	public:
		Station();
	
	  void update_attitude(void);
	  void run_motor(void);
	  void cal_position(void);
};
