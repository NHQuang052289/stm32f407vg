
#include "main.h"
#include "string.h"

/* Private variables ---------------------------------------------------------*/
UART_HandleTypeDef huart4;

/* Private function prototypes -----------------------------------------------*/
void Error_Handler(void);
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_UART4_Init(void);
void UART4_IRQHandler(void);

char* send_to_pc 		= 	" Control Led\r\n";	//data send to PC
char* label_Green 		= 	" Toggle Green Led\r\n";
char* label_Red 		= 	" Toggle Red Led\r\n";
char* label_Blue		= 	" Toggle Blue Led\r\n";
char* label_Orange 		= 	" Toggle Orange Led\r\n";
uint8_t  data_receive[1];				//data receive from PC
uint8_t  dataLed = 0;								//var to control Leds


int main(void)
{
  HAL_Init();	
  SystemClock_Config();

  MX_GPIO_Init();
  MX_UART4_Init();
	
	/*HAL_UART_Transmit_IT(&huart4, (uint8_t*) send_to_pc, 10);
	HAL_UART_Receive_IT(&huart4, data_receive, 20);*/
	__HAL_UART_ENABLE_IT(&huart4, UART_IT_TC);
	__HAL_UART_ENABLE_IT(&huart4, UART_IT_RXNE);
	HAL_UART_Transmit_IT(&huart4, (uint8_t*)send_to_pc, 14);	
  while (1)
  {
		//HAL_UART_Receive_IT(&huart4, &dataLed, 10);
		HAL_UART_Receive_IT(&huart4, data_receive, 1);
			
		HAL_Delay(1000 );
			/* NOTE: this code to control led not use IT*/
					/*if((HAL_UART_Receive(&huart4, &dataLed, 1, 1000)==HAL_OK))
						{
							switch(dataLed)
							{
								case'r':
									HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_14);
									HAL_Delay(500);
									break;
								case 'g':
									HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_12);
									HAL_Delay(500);
									break;
								case 'b':
									HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_15);
									HAL_Delay(500);
									break;	
								case 'o':
									HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_13);
									HAL_Delay(500);
									break;			
							}			
						}*/ 
	}
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart->Instance == huart4.Instance) //check exactly is uart4
	{
		if(data_receive[0] == 'g')	//if received "g"
		{			
			//HAL_UART_Transmit_IT(&huart4, (uint8_t*) label_Green, 20);
			HAL_UART_Transmit(&huart4, (uint8_t*) label_Green, 20, 1000);
			HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_12);
		}
		else if(data_receive[0] == 'r')	//if received "r"
		{
			//HAL_UART_Transmit_IT(&huart4, (uint8_t*) label_Red, 20);
			HAL_UART_Transmit(&huart4, (uint8_t*) label_Red, 20, 1000);
			HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_14);
		}
		else if(data_receive[0] == 'b')	//if received "b"
		{
			//HAL_UART_Transmit_IT(&huart4, (uint8_t*) label_Blue, 20);
			HAL_UART_Transmit(&huart4, (uint8_t*) label_Blue, 20, 1000);
			HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_15);
		}
		else if(data_receive[0] == 'o')	//if received "o"
		{
			//HAL_UART_Transmit_IT(&huart4, (uint8_t*) label_Orange, 20);
			HAL_UART_Transmit(&huart4, (uint8_t*) label_Orange, 20, 1000);
			HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_13);
		}

	}
}

/**
  * @brief This function handles UART4 global interrupt.
  */
void UART4_IRQHandler(void)
{
		
	/*	This code pragraph to blynk 4 leds */
		/*for(int i = 0; i<=20; i++)
		{
			if(data_receive[i] == 'g')
				HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_12);
			else if(data_receive[i] == 'r')
				HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_14);
			else if(data_receive[i] == 'b')
				HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_15);
			else if(data_receive[i] == 'o')
				HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_13);
		}*/
		

  HAL_UART_IRQHandler(&huart4);

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
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 168;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief UART4 Initialization Function
  * @param None
  * @retval None
  */
static void MX_UART4_Init(void)
{

  /* USER CODE BEGIN UART4_Init 0 */

  /* USER CODE END UART4_Init 0 */

  /* USER CODE BEGIN UART4_Init 1 */

  /* USER CODE END UART4_Init 1 */
  huart4.Instance = UART4;
  huart4.Init.BaudRate = 115200;
  huart4.Init.WordLength = UART_WORDLENGTH_8B;
  huart4.Init.StopBits = UART_STOPBITS_1;
  huart4.Init.Parity = UART_PARITY_NONE;
  huart4.Init.Mode = UART_MODE_TX_RX;
  huart4.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart4.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart4) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN UART4_Init 2 */

  /* USER CODE END UART4_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15, GPIO_PIN_RESET);

  /*Configure GPIO pins : PD12 PD13 PD14 PD15 */
  GPIO_InitStruct.Pin = GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{ 
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/