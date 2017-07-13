#include "SerialPort.h"
#include "ESPListener.h"
#include "cstdio"
#include <rthw.h>
void ESPListener::readTimeoutWrap(void* listner){
	((ESPListener*)listner)->readTimeout();
}


void ESPListener::readThreadWrap(void* listner){
	((ESPListener*)listner)->readThread();
}

void ESPListener::readThread( ){
	
	
	while(true){
		
		if( lastDataInTime < 2 || rxBuffer.size() <= 0){
			rt_thread_delay(100);
			continue;
		}
		
		rt_err_t result = rt_sem_take(sem, 10);
		if(result != RT_EOK){
			rt_sem_release(sem);
			rt_thread_delay(100);
			continue;
		}
			
		uint16_t len = rxBuffer.size(), ptr = 0;
		char* date = new char[len];
		while(!rxBuffer.empty())date[ptr++] = rxBuffer.front(), rxBuffer.pop();
		
		rt_sem_release(sem);
		
		//rt_kprintf(date);
		
		if(isFindStr("ready",5, date,len))
		{
			setListenState(LNONE);
			onReady();
			continue ;
		}

		LSITENSTATE preState = mCurListenState;    
		switch (mCurListenState){
			case LWIFIOK:
				if(isFindStr("OK",2, date, len) || isFindStr("GOT IP",6, date, len)){
					onWifiConnectOK();
					if(mCurListenState == preState){
					  setListenState(LNONE);
					}
				}else if(isFindStr("FAIL",4, date, len) ){
					onWifiDisConnection();
				} 
				break;  

			case LCONNOK:
				if((isFindStr("CONNEC",6, date, len) && isFindStr("OK",2, date, len)) || (isFindStr("CONNECTED",9, date, len))){
					onConnectionOk(0);
					if(mCurListenState == preState){
					  setListenState(LNONE);
					}
				}else{
					onConnectionFail(0);
				}
				break;

			case LPRESEND:
				if(isFindStr("OK",2, date, len)){
					onPreSendOk();
					if(mCurListenState == preState){
					  setListenState(LNONE);
					}
				}else if(isFindStr("ERROR", 2, date, len)){
					onPreSendFail();
				} 
				break;

			case LSENDOK:
				if(isFindStr("OK",2, date, len)){
					onSendDataOk(0);
					if(mCurListenState == preState){
					  setListenState(LNONE);
					}
				} 
				break;
		  case LTTMODE:
				if(isFindStr("OK",2, date, len)){
					onEnableTTModeOk();
					if(mCurListenState == preState){
					  setListenState(LNONE);
					}
				} 
				break;
				default:
					break;
			
		}
			
		delete[] date;
		date = NULL;
	}
}


void ESPListener::readTimeout( ){
	lastDataInTime ++;
	if(lastDataInTime < 2)
		return ;
	else 
		lastDataInTime = 2;
	
	
	
}

ESPListener::ESPListener():mESP8266(NULL),mCurListenState(LNONE), lastDataInTime(0), sem(RT_NULL){
	timer = rt_timer_create("ESPListenerTimer",
			ESPListener::readTimeoutWrap, 
			this, 
			RT_TICK_PER_SECOND / 2, 
			RT_TIMER_FLAG_PERIODIC | RT_TIMER_FLAG_SOFT_TIMER); 
			
	if (timer != RT_NULL) rt_timer_start(timer);
			
			
	thread = rt_thread_create("ESPDateThread",
			ESPListener::readThreadWrap, this,
        2048, RT_THREAD_PRI_HIGH, 20);

    if (thread != RT_NULL)rt_thread_startup(thread);
			
	sem = rt_sem_create("ESPLisenterSem", 1, RT_IPC_FLAG_FIFO);
 
}


//implement serialPortListener virtual 
void ESPListener::onDateRecive_IT(SerialPort *reciver, char* date , uint16_t len){
	//char testChar[200];
	//sprintf(testChar, "%s %d\n\0", date, len);	
	//reciver->sendDate(testChar, strlen(testChar));//test
	SerialPort* p1 = SerialPort::getSerialPort(SerialPort::TUSART3);
    p1->sendDate(date, len);
	
	lastDataInTime = 0;
	
	rt_err_t result = rt_sem_take(sem, RT_WAITING_FOREVER);
	if(result == RT_EOK){
		for(int i  = 0; i < len; i++){
			if(rxBuffer.size() == maxRxBufferSize){
				rxBuffer.pop();
			}
			rxBuffer.push(date[i]);
		}
	}
	rt_sem_release(sem);
    
    
	
}


EBOOL ESPListener::isFindStr(const char* str, uint8_t strLen, char* data, uint8_t dataLen){
    if(strLen > dataLen )return FALSE;
    int i = 0, j = 0;
    while(1){
        if(data[i+j] == str[j]){
            j++;
            if(j == strLen)return TRUE;
            else if(i+j == dataLen)return FALSE;
        }else{
            i ++;
            j = 0;
        }
        if(i == dataLen)return FALSE;
    }
    return FALSE;
}


void ESPListener::onDateSendCplt_IT(){

}

void ESPListener::onSeralPortError(){

}
