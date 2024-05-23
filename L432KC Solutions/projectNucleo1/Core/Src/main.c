/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "sht3x.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define BUF_SIZE 2;
#define BYTE_TO_BINARY_PATTERN "%c%c%c%c%c%c%c%c"
#define BYTE_TO_BINARY(byte) \
	((byte) & 0x80 ? '1' : '0'), \
	((byte) & 0x40 ? '1' : '0'), \
	((byte) & 0x20 ? '1' : '0'), \
	((byte) & 0x10 ? '1' : '0'), \
	((byte) & 0x08 ? '1' : '0'), \
	((byte) & 0x04 ? '1' : '0'), \
	((byte) & 0x02 ? '1' : '0'), \
	((byte) & 0x01 ? '1' : '0')
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
ADC_HandleTypeDef hadc1;

I2C_HandleTypeDef hi2c1;
I2C_HandleTypeDef hi2c3;

UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */
//static const uint8_t SGP30_ADDR = 0x44 << 1;
//static const uint8_t REG_GAS = 0x00;
uint8_t received_command = 0;
char temperatureBuf[50];
char humidityBuf[50];
char moistureBuf[50];
char co2Buf[50];
float temp = 73;
float humidity = 24;
float moisture = 25;
uint8_t CO2 = 26;
uint8_t TVOC = 27;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_I2C1_Init(void);
static void MX_I2C3_Init(void);
static void MX_ADC1_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
//	HAL_StatusTypeDef ret;

//	int16_t val;
//	float temp_c;

	sht3x_handle_t handle = {
	    .i2c_handle = &hi2c3,
	    .device_address = SHT3X_I2C_DEVICE_ADDRESS_ADDR_PIN_LOW
	};

	if (!sht3x_init(&handle)) {
	    printf("SHT3x access failed.\n\r");
	}

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USART2_UART_Init();
  MX_I2C1_Init();
  MX_I2C3_Init();
  MX_ADC1_Init();
  /* USER CODE BEGIN 2 */

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  HAL_UART_Transmit(&huart2, " - Nucleo 1 online - \n\r", 23, HAL_MAX_DELAY);
  HAL_I2C_Slave_Receive_IT(&hi2c1, &received_command, 1);

  //initialisatie van de co2 sensor
  HAL_I2C_Master_Transmit(&hi2c3, 0x58 << 1, (uint8_t[]) {0x20, 0x03}, 2, HAL_MAX_DELAY);

  while (1)
  {
	  HAL_I2C_Slave_Receive_IT(&hi2c1, &received_command, 1);
	  /*
	   * Readout temperature and humidity using library functions and store them in corresponding buffers
	   */
	  sht3x_read_temperature_and_humidity(&handle, &temp, &humidity);
	  sprintf(temperatureBuf, "%.1lf", temp);
	  sprintf(humidityBuf, "%.1lf", humidity);
	  HAL_Delay(10);

	  HAL_ADC_Start(&hadc1);
	  if(HAL_ADC_PollForConversion(&hadc1, HAL_MAX_DELAY) == HAL_OK){
		  HAL_ADC_Stop(&hadc1);
		  moisture = HAL_ADC_GetValue(&hadc1);
	  }
	  sprintf(moistureBuf, "%d", (unsigned int)moisture);
	  //HAL_UART_Transmit(&huart2, moistureBuf, sizeof(moistureBuf), HAL_MAX_DELAY);
	  //HAL_UART_Transmit(&huart2, "\r\n", 2, HAL_MAX_DELAY);

		//char buf[50];       // Read data from SGP30 sensor
		uint8_t data[6]; // Data buffer to store sensor readings
		// Request measurement
		uint8_t command[] = {0x20, 0x08}; // Command for reading measurement data
		HAL_I2C_Master_Transmit(&hi2c3, 0x58 << 1, command, sizeof(command), HAL_MAX_DELAY);
		// Wait for measurement to complete
		HAL_Delay(1);
		// Read measurement data
		HAL_I2C_Master_Receive(&hi2c3, (0x58 << 1) | 0x01, data, sizeof(data), HAL_MAX_DELAY);
		// Process data to get CO2 and TVOC
		CO2 = (data[0] << 8) | data[1];
		TVOC = (data[3] << 8) | data[4];        // Print data to Serial port (UART)
		//sprintf(buf, "CO2: %d TVOC: %d \r\n", CO2, TVOC);
		//HAL_UART_Transmit(&huart2, (uint8_t*)buf, strlen(buf), HAL_MAX_DELAY);
		sprintf(co2Buf, "%d", CO2);

    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
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
  if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure LSE Drive Capability
  */
  HAL_PWR_EnableBkUpAccess();
  __HAL_RCC_LSEDRIVE_CONFIG(RCC_LSEDRIVE_LOW);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSE|RCC_OSCILLATORTYPE_MSI;
  RCC_OscInitStruct.LSEState = RCC_LSE_ON;
  RCC_OscInitStruct.MSIState = RCC_MSI_ON;
  RCC_OscInitStruct.MSICalibrationValue = 0;
  RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_6;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_MSI;
  RCC_OscInitStruct.PLL.PLLM = 1;
  RCC_OscInitStruct.PLL.PLLN = 16;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV7;
  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Enable MSI Auto calibration
  */
  HAL_RCCEx_EnableMSIPLLMode();
}

/**
  * @brief ADC1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_ADC1_Init(void)
{

  /* USER CODE BEGIN ADC1_Init 0 */

  /* USER CODE END ADC1_Init 0 */

  ADC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN ADC1_Init 1 */

  /* USER CODE END ADC1_Init 1 */

  /** Common config
  */
  hadc1.Instance = ADC1;
  hadc1.Init.ClockPrescaler = ADC_CLOCK_ASYNC_DIV1;
  hadc1.Init.Resolution = ADC_RESOLUTION_12B;
  hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc1.Init.ScanConvMode = ADC_SCAN_DISABLE;
  hadc1.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
  hadc1.Init.LowPowerAutoWait = DISABLE;
  hadc1.Init.ContinuousConvMode = DISABLE;
  hadc1.Init.NbrOfConversion = 1;
  hadc1.Init.DiscontinuousConvMode = DISABLE;
  hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
  hadc1.Init.DMAContinuousRequests = DISABLE;
  hadc1.Init.Overrun = ADC_OVR_DATA_PRESERVED;
  hadc1.Init.OversamplingMode = DISABLE;
  if (HAL_ADC_Init(&hadc1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Regular Channel
  */
  sConfig.Channel = ADC_CHANNEL_8;
  sConfig.Rank = ADC_REGULAR_RANK_1;
  sConfig.SamplingTime = ADC_SAMPLETIME_2CYCLES_5;
  sConfig.SingleDiff = ADC_SINGLE_ENDED;
  sConfig.OffsetNumber = ADC_OFFSET_NONE;
  sConfig.Offset = 0;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN ADC1_Init 2 */

  /* USER CODE END ADC1_Init 2 */

}

/**
  * @brief I2C1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C1_Init(void)
{

  /* USER CODE BEGIN I2C1_Init 0 */

  /* USER CODE END I2C1_Init 0 */

  /* USER CODE BEGIN I2C1_Init 1 */

  /* USER CODE END I2C1_Init 1 */
  hi2c1.Instance = I2C1;
  hi2c1.Init.Timing = 0x00707CBB;
  hi2c1.Init.OwnAddress1 = 32;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Analogue filter
  */
  if (HAL_I2CEx_ConfigAnalogFilter(&hi2c1, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Digital filter
  */
  if (HAL_I2CEx_ConfigDigitalFilter(&hi2c1, 0) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C1_Init 2 */

  /* USER CODE END I2C1_Init 2 */

}

/**
  * @brief I2C3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C3_Init(void)
{

  /* USER CODE BEGIN I2C3_Init 0 */

  /* USER CODE END I2C3_Init 0 */

  /* USER CODE BEGIN I2C3_Init 1 */

  /* USER CODE END I2C3_Init 1 */
  hi2c3.Instance = I2C3;
  hi2c3.Init.Timing = 0x00707CBB;
  hi2c3.Init.OwnAddress1 = 0;
  hi2c3.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c3.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c3.Init.OwnAddress2 = 0;
  hi2c3.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
  hi2c3.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c3.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c3) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Analogue filter
  */
  if (HAL_I2CEx_ConfigAnalogFilter(&hi2c3, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Digital filter
  */
  if (HAL_I2CEx_ConfigDigitalFilter(&hi2c3, 0) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C3_Init 2 */

  /* USER CODE END I2C3_Init 2 */

}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  huart2.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart2.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(output1_GPIO_Port, output1_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LD3_GPIO_Port, LD3_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : BUT1_Pin */
  GPIO_InitStruct.Pin = BUT1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(BUT1_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : output1_Pin */
  GPIO_InitStruct.Pin = output1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(output1_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : LD3_Pin */
  GPIO_InitStruct.Pin = LD3_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LD3_GPIO_Port, &GPIO_InitStruct);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */
/*
 * Receive a command flag from the server Pi, and react accordingly
 * 0x01 - Request temperature data
 * 0x02 - Turn single LED ON
 * 0x03 - Turn single LED OFF
 * 0x04 - Request button data (Not in use)
 */
void HAL_I2C_SlaveRxCpltCallback(I2C_HandleTypeDef *hi2c) {
	//yete
	if (received_command == 0x01) {
		HAL_UART_Transmit(&huart2, (uint8_t*) "Received TEMP flag, returning temp: ", 36, HAL_MAX_DELAY);
		HAL_UART_Transmit(&huart2, temperatureBuf, strlen(temperatureBuf), HAL_MAX_DELAY);
		HAL_UART_Transmit(&huart2, (uint8_t*) "\n\r", 2, HAL_MAX_DELAY);
	    if (HAL_I2C_Slave_Transmit_IT(&hi2c1, temperatureBuf, strlen(temperatureBuf)) != HAL_OK) {
	    	HAL_UART_Transmit(&huart2, (uint8_t*) "Didn't work!\n\r", 40, HAL_MAX_DELAY);
	    }
	    HAL_I2C_Slave_Receive_IT(&hi2c1, &received_command, 1);
	} else if (received_command == 0x02) {
		HAL_UART_Transmit(&huart2, (uint8_t*) "Received LEDON flag, turning LED ON\n\r", 37, HAL_MAX_DELAY);
		//HAL_GPIO_WritePin(LEDOUT_GPIO_Port, LEDOUT_Pin, GPIO_PIN_SET);
		HAL_I2C_Slave_Receive_IT(&hi2c1, &received_command, 1);
	} else if (received_command == 0x03) {
		HAL_UART_Transmit(&huart2, (uint8_t*) "Received LEDOFF flag, turning LED OFF\n\r", 39, HAL_MAX_DELAY);
		//HAL_GPIO_WritePin(LEDOUT_GPIO_Port, LEDOUT_Pin, GPIO_PIN_RESET);
		HAL_I2C_Slave_Receive_IT(&hi2c1, &received_command, 1);
	} else if (received_command == 0x04) {
		/*HAL_UART_Transmit(&huart2, (uint8_t*) "Received BUTTON flag, returning button state: ", 46, HAL_MAX_DELAY);
		uint8_t value = !HAL_GPIO_ReadPin(BUT1_GPIO_Port, BUT1_Pin);
		uint8_t buf2[4];
		snprintf((char*)buf2, sizeof (buf2), "%d\n\r", value);
		HAL_UART_Transmit(&huart2, (uint8_t*)buf2, strlen((char*)buf2), HAL_MAX_DELAY);
		if (HAL_I2C_Slave_Transmit_IT(&hi2c1, (uint8_t*) buf2, strlen((char*)buf2)) != HAL_OK) {
			HAL_UART_Transmit(&huart2, (uint8_t*) "Didn't work!\n\r", 40, HAL_MAX_DELAY);
		}
		HAL_I2C_Slave_Receive_IT(&hi2c1, &received_command, 1);*/
	} else if (received_command == 0x05) {
		HAL_UART_Transmit(&huart2, (uint8_t*) "Received HUMD flag, returning humidity: ", 40, HAL_MAX_DELAY);
		HAL_UART_Transmit(&huart2, humidityBuf, strlen(humidityBuf), HAL_MAX_DELAY);
		HAL_UART_Transmit(&huart2, (uint8_t*) "\n\r", 2, HAL_MAX_DELAY);
		if (HAL_I2C_Slave_Transmit_IT(&hi2c1, humidityBuf, strlen(humidityBuf)) != HAL_OK) {
		   	HAL_UART_Transmit(&huart2, (uint8_t*) "Didn't work!\n\r", 40, HAL_MAX_DELAY);
		}
		HAL_I2C_Slave_Receive_IT(&hi2c1, &received_command, 1);
	} else if (received_command == 0x06){
		HAL_UART_Transmit(&huart2, (uint8_t*) "Received CO2 flag, returning CO2: ", 34, HAL_MAX_DELAY);
		HAL_UART_Transmit(&huart2, co2Buf, strlen(co2Buf), HAL_MAX_DELAY);
		HAL_UART_Transmit(&huart2, (uint8_t*) "\n\r", 2, HAL_MAX_DELAY);
		if (HAL_I2C_Slave_Transmit_IT(&hi2c1, co2Buf, strlen(co2Buf)) != HAL_OK) {
			HAL_UART_Transmit(&huart2, (uint8_t*) "Didn't work!\n\r", 40, HAL_MAX_DELAY);
		}
		HAL_I2C_Slave_Receive_IT(&hi2c1, &received_command, 1);
	} else if (received_command == 0x07) {
		HAL_UART_Transmit(&huart2, (uint8_t*) "Received MOIST flag, returning moisture: ", 41, HAL_MAX_DELAY);
		HAL_UART_Transmit(&huart2, moistureBuf, strlen(moistureBuf), HAL_MAX_DELAY);
		HAL_UART_Transmit(&huart2, (uint8_t*) "\n\r", 2, HAL_MAX_DELAY);
		if (HAL_I2C_Slave_Transmit_IT(&hi2c1, moistureBuf, strlen(moistureBuf)) != HAL_OK) {
		   	HAL_UART_Transmit(&huart2, (uint8_t*) "Didn't work!\n\r", 40, HAL_MAX_DELAY);
		}
		//sprintf(tempbuf, BYTE_TO_BINARY_PATTERN, BYTE_TO_BINARY(moistureBuf[0]));
		//HAL_UART_Transmit(&huart2, moisture, sizeof(tempbuf), HAL_MAX_DELAY);
		//sprintf(tempbuf2, BYTE_TO_BINARY_PATTERN, BYTE_TO_BINARY(moistureBuf[1]));
		//HAL_UART_Transmit(&huart2, moistureBuf, strlen(moistureBuf), HAL_MAX_DELAY);
		//HAL_UART_Transmit(&huart2, "\r\n", 2, HAL_MAX_DELAY);
		HAL_I2C_Slave_Receive_IT(&hi2c1, &received_command, 1);
	} else {
		HAL_UART_Transmit(&huart2, (uint8_t*) "Huh?\n", 5, HAL_MAX_DELAY);
	}
}
/* USER CODE END 4 */

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
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
