#include "stm32f4xx.h"
#include "mavlink_com.h"
#include "string.h"


class Codec
{
	private:
		void mavlink_msg_to_buf(uint8_t *buf, Mavlink_msg_t *msg);
	  void generate_mavlink_attitude_quat_pck(Mavlink_attitude_quaternion_t *pck);	
	  Mavlink         mavlink;
	
	public:
		//Codec();
		Mavlink_attitude_quaternion_t         mavlink_tx_attitude_quat_pck;
	  Mavlink_msg_t                         mavlink_rx_msg;
	  Mavlink_msg_t                         mavlink_tx_msg;
	  
	  uint32_t attitude_quat_packing(uint8_t *buf,
			                         uint32_t delta_time, float q0, float q1, float q2, float q3,
															 float rollspeed, float pitchspeed, float yawspeed);

	
};

