#include "ESP8266.h"
#include <cstdio>

ESP8266::ESP8266(SerialPort::Type SerialPortType, ESPListener *espListener){
    mUartPort = SerialPort::getSerialPort(SerialPortType);
    mProtocal = espListener;
    mProtocal->setESP8266(this);
    mCurState = UNREADY;
    isWaitToSendData = FALSE;
    isOnTTMode = FALSE;


    isWaitToSendData = FALSE;
    dataToSend = NULL;
    dataToSendLen = 0;
    connectionToSend = 0;

    mCurCipMode = 0;
    mCurCipMux =0;
    mCurMode = APANDSTA;
}


EBOOL ESP8266::start(){
    mUartPort->addListener(mProtocal);
	const char* cmd = "AT+RST\r\n\0";
	mUartPort->sendDate(cmd,strlen(cmd));
    return TRUE;
}


EBOOL ESP8266::isChipReady(){
  if(mCurState == READY)return TRUE;
  return FALSE;
}

EBOOL ESP8266::setMode(CWMODE mode){
    if(mUartPort == NULL)return FALSE;
    const char* cmd = NULL;
    if(mode == STATION){
        cmd = "AT+CWMODE=1\n\0";
    }else if(mode == AP){
        cmd = "AT+CWMODE=2\n\0";
    }else if(mode == APANDSTA){
        cmd = "AT+CWMODE=3\n\0";
    }
    if(cmd != NULL){
        mUartPort->sendDate(cmd,strlen(cmd));
    }else {
        return FALSE;
    }
    return TRUE;    
}


EBOOL ESP8266::connectWIFI(char* name, char* psw){//NULL and ""?
    if(mUartPort == NULL)return FALSE;
    sprintf(cmdTemp, "AT+CWJAP=\"%s\",\"%s\"\r\n\0",name, psw);
    mProtocal->setListenState(ESPListener::LWIFIOK);
    isOnTTMode = FALSE;
    mUartPort->sendDate(cmdTemp,strlen(cmdTemp));
    return TRUE;
}

EBOOL ESP8266::disConnectWIFI(){
    if(mUartPort == NULL)return FALSE;
    char* cmd = "AT+CWQAP\r\n\0";
    isOnTTMode = FALSE;
    mUartPort->sendDate(cmd,strlen(cmd));
    return TRUE;
}


EBOOL ESP8266::serveTCPOn(char* port){
    if(mUartPort == NULL)return FALSE;
    sprintf(cmdTemp, "AT+CIPSERVER=1,%s\r\n\0", port);
    mUartPort->sendDate(cmdTemp,strlen(cmdTemp));
    return TRUE;
}

EBOOL ESP8266::closeServer(){
    if(mUartPort == NULL)return FALSE;
    const char* cmd = "AT+CIPSERVER=0\r\n\0";
    mUartPort->sendDate(cmd,strlen(cmd));
    return TRUE;
}


EBOOL ESP8266::enableTransprantTransmition(EBOOL isEnable){
    if(isEnable == TRUE){
        mProtocal->setListenState(ESPListener::LTTMODE);
        return setCipMode(1);
    }else if(isEnable == FALSE){
        return setCipMode(0);
    }
    return FALSE;
}


//if enbale TransPrantTransmition connectionID is meaningless
EBOOL ESP8266::startConnection(uint8_t connectionId, char* netType, char* targetIp, char* targetPort, char* localPort){
    if(mUartPort == NULL)return FALSE;
    char* com = ",";
    if(mCurCipMux == 0){
        if(localPort ==NULL || localPort == ""){
            sprintf(cmdTemp, "AT+CIPSTART=\"%s\",\"%s\",%s\r\n\0", netType, targetIp,targetPort);
        }else{
            sprintf(cmdTemp, "AT+CIPSTART=\"%s\",\"%s\",%s%s%s\r\n\0", netType, targetIp,targetPort, com, localPort);
        }
    }else{
        if(localPort ==NULL || localPort == ""){
            sprintf(cmdTemp, "AT+CIPSTART=%d,\"%s\",\"%s\",%s\r\n\0", connectionId, netType, targetIp,targetPort);
        }else{
            sprintf(cmdTemp, "AT+CIPSTART=%d,\"%s\",\"%s\",%s%s%s\r\n\0", connectionId, netType, targetIp,targetPort, com, localPort);
        }
    }
    
    //char* cmd = "AT+CIPSTART=\"TCP\",\"192.168.1.103\",8888\r\0";
    isOnTTMode = FALSE;
    mProtocal->setListenState(ESPListener::LCONNOK);
    //mUartPort->sendDate(cmd,strlen(cmd));
    mUartPort->sendDate(cmdTemp,strlen(cmdTemp));
    return TRUE;
}

EBOOL ESP8266::closeConnection(uint8_t connectionId){
    if(mUartPort == NULL)return FALSE;
    isOnTTMode = FALSE;
    sprintf(cmdTemp, "AT+CIPCLOSE=%d\n\0", connectionId);
    mUartPort->sendDate(cmdTemp,strlen(cmdTemp));
    return TRUE;
    
}




EBOOL ESP8266::setPreSend(uint8_t connectionId, int len){
    if(mUartPort == NULL)return FALSE;
    if(mCurCipMode == 0){
        if(mCurCipMux == 1){
            sprintf(cmdTemp, "AT+CIPSEND=%d,%d\r\n\0", connectionId, len); 
        }else if(mCurCipMux == 0){
            sprintf(cmdTemp, "AT+CIPSEND=%d\r\n\0", len); 
        }else{
            return FALSE;
        }
    }else if(mCurCipMode == 1){
        sprintf(cmdTemp, "AT+CIPSEND\r\n\0");
    }else{
        return FALSE;
    }
    isOnTTMode = FALSE;
    mProtocal->setListenState(ESPListener::LPRESEND);
    mUartPort->sendDate(cmdTemp,strlen(cmdTemp));
    return TRUE;
}


EBOOL ESP8266::sendDate(char* data, int len){
    if(mUartPort == NULL)return FALSE;
    mProtocal->setListenState(ESPListener::LSENDOK);
    mUartPort->sendDate(data,len);
    return TRUE;
}

//data receive IT handler
EBOOL ESP8266::setEPSListner(ESPListener *listner){
    if(mProtocal != NULL){//todo remove logic not complete 
        
    }
    mProtocal = listner;
    mProtocal->setESP8266(this);
    return TRUE;
}


//private 
EBOOL ESP8266::setCipMux(uint8_t muxValue){
    if(mUartPort == NULL)return FALSE;
    if(muxValue != 1 || muxValue != 0)return FALSE;
    sprintf(cmdTemp, "AT+CIPMUX=%d\n\0", muxValue);
    mUartPort->sendDate(cmdTemp,strlen(cmdTemp));
    return TRUE;
}

EBOOL ESP8266::setCipMode(uint8_t cipMode){
    if(mUartPort == NULL)return FALSE;
    if(cipMode != 1 && cipMode != 0)return FALSE;
    sprintf(cmdTemp, "AT+CIPMODE=%d\r\n\0", cipMode);
    mCurCipMode = cipMode;
    mUartPort->sendDate(cmdTemp,strlen(cmdTemp));
    return TRUE;
}



