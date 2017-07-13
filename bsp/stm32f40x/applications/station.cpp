#include "station.h"


Station::Station()
{
		//station = Station();
	imu = new MPU6500("mp65");
  attitude = new Attitude(this->imu);
	codec = new Codec();
	//wifi test: init and log
	uart = SerialPort::getSerialPort(SerialPort::TUSART1);
	espListener = new MyESPListener(NULL);
	esp = new ESP8266(SerialPort::TUSART1, espListener);
	
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

void Station::init_station()
{
	attitude->start_init();
	esp->start();

}

void Station::update_compute(void)
{
	attitude->update();
}

void Station::get_attitude(float* quat)
{
	quat[0] = attitude->q0;
	quat[1] = attitude->q1;
	quat[2] = attitude->q2;
	quat[3] = attitude->q3;
}

void Station::send_attitude(uint32_t delay_time)
{
	if(espListener->isCanSendInTTMode() == FALSE) return ;
	uint32_t len;
	len = codec->attitude_quat_packing(tx_buf, 
	                             delay_time, attitude->q0, attitude->q1, attitude->q2, attitude->q3,
	                             attitude->rad2deg(attitude->Gyro_af.x), 
	                             attitude->rad2deg(attitude->Gyro_af.y), 
	                             attitude->rad2deg(attitude->Gyro_af.z));
	uart->sendDate((char*)(tx_buf), len);
	
}






