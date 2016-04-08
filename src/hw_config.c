/********************************************************************************/
/*!
	@file			hw_config.c
	@author         Nemui Trinomius (http://nemuisan.blog.bai.ne.jp)
    @version        2.00
    @date           2015.09.01
	@brief          Configure Basis System on STM32F746G-Discovery!

    @section HISTORY
		2015.07.09	V1.00	Start Here.
		2015.09.01	V2.00	Added ITCM Initialization.

    @section LICENSE
		BSD License. See Copyright.txt
*/
/********************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "hw_config.h"

/* Defines -------------------------------------------------------------------*/
/*
  *         The system Clock is configured as follow :
  *            System Clock source            = PLL (HSE)
  *            SYSCLK(Hz)                     = 200000000(Due to SDRAM Support upto 200MHz)
  *            HCLK(Hz)                       = 200000000
  *            AHB Prescaler                  = 1
  *            APB1 Prescaler                 = 4
  *            APB2 Prescaler                 = 2
  *            HSE Frequency(Hz)              = 25000000
  *            PLL_M                          = 25
  *            PLL_N                          = 400
  *            PLL_P                          = 2
  *            PLL_Q                          = 9
  *            PLLSAI_N                       = 192
  *            PLLSAI_P                       = 4
  *            PLLSAI_Q                       = 4
  *            VDD(V)                         = 3.3
  *            Main regulator output voltage  = Scale1 mode
  *            Flash Latency(WS)              = 6
*/
#define PLL_M		25
#define PLL_N		400
#define PLL_P		RCC_PLLP_DIV2
#define PLL_Q		9
#define PLLSAI_N	192
#define PLLSAI_P	RCC_PLLSAIP_DIV4 /* 192/4=48MHz */
#define PLLSAI_Q	4

/* Constants -----------------------------------------------------------------*/

/* Variables -----------------------------------------------------------------*/
__IO uint16_t CmdKey=0;

/* Copy ITCM Codes Valiables */
extern unsigned int _etdata;
extern unsigned int _stitcm;
extern unsigned int _sitcm;
extern unsigned int _eitcm;

/* Prototypes ----------------------------------------------------------------*/

/* Functions -----------------------------------------------------------------*/
/**************************************************************************/
/*!
	Internal Functions
*/
/**************************************************************************/
/**************************************************************************/
/*!
    @brief	Copy Volatile ITCM.
	@param	None.
    @retval	None.
*/
/**************************************************************************/
static void datacopy(unsigned int romstart, unsigned int start, unsigned int len)
{
	unsigned int *pulDest = (unsigned int*) start;
	unsigned int *pulSrc  = (unsigned int*) romstart;
	unsigned int loop;
	for (loop = 0; loop < len; loop = loop + 4)
		*pulDest++ = *pulSrc++;
}

static void ITCM_Datainit(void)
{
	unsigned int *LoadAddr, *ExeAddr, *EndAddr, SectionLen;

    /* Copy ITCM Codes into ITCM */
	LoadAddr = &_stitcm;
	ExeAddr  = &_sitcm;
	EndAddr  = &_eitcm;
	SectionLen = (void*)EndAddr - (void*)ExeAddr;
	datacopy((unsigned int)LoadAddr, (unsigned int)ExeAddr, SectionLen);
}

/**************************************************************************/
/*!
    @brief	Configures Main system clocks & power.
	@param	None.
    @retval	None.
*/
/**************************************************************************/
static void SystemClock_Config(void)
{
	RCC_ClkInitTypeDef RCC_ClkInitStruct;
	RCC_OscInitTypeDef RCC_OscInitStruct;
	RCC_PeriphCLKInitTypeDef RCC_PeriphCLKInitStruct;

	HAL_StatusTypeDef ret = HAL_OK;

	/* Enable HSE Oscillator and activate PLL with HSE as source */
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
	RCC_OscInitStruct.HSEState = RCC_HSE_ON;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
	RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
	RCC_OscInitStruct.PLL.PLLM = PLL_M;
	RCC_OscInitStruct.PLL.PLLN = PLL_N;
	RCC_OscInitStruct.PLL.PLLP = PLL_P;
	RCC_OscInitStruct.PLL.PLLQ = PLL_Q;

	ret = HAL_RCC_OscConfig(&RCC_OscInitStruct);
	if(ret != HAL_OK)
	{
		while(1) { ; }
	}

	/* Activate the OverDrive to reach the 216 MHz Frequency */
	ret = HAL_PWREx_EnableOverDrive();
	if(ret != HAL_OK)
	{
		while(1) { ; }
	}

	/* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2 clocks dividers */
	RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

	ret = HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_6);
	if(ret != HAL_OK)
	{
		while(1) { ; }
	}


	/* Select PLLSAI output as USB-FS/SDMMC/SAI2 clock(CLK48) source */
    RCC_PeriphCLKInitStruct.PeriphClockSelection = RCC_PERIPHCLK_CLK48;
	RCC_PeriphCLKInitStruct.Clk48ClockSelection  = RCC_CLK48SOURCE_PLLSAIP;
    RCC_PeriphCLKInitStruct.PLLSAI.PLLSAIN = PLLSAI_N;
    RCC_PeriphCLKInitStruct.PLLSAI.PLLSAIP = PLLSAI_P;
    RCC_PeriphCLKInitStruct.PLLSAI.PLLSAIQ = PLLSAI_Q;
	if(HAL_RCCEx_PeriphCLKConfig(&RCC_PeriphCLKInitStruct) != HAL_OK)
	{
		while(1) { ; }
	}
	/* Change SDMMC Clock Source into PLLCLK48(48MHz) */
    RCC_PeriphCLKInitStruct.PeriphClockSelection = RCC_PERIPHCLK_SDMMC1;
    RCC_PeriphCLKInitStruct.Sdmmc1ClockSelection = RCC_SDMMC1CLKSOURCE_CLK48;
    RCC_PeriphCLKInitStruct.PLLSAI.PLLSAIN = PLLSAI_N;
    RCC_PeriphCLKInitStruct.PLLSAI.PLLSAIP = PLLSAI_P;
    RCC_PeriphCLKInitStruct.PLLSAI.PLLSAIQ = PLLSAI_Q;
	if(HAL_RCCEx_PeriphCLKConfig(&RCC_PeriphCLKInitStruct) != HAL_OK)
	{
		while(1) { ; }
	}

	/* Enable I/O Compensation Cell for GPIO over 50MHz! */
	RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;
	SYSCFG->CMPCR |= SYSCFG_CMPCR_CMP_PD;

}

/**************************************************************************/
/*!
    @brief	CPU L1-Cache enable.
	@param	None.
    @retval	None.
*/
/**************************************************************************/
static void CPU_CACHE_Enable(void)
{
	/* Invalidate I-Cache : ICIALLU register */
	SCB_InvalidateICache();

	/* Enable branch prediction */
	SCB->CCR |= (1 <<18);
	__DSB();

	/* Invalidate I-Cache : ICIALLU register */
	SCB_InvalidateICache();

	/* Enable I-Cache */
	SCB_EnableICache();

	/* Enable D-Cache */
	SCB_InvalidateDCache();
	SCB_EnableDCache();

}

/**************************************************************************/
/*!
    @brief	Configure the MPU attributes as Write Through for SRAM1/2.
	@param	None.
    @retval	None.
*/
/**************************************************************************/
static void MPU_Config(void)
{
	MPU_Region_InitTypeDef MPU_InitStruct;

	/* Disable the MPU */
	HAL_MPU_Disable();

	/* Configure the MPU attributes as WT for SRAM */
	MPU_InitStruct.Enable 			= MPU_REGION_ENABLE;
	MPU_InitStruct.BaseAddress 		= 0x20010000;
	MPU_InitStruct.Size 			= MPU_REGION_SIZE_256KB;
	MPU_InitStruct.AccessPermission = MPU_REGION_FULL_ACCESS;
	MPU_InitStruct.IsBufferable 	= MPU_ACCESS_NOT_BUFFERABLE;
	MPU_InitStruct.IsCacheable 		= MPU_ACCESS_CACHEABLE;
	MPU_InitStruct.IsShareable 		= MPU_ACCESS_NOT_SHAREABLE;
	MPU_InitStruct.Number 			= MPU_REGION_NUMBER0;
	MPU_InitStruct.TypeExtField 	= MPU_TEX_LEVEL0;
	MPU_InitStruct.SubRegionDisable = 0x00;
	MPU_InitStruct.DisableExec 		= MPU_INSTRUCTION_ACCESS_ENABLE;

	HAL_MPU_ConfigRegion(&MPU_InitStruct);

	/* Enable the MPU */
	HAL_MPU_Enable(MPU_PRIVILEGED_DEFAULT);
}

/**************************************************************************/
/*!
    @brief	Configures the LED.
	@param	None.
    @retval	None.
*/
/**************************************************************************/
static void LED_Configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	/* Enable GPIO_LED clock */
	RCC->AHB1ENR |= (RCC_AHBPeriph_GPIO_LED);

	/* Configure GPIO for LEDs as Output push-pull */
	GPIO_InitStructure.Pin 			= LED_D1;
	GPIO_InitStructure.Mode 		= GPIO_MODE_OUTPUT_PP;
	GPIO_InitStructure.Pull 		= GPIO_NOPULL;
	GPIO_InitStructure.Speed 		= GPIO_SPEED_HIGH;
	GPIO_InitStructure.Alternate 	= 0;
	HAL_GPIO_Init(GPIO_LED, &GPIO_InitStructure);

	LED_OFF(LED_D1);
}





/**************************************************************************/
/*!
	Gloval Functions
*/
/**************************************************************************/
/**************************************************************************/
/*!
    @brief	Configures Main system clocks & power.
	@param	None.
    @retval	None.
*/
/**************************************************************************/
void Set_System(void)
{
	/* SystemInit was already executed in asm startup */

#if defined(USE_SEMIHOSTING)
	/* Enable Semihosting */
	initialise_monitor_handles();
#endif

	/* Configure MPU */
	MPU_Config();

	/* CPU L1-Cache enable */
	CPU_CACHE_Enable();

	/* STM32F7xx HAL library initialization:
       - Configure the Flash ART accelerator on ITCM interface
       - Systick timer is configured by default as source of time base, but user
         can eventually implement his proper time base source (a general purpose
         timer for example or other time source), keeping in mind that Time base
         duration should be kept 1ms since PPP_TIMEOUT_VALUEs are defined and
         handled in milliseconds basis.
       - Set NVIC Group Priority to 4
		 NVIC_PriorityGroup_0: 0 Pre-emption priorities, 16 Sub-priorities
		 NVIC_PriorityGroup_1: 2 Pre-emption priorities, 8 Sub-priorities
		 NVIC_PriorityGroup_2: 4 Pre-emption priorities, 4 Sub-priorities
		 NVIC_PriorityGroup_3: 8 Pre-emption priorities, 2 Sub-priorities
		 NVIC_PriorityGroup_4: 16 Pre-emption priorities, 0 Sub-priorities
       - Low Level Initialization
     */
	if (HAL_Init() != HAL_OK)
	{
		while(1){};
	}

	/* Initialize ITCM Functions */
	ITCM_Datainit();

	/* Rize up HCLK and SYSClock */
	SystemClock_Config();

	/* Set SysTickCounter for _delay_ms(); */
	SysTickInit(INTERVAL);


	/* Configure the LED  */
	LED_Configuration();

}

/* End Of File ---------------------------------------------------------------*/
