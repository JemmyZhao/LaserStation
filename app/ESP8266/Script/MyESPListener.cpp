#include "Script/MyESPListener.h"
#include "ESP8266.h"
#define USING_LOG_DEBUG


void MyESPListener::onDataReive_IT(int connectionID, char* data, uint8_t len){
    
}

void MyESPListener::onReady(){//boot/reboot signal
	#ifdef USING_LOG_DEBUG
	rt_kprintf("[ESP] Ready\n");
	#endif
	
    isCanSendByTTMODE = FALSE;
    rt_thread_delay(RT_TICK_PER_SECOND);
    mESP8266->connectWIFI(wifiName,wifipsw);
	
	
}

void MyESPListener::onWifiConnectOK(){
	
	#ifdef USING_LOG_DEBUG
	rt_kprintf("[ESP] WifiConnectOK\n");
	#endif
	
    isCanSendByTTMODE = FALSE;
	rt_thread_delay(pendtime);
    mESP8266->startConnection(0, "UDP", ip, port, NULL);
    //enable TTmode?
}

void MyESPListener::onWifiDisConnection(){
    isCanSendByTTMODE = FALSE;
    rt_thread_delay(pendtime );
    mESP8266->connectWIFI(wifiName,wifipsw);
}

void MyESPListener::onServerOn(){
     isCanSendByTTMODE = FALSE;
}
    
void MyESPListener::onConnectionOk(uint8_t connectionID){
	#ifdef USING_LOG_DEBUG
	rt_kprintf("[ESP] UDPConnectionOk\n");
	#endif
	
	
    isCanSendByTTMODE = FALSE;
	
    mESP8266->enableTransprantTransmition(TRUE);
}

void MyESPListener::onConnectionFail(uint8_t connectionID){
    isCanSendByTTMODE = FALSE;
    //reconnect
	rt_thread_delay(pendtime );
    mESP8266->startConnection(0, "UDP", ip, port, NULL);
}

void MyESPListener::onConnectonClosed(uint8_t connectionID){
    isCanSendByTTMODE = FALSE;
}

void MyESPListener::onPreSendOk(){
	#ifdef USING_LOG_DEBUG
	rt_kprintf("[ESP] PreSendOk\n");
	#endif
	
    isCanSendByTTMODE = TRUE;
    //mESP8266->sendDate("hhh\r\n", 5);
}

void MyESPListener::onPreSendFail(){
    isCanSendByTTMODE = FALSE;
    rt_thread_delay(pendtime);
	mESP8266->startConnection(0, "UDP", ip, port, NULL);
}

void MyESPListener::onSendDataOk(uint8_t connectonID){
    mESP8266->setPreSend(connectonID, 5);
}


void MyESPListener::onEnableTTModeOk(){
	#ifdef USING_LOG_DEBUG
	rt_kprintf("[ESP] EnableTTModeOk\n");
	#endif
	rt_thread_delay(pendtime);
    isCanSendByTTMODE = FALSE;
    mESP8266->setPreSend(0, 0);
}


EBOOL MyESPListener::isCanSendInTTMode(){
    return isCanSendByTTMODE;
}
