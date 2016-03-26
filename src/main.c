
#include "hw_config.h"

#define  EMU_PERIOD        (((SystemCoreClock/4)/10000) - 1)
uint32_t uwDirection = 0;


TIM_Encoder_InitTypeDef Encoder;
/* Timer handler declaration that emulates a quadrature encoder outputs */
TIM_HandleTypeDef    EmulatorHandle;

/* Timer handler declaration */
TIM_HandleTypeDef    Encoder_Handle;

/* Timer Output Compare Configuration Structure declaration */
TIM_OC_InitTypeDef sConfig;

/* Timer Encoder Configuration Structure declaration */
TIM_Encoder_InitTypeDef sEncoderConfig;

void HAL_TIM_Encoder_MspInit(TIM_HandleTypeDef *htim)
{
	GPIO_InitTypeDef GPIO_InitStruct;

	if(htim->Instance == TIM3){
		__TIM3_CLK_ENABLE();
		__GPIOC_CLK_ENABLE();
		GPIO_InitStruct.Pin = GPIO_PIN_6 | GPIO_PIN_7;
		GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
		GPIO_InitStruct.Pull = GPIO_PULLDOWN;
		GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
		GPIO_InitStruct.Alternate = GPIO_AF2_TIM3;
		HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
	}
}
void EncoderInit(TIM_Encoder_InitTypeDef * encoder, TIM_HandleTypeDef * timer, TIM_TypeDef * TIMx, uint32_t maxcount, uint32_t encmode)
{

    timer->Instance = TIMx;
    timer->Init.Period = maxcount;
    timer->Init.CounterMode = TIM_COUNTERMODE_UP;
    timer->Init.Prescaler = 0;
    timer->Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;

    encoder->EncoderMode = encmode;

    encoder->IC1Filter = 0x0F;
    encoder->IC1Polarity = TIM_INPUTCHANNELPOLARITY_RISING;
    encoder->IC1Prescaler = TIM_ICPSC_DIV4;
    encoder->IC1Selection = TIM_ICSELECTION_DIRECTTI;

    encoder->IC2Filter = 0x0F;
    encoder->IC2Polarity = TIM_INPUTCHANNELPOLARITY_FALLING;
    encoder->IC2Prescaler = TIM_ICPSC_DIV4;
    encoder->IC2Selection = TIM_ICSELECTION_DIRECTTI;


    if (HAL_TIM_Encoder_Init(timer, encoder) != HAL_OK) {
        printf("Couldn't Init Encoder\r\n");
        while (1) {}
    }

    if(HAL_TIM_Encoder_Start(timer,TIM_CHANNEL_1)!=HAL_OK) {
        printf("Couldn't Start Encoder\r\n");
        while (1) {}
    }
}
int main(void)
{
	Set_System();
	conio_init(UART_DEFAULT_NUM,UART_BAUDRATE);
	EncoderInit(&Encoder, &EmulatorHandle, TIM3, 0xffff, TIM_ENCODERMODE_TI12);
    uint32_t count = 0;
    while(1){
    	count = __HAL_TIM_GET_COUNTER(&EmulatorHandle);
    	uwDirection = __HAL_TIM_IS_TIM_COUNTING_DOWN(&EmulatorHandle);
    	printf("%d\t%d\n",count,uwDirection);
    }
    return 0;
}



