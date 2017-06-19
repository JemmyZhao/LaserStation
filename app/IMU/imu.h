#include "mpu6500.h"

#define RAD2DEG 57.29577951308232
#define DEG2RAD 0.017453292519943295

class Attitude
{
private:
    MPU6500*             _mpu;
    void MadgwickAHRSupdateIMU(float gx, float gy, float gz, float ax, float ay, float az, float sampleFreq);

    void MadgwickAHRSupdate(float gx, float gy, float gz, float ax, float ay, float az, 
			                      float mx, float my, float mz, float sampleFreq);


public:
    Vector3f              Gyro_af;
    Vector3f              Accel_af;
    float                 Temp_af;
    float                 quat[4];
    float                 q0;
    float                 q1;
    float                 q2;
    float                 q3;

    Vector3f              euler;
    Vector3f              euler_rad;
    
    float                 rawYawError;

    //MadgwickAHR
    float beta;
    float sampleFreq;
    
    // ¹¹Ôìº¯Êý
    Attitude(MPU6500* mpu_6500)
    {
			_mpu = mpu_6500;
    };
    void start_init();
		
		void update();
		float get_q0() { return q0;}
		float get_q1() { return q1;}
		float get_q2() { return q2;}
    float get_q3() { return q3;}
		
		float deg2rad(float x) { return x*DEG2RAD;}
		float rad2deg(float x) { return x*RAD2DEG;}
};