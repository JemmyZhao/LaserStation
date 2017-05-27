#ifndef __ATTITUDE_FILTER_H__
#define __ATTITUDE_FILTER_H__

#ifdef __cplusplus

#include "parameter.h"
#include <stm32f4xx.h>

typedef float Quarternion[4];	//w x y z
typedef float Eular[3];			//r p y

typedef struct Kalman_struct
{
	float K;
	float P;
	float Q;
	float R;
} __Kalman;

// ��Ԫ������
class Quarternion_math
{
public:
    void quart_init(Quarternion q);
    void quart_set(Quarternion q, float w, float x, float y, float z);
    void quart_from_eular_d(Quarternion q,  Eular e);
    void quart_from_eular(Quarternion q, Eular e);
    float quart_length(Quarternion q);
    void quart_to_eular(Quarternion q, Eular e);
    void quart_normalize(Quarternion q);
    void quart_slerp(Quarternion pDest, Quarternion pQ1, Quarternion pQ2, float t);
    void quart_add(Quarternion q, Quarternion _q);
    void quart_minus(Quarternion q, Quarternion _q);
    void quart_multi_q(Quarternion q, Quarternion _q);
    void quart_multi_f(Quarternion q, float f);
    void quart_conjugation(Quarternion q);
    void quart_inverse(Quarternion q);
    void quart_from_vector(Quarternion q, Quarternion v1, Quarternion v2);
};

// �ݶ��½�
class Madgwick
{
private:
    float q0,q1,q2,q3; // ��Ԫ��
    float beta;

		float invSqrt(float x);
public:
    Madgwick(float bt);
    float sampleFreq;         // ����Ƶ��
    uint32_t dt_ms;
    void MadgwickUpdate(float gx, float gy, float gz, float ax, float ay, float az);
    void MadgwickUpdate(float gx, float gy, float gz, float ax, float ay, float az, float mx, float my, float mz);
    void getEuler(Vector3f *eular, Vector3f *rad);
    
    void set_beta(float b) { this->beta = b;};
		
		float   get_q0() { float q = q0; return q; }
		float   get_q1() { float q = q1; return q; }
		float   get_q2() { float q = q2; return q; }
		float   get_q3() { float q = q3; return q; }
		
};


// Kalman
class Kalman_AHRS
{
private:
    // ��Ԫ��������
    Quarternion_math _quart;

    Quarternion position_q; // ��Ԫ��
    Quarternion position_q_new;
    Eular position_e;       // ŷ����
    Eular position_e_new;
    // ��������ټ��׳��������ٶȵ�Ӱ��
    float acc_relative[3];
    // ����Ƶ��
    int sample_rate;
    
    Quarternion position_q_adjust;

    __Kalman kalman[3];
  
    // ���ο������˲�
    float kalman_mix_ring(__Kalman *k, float y0, float u, float mid);
    float kalman_mix(__Kalman *k, float y0, float u);
    float sub_ring(float x1, float x2, float mid);

public:
    Kalman_AHRS(int sample);
    // λ�ý��㹦��
    unsigned char position_function;
    
    void posit_acc(Vector3f *avalf);
    void posit_proc_gyro(Vector3f *gvalf);
    void posit_proc_gyro_mul2(Vector3f *gvalf);
    void posit_proc_acce_magn(Vector3f *gvalf, Vector3f *avalf, Vector3f *mvalf);
    void posit_proc_init(Vector3f *avalf, Vector3f *mvalf);
    void posit_adjust_init();
    void posit_adjust_rst();
    void posit_adjust();
    void posit_proc1(Vector3f *avalf, Vector3f *gvalf);
    void posit_proc2(Vector3f *avalf, Vector3f *gvalf, Vector3f *mvalf);

    void posit_eular(Vector3f *eular, Vector3f *rad);
};

/* -------------------------------------------------------------------------- */
/*                                  Mahony                                    */
/* -------------------------------------------------------------------------- */
typedef struct accDeadband_s {
    uint8_t xy;                 // set the acc deadband for xy-Axis
    uint8_t z;                  // set the acc deadband for z-Axis, this ignores small accelerations
} accDeadband_t;

typedef struct fp_vector {
    float X;
    float Y;
    float Z;
} t_fp_vector_def;

typedef union {
    float A[3];
    t_fp_vector_def V;
} t_fp_vector;


#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define ABS(x) ((x) > 0 ? (x) : -(x))

// Mahony
class MahonyAHRS
{
private:
	float q0,q1,q2,q3;
	
	float rMat[3][3];

	accDeadband_t *accDeadband;

	uint32_t accTimeSum;        // keep track for integration of acc
	int accSumCount;

	int32_t accSum[3];
	float fc_acc;

	float invSqrt(float x);
	float atan2_approx(float y, float x);
	float sin_approx(float x);
	float acos_approx(float x);
    float To_180_degrees(float x);
protected:
    float Kp;
    float Ki;
		float  t;

public:
	MahonyAHRS(float kp, float ki);
    void MahonyAHRSUpdate(float gx, float gy, float gz,  float ax, float ay, float az,  float yawError);
	void request_eular(Vector3f *eular, Vector3f *rad);

	void BodyToEarth(t_fp_vector * v);
    // user settable parameters
    static const struct AP_Param::GroupInfo var_info[];
};

#endif
#endif
