/******************** (C) COPYRIGHT 2013 **************************
 * �ļ���  ��main.c
 * ����    ������STM32��RTCʵ��һ�����׵ĵ���ʱ�ӡ��ڳ����ն�����ʾʱ��ֵ��
 *           ��ʾ��ʽΪ Time: XX:XX:XX(ʱ���֣���)����ʱ��
 *           ����Ϊ��23��59��59 ʱ��ˢ��Ϊ��00��00��00��         
 * ʵ��ƽ̨�� STM32������
 * ��汾  ��ST3.5.0
 *
 * ����    ��  
 * ��̳    �� 
 * �Ա�    ��http://gomcu.taobao.com
**********************************************************************************/
#include "stm32f10x.h"
#include "usart1.h"
#include "rtc.h"

/**
  * @brief  Main program.
  * @param  None
  * @retval : None
  */
int main(void)
{	
	/* USART1 config */
	USART1_Config();
	
	/* ����RTC���ж����ȼ� */
	NVIC_Configuration();
	
	printf( "\r\n This is a RTC demo...... \r\n" );
	
	if (BKP_ReadBackupRegister(BKP_DR1) != 0xA5A5)
	{
		/* Backup data register value is not correct or not yet programmed (when
		the first time the program is executed) */
		printf("\r\nThis is a RTC demo!\r\n");
		printf("\r\n\n RTC not yet configured....");
		
		/* RTC Configuration */
		RTC_Configuration();
		
		printf("\r\n RTC configured....");
		
		/* Adjust time by values entred by the user on the hyperterminal */
		Time_Adjust();
		
		BKP_WriteBackupRegister(BKP_DR1, 0xA5A5);
	}
	else
	{
		/* Check if the Power On Reset flag is set */
		if (RCC_GetFlagStatus(RCC_FLAG_PORRST) != RESET)
		{
			printf("\r\n\n Power On Reset occurred....");
		}
		/* Check if the Pin Reset flag is set */
		else if (RCC_GetFlagStatus(RCC_FLAG_PINRST) != RESET)
		{
			printf("\r\n\n External Reset occurred....");
		}
		
		printf("\r\n No need to configure RTC....");
		/* Wait for RTC registers synchronization */
		RTC_WaitForSynchro();
		
		/* Enable the RTC Second */
		RTC_ITConfig(RTC_IT_SEC, ENABLE);
		
		/* Wait until last write operation on RTC registers has finished */
		RTC_WaitForLastTask();
	}
	
	#ifdef RTCClockOutput_Enable
	/* Enable PWR and BKP clocks */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);
	
	/* Allow access to BKP Domain */
	PWR_BackupAccessCmd(ENABLE);
	
	/* Disable the Tamper Pin */
	BKP_TamperPinCmd(DISABLE); /* To output RTCCLK/64 on Tamper pin, the tamper
	             functionality must be disabled */
	
	/* Enable RTC Clock Output on Tamper Pin */
	BKP_RTCOutputConfig(BKP_RTCOutputSource_CalibClock);
	#endif
	
	/* Clear reset flags */
	RCC_ClearFlag();
	
	/* Display time in infinite loop */
	Time_Show();
	
	while (1)
	{
	
	}
}
/******************* (C) COPYRIGHT 2013 *****END OF FILE************/