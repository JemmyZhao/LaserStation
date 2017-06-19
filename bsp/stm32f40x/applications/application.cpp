/*
 * File      : application.c
 * This file is part of RT-Thread RTOS
 * COPYRIGHT (C) 2006, RT-Thread Development Team
 *
 * The license and distribution terms for this file may be
 * found in the file LICENSE in this distribution or at
 * http://www.rt-thread.org/license/LICENSE
 *
 * Change Logs:
 * Date           Author       Notes
 * 2009-01-05     Bernard      the first version
 * 2014-04-27     Bernard      make code cleanup. 
 */

#include <board.h>
#include <rtthread.h>
#include "station.h"


#ifdef  __cplusplus
extern "C" {
#endif


#ifdef RT_USING_LWIP
#include <lwip/sys.h>
#include <lwip/api.h>
#include <netif/ethernetif.h>
#include "stm32f4xx_eth.h"
#endif

#ifdef RT_USING_GDB
#include <gdb_stub.h>
#endif

	


//MPU6500 imu("mp65");
Station *laser_station;// = new Station();
	
void init_all()
{
	//imu.mpu_init(0, BITS_DLPF_CFG_20HZ);
	//imu.set_acc_scale(BITS_FS_8G);
	//imu.set_gyro_scale(BITS_FS_2000DPS);
	
	//imu.gyroOffsetCalibration();
	laser_station = new Station();
  laser_station->init_station();
}
	
void station_compute_update_entry(void* parameter)
{
	rt_thread_delay(2000);
	while(1)
	{
		laser_station->update_compute();
		rt_thread_delay(1);
	}
}

void send_data_to_computer_entry(void* parameter)
{
	rt_tick_t now_time, last_time;
	rt_thread_delay(2000);
	float quat[4];
	now_time = rt_tick_get();
	while(1)
	{
		last_time = now_time;
		now_time = rt_tick_get();
		laser_station->send_attitude((uint32_t)(now_time-last_time));
		laser_station->get_attitude(quat);
		rt_kprintf("%d \t %d \t %d \t %d\n", (int)(1000*quat[0]), (int)(1000*quat[1]), (int)(1000*quat[2]), (int)(1000*quat[3]));
		rt_thread_delay(10);
	}

}


void rt_init_thread_entry(void* parameter)
{
    /* GDB STUB */
	  init_all();
#ifdef RT_USING_GDB
    gdb_set_device("uart6");
    gdb_start();
#endif
    /* LwIP Initialization */
#ifdef RT_USING_LWIP
    {
        extern void lwip_sys_init(void);

        /* register ethernetif device */
        eth_system_device_init();

        rt_hw_stm32_eth_init();

        /* init lwip system */
        lwip_sys_init();
        rt_kprintf("TCP/IP initialized!\n");
    }
#endif
}

int rt_application_init()
{
    rt_thread_t tid, station_compute_update, send_data_to_computer;
	

    tid = rt_thread_create("init",
        rt_init_thread_entry, RT_NULL,
        2048, RT_THREAD_PRIORITY_MAX/3, 20);

    if (tid != RT_NULL)
        rt_thread_startup(tid);
		
		
		station_compute_update = rt_thread_create("station_compute_update",
		                                          station_compute_update_entry, RT_NULL,
		                                          1024, RT_THREAD_PRIORITY_MAX/3+1, 10);
		if(station_compute_update != RT_NULL)
			rt_thread_startup(station_compute_update);
		
		send_data_to_computer = rt_thread_create("send_data_to_computer",
		                                         send_data_to_computer_entry, RT_NULL,
		                                         1024, RT_THREAD_PRIORITY_MAX/3+2, 20);
		if(send_data_to_computer != RT_NULL)
			rt_thread_startup(send_data_to_computer);
		

		

		
		

		
		

    return 0;
}


#ifdef  __cplusplus
}  
#endif



/*@}*/
