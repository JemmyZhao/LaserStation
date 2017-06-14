#include "station.h"

Station::Station()
{
		//station = Station();
	imu = new MPU6500("mp65");
	imu->mpu_init(0, BITS_DLPF_CFG_20HZ);
	imu->set_acc_scale(BITS_FS_8G);
	imu->set_gyro_scale(BITS_FS_2000DPS);
	
	imu->gyroOffsetCalibration();

	//wifi test: init and log
	//uart = SerialPort::getSerialPort(SerialPort::TUSART1);
	//espListener = new MyESPListener(NULL);
	//esp = new ESP8266(SerialPort::TUSART1, espListener);
	//esp->start();
	
	/*
	char log[100];
	while(true){
		if(espListener->isCanSendInTTMode()){
			//格式：@片子名字#x#y#z#w$\r\n
			sprintf(log, "@TestChip#%d#%d#%d#%d$\r\n\0", (int)(1 * 10000), (int)(0 * 10000), (int)(0* 10000), (int)(q0 * 10000));
			//q1 += 0.001f;
			uart->sendDate(log, strlen(log));
			rt_thread_delay(RT_TICK_PER_SECOND / 2);
		}
	}
	*/
}


int a = 0;
int b = 1;

void tran(int x, int y)
{
	int z;
	z = x;
	x = y;
	y = z;
}





