#include "button.h"


void EXTI4_IRQHandler(void){
	if(EXTI_GetITStatus(EXTI_Line4) != RESET){
		// ƫ��������
		nextYaw = nextYaw + BUTTON_ANGLE_STEP;
		nextYaw = nextYaw > YAW_MAX ? YAW_MAX : nextYaw;
		// ����жϱ�־
		EXTI_ClearITPendingBit(EXTI_Line4);
	}
	
}

void EXTI9_5_IRQHandler(void){
	if(EXTI_GetITStatus(EXTI_Line5) != RESET){
		nextYaw = nextYaw - BUTTON_ANGLE_STEP; // ƫ���Ǽ���
		nextYaw = nextYaw < YAW_MIN ? YAW_MIN : nextYaw;		
		// ����жϱ�־
		EXTI_ClearITPendingBit(EXTI_Line5);
	}
	
	if(EXTI_GetITStatus(EXTI_Line6) != RESET){
		nextPitch = nextPitch + BUTTON_ANGLE_STEP; // ����������
		nextPitch = nextPitch > PITCH_MAX ? PITCH_MAX: nextPitch;
		EXTI_ClearITPendingBit(EXTI_Line6); // ����жϱ�־
	}
	if(EXTI_GetITStatus(EXTI_Line7) != RESET){
		nextPitch = nextPitch - BUTTON_ANGLE_STEP; // �����Ǽ���
		nextPitch  = nextPitch < PITCH_MIN ? PITCH_MIN: nextPitch;
		EXTI_ClearITPendingBit(EXTI_Line7); // ����жϱ�־
	}
	
}

// ����Ƕ�������жϿ�����
void Button_NVIC(void){
	NVIC_InitTypeDef NVIC_InitStructure;
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI4_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

// ����GPIO����
void Button_GPIO(void){
	GPIO_InitTypeDef GPIO_InitStructure; // IO���ýṹ��
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; // ��������
	
	// PB4: ����-ƫ��������
	GPIO_InitStructure.GPIO_Pin = BUTTON_YAW_ADD_PIN;
	GPIO_Init(BUTTON_YAW_ADD_PORT, &GPIO_InitStructure);
	// PB5: ����-ƫ���Ǽ���
	GPIO_InitStructure.GPIO_Pin = BUTTON_YAW_MINUS_PIN;
	GPIO_Init(BUTTON_YAW_MINUS_PORT, &GPIO_InitStructure);
	// PB6: ����-����������
	GPIO_InitStructure.GPIO_Pin = BUTTON_PITCH_ADD_PIN;
	GPIO_Init(BUTTON_PITCH_ADD_PORT, &GPIO_InitStructure);
	// PB7: ����-�����Ǽ���
	GPIO_InitStructure.GPIO_Pin = BUTTON_PITCH_MINUS_PIN;
	GPIO_Init(BUTTON_PITCH_MINUS_PORT, &GPIO_InitStructure);
	
}

// �����ⲿ�ж�����
void Button_EXTI(void){
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource0);
	EXTI_InitTypeDef EXTI_InitStructure;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; // �½��ش����ж�
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);
	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource4);
	EXTI_InitStructure.EXTI_Line = EXTI_Line4;
	EXTI_Init(&EXTI_InitStructure);
	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource5);
	EXTI_InitStructure.EXTI_Line = EXTI_Line5;
	EXTI_Init(&EXTI_InitStructure);
	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource6);
	EXTI_InitStructure.EXTI_Line = EXTI_Line6;
	EXTI_Init(&EXTI_InitStructure);
	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource7);
	EXTI_InitStructure.EXTI_Line = EXTI_Line7;
	EXTI_Init(&EXTI_InitStructure);
}

// ������ʼ��
void Button_Init(void){
	// ����ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);
	// ����Ƕ�������жϿ�����
	Button_NVIC();
	// ����GPIO����
	Button_GPIO();
	// �����ⲿ�ж�����
	Button_EXTI();
}


