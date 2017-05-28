#ifndef SERIALPORT_H
#define SERIALPORT_H
#include <map>
#include <vector>

#include <rtthread.h>
#include "SerialPortListener.h"

using namespace std;
class SerialPort{
public :
    enum Type{TUSART1, TUSART2,TUSART3} mType;
    enum State{OK, FAIL, READY, BUSSY, ERROR};
    
protected:
	struct rx_msg
	{
		rt_device_t dev;
		rt_size_t size;
	};

	vector<SerialPortListener*> *ListenerPool;
	rt_thread* portThread;
	rt_device* uartDev;
		
	static map<Type, SerialPort*> *PortPool;
	static rx_msg uart1RXMsg, uart2RXMsg, uart3RXMsg;

	
	static rt_mq_t uart1_rx_mq, uart2_rx_mq, uart3_rx_mq;
	static const uint16_t uart_rx_msg_max_size = 64; 
	static rx_msg uart1_msg_pool[uart_rx_msg_max_size];
	static rx_msg uart2_msg_pool[uart_rx_msg_max_size];
	static rx_msg uart3_msg_pool[uart_rx_msg_max_size];
	
	
	static void uart1_poll_thread(void* parameter);

public:
	SerialPort();

	~SerialPort();// not allow release directly by user

  
//user interface function  
public:
	static SerialPort* getSerialPort(SerialPort::Type PortType);

	State sendDate(const char* date, uint8_t len);

	void addListener(SerialPortListener *Listener);

	State removeListener(SerialPortListener *Listener);

	State rebootSerial(SerialPort* serial);

	rt_err_t rx_ind(rt_device_t dev,rt_size_t size);


	static rt_err_t uart1_rx_ind(rt_device_t dev,rt_size_t size);
	static rt_err_t uart2_rx_ind(rt_device_t dev,rt_size_t size);
	static rt_err_t uart3_rx_ind(rt_device_t dev,rt_size_t size);
private:
    void OnInitError();

    
};

#endif  //SERIALPORT_H
