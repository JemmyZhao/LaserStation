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
//#include "SerialPort.h"
//#include <stdio.h>
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
Station *laser_station;
	
void init_all()
{
	laser_station = new Station();
	
}
	


void mpu6050_thread_entry(void* parameter)
{
		//station.update();
		
}

void print_thread_entry(void* parameter)
{
	 //station.printAccGyro();
}

	

void rt_init_thread_entry(void* parameter)
{
    /* GDB STUB */
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
	
	init_all();
}

int rt_application_init()
{
    rt_thread_t tid, mpu_thread, print_thread;
	

    tid = rt_thread_create("init",
        rt_init_thread_entry, RT_NULL,
        2048, RT_THREAD_PRIORITY_MAX/3, 20);

    if (tid != RT_NULL)
        rt_thread_startup(tid);
	
	
	
	mpu_thread = rt_thread_create("mpu",
		mpu6050_thread_entry, RT_NULL,
		2048, RT_THREAD_PRIORITY_MAX/3+3, 10);
	
	if (mpu_thread != RT_NULL)
		rt_thread_startup(mpu_thread);
	
	print_thread = rt_thread_create("pirnt",
		 print_thread_entry, RT_NULL,
	   2048, RT_THREAD_PRIORITY_MAX/3+4, 10);
	
	if(print_thread != RT_NULL)
		rt_thread_startup(print_thread);
		
	

    return 0;
}


#ifdef  __cplusplus
}  
#endif



/*@}*/
