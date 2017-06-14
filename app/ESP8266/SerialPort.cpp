
#include "Application.h"
#include "SerialPort.h"



rt_mq_t SerialPort::uart1_rx_mq = NULL;
rt_mq_t SerialPort::uart2_rx_mq =  NULL;
rt_mq_t SerialPort::uart3_rx_mq = NULL;

SerialPort::rx_msg SerialPort::uart1_msg_pool[uart_rx_msg_max_size];
SerialPort::rx_msg SerialPort::uart2_msg_pool[uart_rx_msg_max_size];
SerialPort::rx_msg SerialPort::uart3_msg_pool[uart_rx_msg_max_size];

map<SerialPort::Type, SerialPort*>* SerialPort::PortPool = NULL;

rt_err_t SerialPort::uart1_rx_ind(rt_device_t dev,rt_size_t size){
	struct rx_msg msg;
	msg.dev = dev;
	msg.size = size;
	rt_mq_send(uart1_rx_mq, &msg, sizeof(struct rx_msg));
}

rt_err_t SerialPort::uart2_rx_ind(rt_device_t dev,rt_size_t size){
	
	rx_msg msg;
	msg.dev = dev;
	msg.size = size;
	rt_mq_send(uart2_rx_mq, &msg, sizeof(struct rx_msg));
}


rt_err_t SerialPort::uart3_rx_ind(rt_device_t dev,rt_size_t size){
	struct rx_msg msg;
	msg.dev = dev;
	msg.size = size;
	rt_mq_send(uart3_rx_mq, &msg, sizeof(struct rx_msg));
	
}

void SerialPort::uart1_poll_thread(void* parameter){
	
	struct rx_msg msg;
	char uart_rx_buffer[1024];
	SerialPort* uart1 = NULL;
	while(true){
		/*SerialPort *port1 = SerialPort::getSerialPort(SerialPort::TUSART1);
		rt_device_write(port1->uartDev, 0, "hhhh", 4);
		continue;*/
		
		rt_err_t result = rt_mq_recv(uart1_rx_mq, &msg, sizeof(struct rx_msg), 50);
		if (result == RT_ETIMEOUT)
		{
			;
		
		}
		if (result == RT_EOK)
		{
			rt_uint32_t rx_length;
			rx_length = (sizeof(uart_rx_buffer) - 1) > msg.size ?
			msg.size : sizeof(uart_rx_buffer) - 1;
			
			rx_length = rt_device_read(msg.dev, 0, &uart_rx_buffer[0],
			rx_length);
			uart_rx_buffer[rx_length] = '\0';
			
			if(rx_length <= 0)continue;
			
			
			//data receive callback
			uart1 = SerialPort::getSerialPort(SerialPort::TUSART1);
			if(uart1 != NULL && uart1->ListenerPool != NULL){
				uint8_t size = uart1->ListenerPool->size();
				for(uint8_t i = 0; i < size; i++){
					SerialPortListener* listener = (*(uart1->ListenerPool))[i];
					if(listener != NULL){
						listener->onDateRecive_IT(uart1, uart_rx_buffer, rx_length);
					}
				}
			}
		}

		
		//rt_thread_delay(RT_TICK_PER_SECOND / 2);
	}
}


SerialPort::SerialPort(){
    ListenerPool = NULL;
    
}


SerialPort::~SerialPort(){
   
    delete[] ListenerPool;
}



SerialPort* SerialPort::getSerialPort(SerialPort::Type PortType){
	
	
	
    if(PortPool != NULL && (*PortPool)[PortType] != NULL){
        SerialPort* curport = (*PortPool)[PortType];
        return curport;
    }
		
	
	
	SerialPort* portToMake = new SerialPort();
	char* uartName = NULL;
	
	switch (PortType) {

		case SerialPort::TUSART1: {
			uartName = "uart1"	;
			break;
		}
		
		
		 case SerialPort::TUSART2: {
			uartName = "uart2"	;
			break;
		}
		 
		 case SerialPort::TUSART3: {
			uartName = "uart3"	;
			break;
		}
		
		default:
			break;
		
	}
	if(uartName == NULL){
		delete portToMake;
		return NULL;
	}
	
	
	portToMake->uartDev = rt_device_find(uartName);
	if(portToMake->uartDev == RT_NULL){
		delete portToMake;
		return NULL;
	}
	
	if(rt_device_open(portToMake->uartDev, RT_DEVICE_OFLAG_RDWR | RT_DEVICE_FLAG_INT_RX) == RT_EOK){
		switch (PortType) {

			case SerialPort::TUSART1: {
				uart1_rx_mq = new rt_messagequeue();
				rt_mq_init(uart1_rx_mq, "uart1_rx_mq",
								&uart1_msg_pool[0], 
								sizeof(rx_msg), 
								sizeof(uart1_msg_pool), 
								RT_IPC_FLAG_FIFO);
				
				rt_device_set_rx_indicate(portToMake->uartDev, SerialPort::uart1_rx_ind);
								
				portToMake->portThread = rt_thread_create("uart1_port_thread",
				uart1_poll_thread, RT_NULL,
				2048, RT_THREAD_PRI_HIGH, 20);

				if (portToMake->portThread != RT_NULL)
					rt_thread_startup(portToMake->portThread);
				break;
			}
			
			
			 case SerialPort::TUSART2: {
				uart2_rx_mq = new rt_messagequeue();
				rt_mq_init(uart2_rx_mq, "uart2_rx_mq",
								&uart2_msg_pool[0], 
								sizeof(rx_msg), 
								sizeof(uart2_msg_pool), 
								RT_IPC_FLAG_FIFO);
				rt_device_set_rx_indicate(portToMake->uartDev, SerialPort::uart2_rx_ind);
				break;
			}
			 
			 case SerialPort::TUSART3: {
				uart3_rx_mq = new rt_messagequeue();
				rt_mq_init(uart3_rx_mq, "uart3_rx_mq",
								&uart3_msg_pool[0], 
								sizeof(rx_msg), 
								sizeof(uart3_msg_pool), 
								RT_IPC_FLAG_FIFO);
				rt_device_set_rx_indicate(portToMake->uartDev, SerialPort::uart3_rx_ind);
								
								
				break;
			}
			
			default:
				break;
			
		}
		
		if (PortPool == NULL) {
			PortPool = new map<SerialPort::Type, SerialPort*>();
		}

		(*PortPool)[PortType] = portToMake;
		return portToMake;
	}//end if (dev open ok)
	
	delete portToMake;
	return NULL;
} 
       

SerialPort::State SerialPort::sendDate(const char* date, uint8_t len){
    rt_device_write(uartDev, 0, date, len);
    return OK;
}

//线程同步方法
void SerialPort::addListener(SerialPortListener *Listener){
			
	rt_base_t level =  rt_hw_interrupt_disable();
	
    if(ListenerPool == NULL){
        ListenerPool = new vector<SerialPortListener*>();
    }
    if(Listener != NULL){
		ListenerPool->push_back(Listener);
    }
		
	rt_hw_interrupt_enable(level);

}

SerialPort::State SerialPort::removeListener(SerialPortListener *Listener){
    return SerialPort::BUSSY; 
}

SerialPort::State SerialPort::rebootSerial(SerialPort* serial){
    return SerialPort::BUSSY;
}




rt_err_t SerialPort::rx_ind(rt_device_t dev,rt_size_t size){
	return RT_EOK;
}


