/*
 * ��������
 * Author: Kyle
 */
 
#ifndef __BUTTON_H
#define __BUTTON_H

#include "stm32f10x.h"
#include "gimbal.h"
#include "sys_tick.h"

// PB0: ����-ƫ��������
#define BUTTON_YAW_ADD_PORT GPIOB 
#define BUTTON_YAW_ADD_PIN GPIO_Pin_4
// PB1: ����-ƫ���Ǽ���
#define BUTTON_YAW_MINUS_PORT GPIOB
#define BUTTON_YAW_MINUS_PIN GPIO_Pin_5
// PB5: ����-����������
#define BUTTON_PITCH_ADD_PORT GPIOB
#define BUTTON_PITCH_ADD_PIN GPIO_Pin_6
// PB4: ����-�����Ǽ���
#define BUTTON_PITCH_MINUS_PORT GPIOB
#define BUTTON_PITCH_MINUS_PIN GPIO_Pin_7

#define BUTTON_NUM 4 // ��������

// �������
#define BUTTON_ID_YAW_ADD 0
#define BUTTON_ID_YAW_MINUS 1
#define BUTTON_ID_PITCH_ADD 2
#define BUTTON_ID_PITCH_MINUS 3

// ��������ÿ�νǶȵ����ӷ���
#define BUTTON_ANGLE_STEP 5

 
// �����жϳ�ʼ��
void Button_NVIC_Config(void);
// ����GPIO����
void Button_GPIO(void);
// �����ж�����
void Button_EXTI(void);
// ������ʼ��
void Button_Init(void);

#endif
