#ifndef SERIALPORTLISTENER_H
#define SERIALPORTLISTENER_H

#include <stm32f4xx.h>

class SerialPort;
class SerialPortListener{

public:
    
    SerialPortListener(){}

    virtual void onDateRecive_IT(SerialPort *reciver, char* date , uint16_t len) = 0;

    virtual void onDateSendCplt_IT() = 0;

    virtual void onSeralPortError() = 0;
};

#endif  //SERIALPORTLISTENER_H
