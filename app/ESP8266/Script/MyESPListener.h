#ifndef MYESPLISTENER_H
#define MYESPLISTENER_H

#include "ESPListener.h"
#include "SerialPort.h"

class MyESPListener : public ESPListener{
private:
    SerialPort* usbPort;

    EBOOL isCanSendByTTMODE;
	
	char* ip;
	char* port;
	char* wifiName;
	char* wifipsw;
	uint32_t pendtime;

public:
    MyESPListener(SerialPort* usbLogPort):usbPort(usbLogPort),isCanSendByTTMODE(FALSE){
		ip = "192.168.1.107";
		port = "8888";
		wifiName = "ziroom501";
		wifipsw = "4001001111";
		pendtime = RT_TICK_PER_SECOND / 4;
	}


    //impelment function
    void onDataReive_IT(int connectionID, char* data, uint8_t len);

    void onReady();

    void onWifiConnectOK();

    void onWifiDisConnection();

    void onServerOn();

    void onConnectionFail(uint8_t connectionID);
    
    void onConnectionOk(uint8_t connectionID);

    void onConnectonClosed(uint8_t connectionID);

    void onPreSendOk();

    void onPreSendFail();

    void onSendDataOk(uint8_t connectonID);

    void onESPMiss(){}//?

    void onEnableTTModeOk();


public:
    EBOOL isCanSendInTTMode();

};

#endif  //MYESPLISTENER_H
