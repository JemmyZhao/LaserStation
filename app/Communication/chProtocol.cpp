#include "chProtocol.h"
#include "rtthread.h"



void Codec::mavlink_msg_to_buf(uint8_t *buf, Mavlink_msg_t *msg)
{
	int i = 0;
	buf[i++] = msg->stx;
	buf[i++] = msg->payload_lth;
	buf[i++] = msg->packet_seq;
	buf[i++] = msg->sys_id;
	buf[i++] = msg->comp_id;
	buf[i++] = msg->msg_id;
	rt_memcpy(&buf[i], &msg->data, msg->payload_lth);
	buf[msg->payload_lth+6] = msg->crcl;
	buf[msg->payload_lth+7] = msg->crch;
}

uint32_t Codec::attitude_quat_packing(uint8_t *buf, 
			                         uint32_t delta_time, float q0, float q1, float q2, float q3,
															 float rollspeed, float pitchspeed, float yawspeed)
{
	mavlink.attitude_quaternion_pack(MAV_TYPE_GIMBAL, MAV_COMP_ID_IMU, &mavlink_tx_msg, 
	                         delta_time, q0, q1, q2, q3, 
	                         rollspeed, pitchspeed, yawspeed);
	mavlink_msg_to_buf(buf, &mavlink_tx_msg);
	
	return mavlink_tx_msg.payload_lth+8;
	
}