#include "main.h"

volatile uint32_t control_snapshot = 0U; //Stores value of CONTROL Register.
/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
void PeriphCommonClock_Config(void);
void generate_interrupt(void);
void RTC_WKUP_IRQHandler(void);
void GetControlRegInfo(void);
void PrintControlRegInfo(void);
void change_access_level_unpriv(void);
void Activity1(void);
/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* Configure the system clock */
  SystemClock_Config();

  /* Configure the peripherals common clocks */
  PeriphCommonClock_Config();

  /* This activity does
     -> Change Privilege access to Un-privilege access.
     -> take snapshot of CONTROL Reg, save it in global var.
     -> if generate_interrupt or Printf gets called,
     -> it will result in hardfault exception.
     -> again change access level to Privilege access.
     -> then use printf or enable/disable irq in code.
     -> print previous & current CONTROL reg values. 
  */
  Activity1();
  
  /* Infinite loop */
  while (1)
  {

  }
}


void Activity1(void)
{
  printf("#################################\n");
  printf("In thread mode : before interrupt\n");
  
  GetControlRegInfo();
  PrintControlRegInfo();

  //Change access level to Un-Privileged.
  change_access_level_unpriv();
  
  //Save CONTROL Register info.
  GetControlRegInfo();
  
  /*This will not work in Unprivileged mode.(SWV hardfault)*/
  //PrintControlRegInfo(); 
  //generate_interrupt();

  //Change access level to Privileged.
  __asm volatile("SVC #2");

  PrintControlRegInfo(); //This will print Snapshot info saved when device is in unprivileged level.

  //Check CONTROL Register info.
  GetControlRegInfo();

  PrintControlRegInfo(); //This will print current info of CONTROL reg.

}

void SVC_Handler(void)
{
  printf("In SVC Handler.\n");
  //read
	__asm volatile ("MRS R0,CONTROL");
	//modify
	__asm volatile ("BIC R0,R0,#0x01");
	//write
	__asm volatile ("MSR CONTROL,R0");
}

void change_access_level_unpriv(void)
{
  printf("Changing to Unprivileged Access Level.\n");
	//read
	__asm volatile ("MRS R0,CONTROL");
	//modify
	__asm volatile ("ORR R0,R0,#0x01");
	//write
	__asm volatile ("MSR CONTROL,R0");

}

void PrintControlRegInfo(void)
{
  if (control_snapshot & (1 << 0)) {
	    printf("Unprivileged Access Level.\n");
	} 
  else {
	    printf("Privileged Access Level.\n");
	}

	if (control_snapshot & (1 << 1)) {
	    printf("Stack pointer: PSP in use\n");
	}
  else {
	    printf("Stack pointer: MSP in use\n");
	}

	if (control_snapshot & (1 << 2)) {
	    printf("FPU context active\n");
	}
}

void GetControlRegInfo(void)
{
	control_snapshot = __get_CONTROL();  // CMSIS intrinsic
}

void generate_interrupt(void)
{
	volatile uint32_t * const pSTIR  = (uint32_t*)0xE000EF00;
	volatile uint32_t * const pISER0 = (uint32_t*)0xE000E100;

	//enable IRQ3 interrupt
	*pISER0 |= ( 1 << 3);

	//generate an interrupt from software for IRQ3
	*pSTIR = (3 & 0x1FF);
}

/* This function(ISR) executes in HANDLER MODE of the processor */
void RTC_WKUP_IRQHandler(void)
{
  GetControlRegInfo();
	printf("In handler mode : ISR\n");
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI|RCC_OSCILLATORTYPE_MSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.MSIState = RCC_MSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.MSICalibrationValue = RCC_MSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_10;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure the SYSCLKSource, HCLK, PCLK1 and PCLK2 clocks dividers
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK4|RCC_CLOCKTYPE_HCLK2
                              |RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_MSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.AHBCLK2Divider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.AHBCLK4Divider = RCC_SYSCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief Peripherals Common Clock Configuration
  * @retval None
  */
void PeriphCommonClock_Config(void)
{
  RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};

  /** Initializes the peripherals clock
  */
  PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_SMPS;
  PeriphClkInitStruct.SmpsClockSelection = RCC_SMPSCLKSOURCE_HSI;
  PeriphClkInitStruct.SmpsDivSelection = RCC_SMPSCLKDIV_RANGE0;

  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}
