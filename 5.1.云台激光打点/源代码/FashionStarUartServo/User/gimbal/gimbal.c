#include "gimbal.h"

// ƫ���������Ƕ� ����ӳ��
// servo = K_yaw2srv * yaw + b_yaw2srv
float K_yaw2srv; // ƫ����ת��Ϊ����Ƕȵı���ϵ��
float b_yaw2srv; // ƫ����ת��Ϊ����Ƕȵ�ƫ����
// yaw = K_srv2yaw * servo + b_srv2yaw 
float K_srv2yaw; // ����Ƕ�ת��Ϊƫ���ǵı���ϵ��
float b_srv2yaw; // ����Ƕ�ת��Ϊƫ���ǵ�ƫ����
// �����������Ƕ� (����ӳ��)
// servo = K_pitch2srv * pitch + b_yaw2srv
float K_pitch2srv; // ������ת��Ϊ����Ƕȵı���ϵ��
float b_pitch2srv; // ������ת��Ϊ����Ƕȵ�ƫ����
// pitch = K_srv2pitch * servo + b_srv2pitch
float K_srv2pitch; // ����Ƕ�ת��Ϊ�����ǵı���ϵ��
float b_srv2pitch; // ����Ƕ�ת��Ϊ�����ǵ�ƫ���� 

// ��̨λ��״̬
float curSrvYaw = 0; 	// ƫ���ǵ�ԭʼ�����ǰ�Ƕ� (��λ ��)
float curSrvPitch = 0; 	// �����ǵ�ԭʼ�����ǰ�Ƕ� (��λ ��)
float nextSrvYaw = 0; 	// ƫ���ǵ�ԭʼ���Ŀ��Ƕ� (��λ ��)
float nextSrvPitch = 0; // �����ǵ�ԭʼ���Ŀ��Ƕ� (��λ ��)
float curYaw = 0; 		// ��̨��ǰ��ƫ���� (��λ ��)
float curPitch = 0; 	// ��̨��ǰ�ĸ����� (��λ ��)
float nextYaw = 0; 		// ��̨Ŀ��ĸ����� (��λ ��)
float nextPitch = 0; 	// ��̨Ŀ���ƫ���� (��λ ��)
float speedYaw = 0; 	// ƫ����ת�� (��λ ��/s)
float speedPitch = 0; 	// ������ת�� (��λ ��/s)

// ��̨��ʼ��
void Gimbal_Init(Usart_DataTypeDef* servoUsart){
	
	// ���ɶ���Ƕ�ӳ����ص�ϵ��
	Gimbal_GenSrvMapParams();
	// �����̨����
	Gimbal_Reset(servoUsart);
	// ����λ��
	Gimbal_Update(servoUsart);
}

// ���ö����̨
void Gimbal_Reset(Usart_DataTypeDef* servoUsart){
	uint16_t interval1, interval2;
	// ����Ϊ��ʼ�Ƕ�
	interval1 = Gimbal_SetYaw(servoUsart, YAW_INIT, YAW_SPEED_INIT);
	interval2 = Gimbal_SetPitch(servoUsart, PITCH_INIT, PITCH_SPEED_INIT);
	// ��ʱ�ȴ���̨��λ
	interval1 = interval1 > interval2 ? interval1 : interval2;
	SysTick_DelayMs(interval1);
}

// ������̨Ϊ����ģʽ
void Gimbal_Damping(Usart_DataTypeDef* servoUsart){
	uint16_t power = 500;
	FSUS_DampingMode(servoUsart, SERVO_ID_YAW, power);
	FSUS_DampingMode(servoUsart, SERVO_ID_PITCH, power);
}


// ���ɶ���Ƕ�ӳ����ص�ϵ��
void Gimbal_GenSrvMapParams(void){
	K_yaw2srv = (YAW1_SERVO_ANGLE - YAW2_SERVO_ANGLE) / (YAW1 - YAW2);
	b_yaw2srv = YAW1_SERVO_ANGLE - K_yaw2srv * YAW1;
	K_srv2yaw = (YAW1 - YAW2) / (YAW1_SERVO_ANGLE - YAW2_SERVO_ANGLE);
	b_srv2yaw =  YAW1 - K_srv2yaw *  YAW1_SERVO_ANGLE;
	
	K_pitch2srv = (PITCH1_SERVO_ANGLE - PITCH2_SERVO_ANGLE) / (PITCH1 - PITCH2);
	b_pitch2srv = PITCH1_SERVO_ANGLE - K_pitch2srv * PITCH1;
	K_srv2pitch = (PITCH1 - PITCH2) / (PITCH1_SERVO_ANGLE - PITCH2_SERVO_ANGLE);
	b_srv2pitch = PITCH1 - K_srv2pitch * PITCH1_SERVO_ANGLE;	
}

// ����ƫ����
void Gimbal_UpdateYaw(Usart_DataTypeDef* servoUsart){
	FSUS_QueryServoAngle(servoUsart, SERVO_ID_YAW, &curSrvYaw);
	curYaw = Gimbal_Servo2Yaw(curSrvYaw);
}

// ���¸�����
void Gimbal_UpdatePitch(Usart_DataTypeDef* servoUsart){
	FSUS_QueryServoAngle(servoUsart, SERVO_ID_PITCH, &curSrvPitch);
	curPitch = Gimbal_Servo2Pitch(curSrvPitch);
}

// ���¶����̨λ��
void Gimbal_Update(Usart_DataTypeDef* servoUsart){
	Gimbal_UpdateYaw(servoUsart);
	Gimbal_UpdatePitch(servoUsart);
}

// ƫ����ת��Ϊ����Ƕ�
float Gimbal_Yaw2Servo(float yaw){
	return K_yaw2srv * yaw + b_yaw2srv;
}

// ����Ƕ�ת��Ϊƫ����
float Gimbal_Servo2Yaw(float servo){
	return K_srv2yaw * servo + b_srv2yaw;
}
	
// ������ת��Ϊ����Ƕ�
float Gimbal_Pitch2Servo(float pitch){
	return K_pitch2srv * pitch + b_pitch2srv;
}

// ����Ƕ�ת��Ϊ������
float Gimbal_Servo2Pitch(float servo){
	return K_srv2pitch * servo + b_srv2pitch;
}

// ������̨��ƫ����
uint16_t Gimbal_SetYaw(Usart_DataTypeDef* servoUsart, float yaw, float speed){
	uint16_t interval;
	// ���½Ƕ�
	Gimbal_UpdateYaw(servoUsart);
	// �߽���
	yaw = (yaw < YAW_MIN) ? YAW_MIN: yaw;
	yaw = (yaw > YAW_MAX) ? YAW_MAX: yaw;
	// Ŀ��Ƕ�
	nextYaw = yaw;
	nextSrvYaw = Gimbal_Yaw2Servo(yaw);
	// ���ƶ����ת
	interval = (uint16_t)__fabs(1000 * (curYaw - nextYaw) / speed); // ʱ������λms
	FSUS_SetServoAngle(servoUsart, SERVO_ID_YAW, nextSrvYaw, interval, 0, 0);
	return interval;
}

// ������̨�ĸ�����
uint16_t Gimbal_SetPitch(Usart_DataTypeDef* servoUsart, float pitch, float speed){
	uint16_t interval;
	// ���½Ƕ�
	Gimbal_UpdatePitch(servoUsart);
	// �߽���
	pitch = (pitch < PITCH_MIN) ? PITCH_MIN : pitch;
	pitch = (pitch > PITCH_MAX) ? PITCH_MAX : pitch;
	// Ŀ��Ƕ�
	nextPitch = pitch;
	nextSrvPitch = Gimbal_Pitch2Servo(pitch);
	// ���ƶ����ת
	interval = (uint16_t)__fabs(1000 *(curPitch - nextPitch) / speed);
	FSUS_SetServoAngle(servoUsart, SERVO_ID_PITCH, nextSrvPitch, interval, 0, 0);
	return interval;
}

// �ȴ������ת���ض���λ��
void Gimbal_Wait(Usart_DataTypeDef* servoUsart){
	SysTick_DelayMs(10);
	while(1){
		// ���½Ƕ�
		Gimbal_Update(servoUsart);
		if (__fabs(curPitch - nextPitch) < SERVO_DEAD_BLOCK && __fabs(curYaw - nextYaw) < SERVO_DEAD_BLOCK){
			break;
		}
	}
}
