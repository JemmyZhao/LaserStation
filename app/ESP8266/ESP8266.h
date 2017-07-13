#ifndef ESP8266_H 
#define ESP8266_H 

#include "SerialPort.h"
#include "ESPListener.h"

class ESP8266{
public:
    enum CWMODE {STATION = 1, AP = 2,  APANDSTA = 3} ;
    enum CIPSTATUS{UNREADY, READY, BUSSY};

private:
    ESPListener* mProtocal;
    CWMODE mCurMode;
    CIPSTATUS mCurState;

    uint8_t mCurCipMode;
    uint8_t mCurCipMux;

    EBOOL isWaitToSendData;
    char* dataToSend;
    uint8_t dataToSendLen;
    uint8_t connectionToSend;

    EBOOL isOnTTMode;

    char cmdTemp[100];

protected:
    SerialPort* mUartPort;
    
public:
    
    ESP8266(SerialPort::Type SerialPortType, ESPListener *espListener);
   
    EBOOL start();

    EBOOL isChipReady();

    EBOOL setMode(CWMODE mode);


    EBOOL connectWIFI(char* name, char* psw);

    EBOOL disConnectWIFI();


    EBOOL serveTCPOn(char* port);

    EBOOL closeServer();
    

    EBOOL enableTransprantTransmition(EBOOL isEnable);


    //if enbale TransPrantTransmition connectionID is meaningless
    EBOOL startConnection(uint8_t connectionId, char* netType, char* targetIp, char* targetPort, char* localPort);


    EBOOL closeConnection(uint8_t connectionId);
    
    EBOOL setPreSend(uint8_t connectionId, int len);
    
    EBOOL sendDate(char* data, int len);

    //data receive IT handler
    EBOOL setEPSListner(ESPListener *listner);


    EBOOL setCipMux(uint8_t muxValue);

    EBOOL setCipMode(uint8_t cipMode);
    
	
	EBOOL connPC(char* ip, char* port, char* wifiName, char* wifiPsw);

};


#endif  //ESP8266_H
