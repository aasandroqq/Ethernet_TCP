#include "stm32f4x7_eth.h"
#include "netconf.h"
#include "main.h"
#include "lwip/tcp.h"
#include "lwip/tcp_impl.h"
#include "serial_debug.h"

#define SYSTEMTICK_PERIOD_MS  10


#define MESSAGE1   "STM32F427x"
#define MESSAGE2   "Hello"

__IO uint32_t LocalTime = 0; /* this variable is used to create a time reference incremented by 10ms */
uint32_t timingdelay;

void LCD_LED_BUTTON_Init(void);

int main(void)
{  
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);

  LCD_LED_BUTTON_Init();

  ETH_BSP_Config();

  LwIP_Init();

  while (1)
  {  
    if (ETH_CheckFrameReceived())
    { 
      LwIP_Pkt_Handle();
    }
    LwIP_Periodic_Handle(LocalTime);
  }
}

void Delay(uint32_t nCount)
{
  timingdelay = LocalTime + nCount;

  while(timingdelay > LocalTime)
  {
  }
}

void Time_Update(void)
{
  LocalTime += SYSTEMTICK_PERIOD_MS;
}

void LCD_LED_BUTTON_Init(void)
{
#ifdef USE_LCD
  STM324xG_LCD_Init();
#endif

  STM_EVAL_LEDInit(LED1);
  STM_EVAL_LEDInit(LED2);
  STM_EVAL_LEDInit(LED3);
  STM_EVAL_LEDInit(LED4);

  STM_EVAL_LEDOn(LED1);
  STM_EVAL_LEDOn(LED2);
  STM_EVAL_LEDOn(LED3);
  STM_EVAL_LEDOn(LED4);
 
#ifdef USE_LCD
  LCD_Clear(Black);

  LCD_SetBackColor(Black);

  LCD_SetTextColor(White);

  LCD_DisplayStringLine(Line0, (uint8_t*)MESSAGE1);
  LCD_DisplayStringLine(Line1, (uint8_t*)MESSAGE2);
#endif
  
  STM_EVAL_PBInit(BUTTON_KEY, BUTTON_MODE_EXTI);
}

