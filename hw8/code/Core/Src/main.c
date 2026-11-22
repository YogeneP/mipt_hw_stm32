/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
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
#include <string.h>
#include "main.h"
#include "gpio.h"
#include "cmox_crypto.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define BTN_TICKS   20
#define CHUNK_SIZE  48
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
cmox_cbc_handle_t Cbc_Ctx;
__IO TestStatus glob_status = FAILED;
/** Extract from NIST Special Publication 800-38A
  * F.2.1 CBC-AES128.Encrypt
    Key 2b7e151628aed2a6abf7158809cf4f3c
    IV 000102030405060708090a0b0c0d0e0f
    Block #1
    Plaintext 6bc1bee22e409f96e93d7e117393172a
    Input Block 6bc0bce12a459991e134741a7f9e1925
    Output Block 7649abac8119b246cee98e9b12e9197d
    Ciphertext 7649abac8119b246cee98e9b12e9197d
    Block #2
    Plaintext ae2d8a571e03ac9c9eb76fac45af8e51
    Input Block d86421fb9f1a1eda505ee1375746972c
    Output Block 5086cb9b507219ee95db113a917678b2
    Ciphertext 5086cb9b507219ee95db113a917678b2
    Block #3
    Plaintext 30c81c46a35ce411e5fbc1191a0a52ef
    Input Block 604ed7ddf32efdff7020d0238b7c2a5d
    Output Block 73bed6b8e3c1743b7116e69e22229516
    Ciphertext 73bed6b8e3c1743b7116e69e22229516
    Block #4
    Plaintext f69f2445df4f9b17ad2b417be66c3710
    Input Block 8521f2fd3c8eef2cdc3da7e5c44ea206
    Output Block 3ff1caa1681fac09120eca307586e1a7
    Ciphertext 3ff1caa1681fac09120eca307586e1a7
  */
const uint8_t Key[] =
{
  0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6, 0xab, 0xf7, 0x15, 0x88, 0x09, 0xcf, 0x4f, 0x3c
};
const uint8_t IV[] =
{
  0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f
};
const uint8_t Plaintext[] =
{
  0x6b, 0xc1, 0xbe, 0xe2, 0x2e, 0x40, 0x9f, 0x96, 0xe9, 0x3d, 0x7e, 0x11, 0x73, 0x93, 0x17, 0x2a,
  0xae, 0x2d, 0x8a, 0x57, 0x1e, 0x03, 0xac, 0x9c, 0x9e, 0xb7, 0x6f, 0xac, 0x45, 0xaf, 0x8e, 0x51,
  0x30, 0xc8, 0x1c, 0x46, 0xa3, 0x5c, 0xe4, 0x11, 0xe5, 0xfb, 0xc1, 0x19, 0x1a, 0x0a, 0x52, 0xef,
  0xf6, 0x9f, 0x24, 0x45, 0xdf, 0x4f, 0x9b, 0x17, 0xad, 0x2b, 0x41, 0x7b, 0xe6, 0x6c, 0x37, 0x10
};
const uint8_t Expected_Ciphertext[] =
{
  0x76, 0x49, 0xab, 0xac, 0x81, 0x19, 0xb2, 0x46, 0xce, 0xe9, 0x8e, 0x9b, 0x12, 0xe9, 0x19, 0x7d,
  0x50, 0x86, 0xcb, 0x9b, 0x50, 0x72, 0x19, 0xee, 0x95, 0xdb, 0x11, 0x3a, 0x91, 0x76, 0x78, 0xb2,
  0x73, 0xbe, 0xd6, 0xb8, 0xe3, 0xc1, 0x74, 0x3b, 0x71, 0x16, 0xe6, 0x9e, 0x22, 0x22, 0x95, 0x16,
  0x3f, 0xf1, 0xca, 0xa1, 0x68, 0x1f, 0xac, 0x09, 0x12, 0x0e, 0xca, 0x30, 0x75, 0x86, 0xe1, 0xa7
};

/* Computed data buffer */
uint8_t Computed_Ciphertext[sizeof(Expected_Ciphertext)];
uint8_t Computed_Plaintext[sizeof(Plaintext)];
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
void ErrorHandler();
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
  cmox_cipher_retval_t retval;
  size_t computed_size;

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
  /* USER CODE BEGIN 2 */
  uint32_t tick_led = HAL_GetTick();
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */

  while(1) {
    if((HAL_GetTick() - tick_led) > 200) {
      LED_GPIO_Port->BSRR = (uint32_t)LED_Pin;
      break;
    }
  }
  tick_led = HAL_GetTick();
  while(1) {
    if((HAL_GetTick() - tick_led) > 400) {
      LED_GPIO_Port->BSRR = (uint32_t)LED_Pin << 16;
      break;
    } 
  }
  /* --------------------------------------------------------------------------
  * SINGLE CALL USAGE
  * --------------------------------------------------------------------------
  */

  /* Compute directly the cipher text passing all the needed parameters */
  retval = cmox_cipher_encrypt(CMOX_AESFAST_CBC_ENC_ALGO, /* Use AES CBC algorithm */
                              Plaintext, sizeof(Plaintext), /* Plain text to encrypt */
                              Key, sizeof(Key), /* AES key to use */
                              IV, sizeof(IV), /* Initialization vector */
                              Computed_Ciphertext, &computed_size); /* Data buffer to receive generated cipher text */

  /* Verify API returned value */
  if (retval != CMOX_CIPHER_SUCCESS)
  {
    Error_Handler();
  }

  /* Verify generated data size is the expected one */
  if (computed_size != sizeof(Expected_Ciphertext))
  {
    Error_Handler();
  }

  /* Verify generated data are the expected ones */
  if (memcmp(Expected_Ciphertext, Computed_Ciphertext, computed_size) != 0)
  {
    Error_Handler();
  }
  LED_GPIO_Port->BSRR = LED_Pin << ((!!(LED_GPIO_Port->ODR & LED_Pin))*16);
  /* Compute directly the plain text passing all the needed parameters */
  retval = cmox_cipher_decrypt(CMOX_AESFAST_CBC_DEC_ALGO, /* Use AES CBC algorithm */
                              Expected_Ciphertext, sizeof(Expected_Ciphertext), /* Cipher text to decrypt */
                              Key, sizeof(Key), /* AES key to use */
                              IV, sizeof(IV), /* Initialization vector */
                              Computed_Plaintext, &computed_size); /* Data buffer to receive generated plain text */

  /* Verify API returned value */
  if (retval != CMOX_CIPHER_SUCCESS)
  {
    Error_Handler();
  }

  /* Verify generated data size is the expected one */
  if (computed_size != sizeof(Plaintext))
  {
    Error_Handler();
  }

  /* Verify generated data are the expected ones */
  if (memcmp(Plaintext, Computed_Plaintext, computed_size) != 0)
  {
    Error_Handler();
  }
  tick_led = HAL_GetTick();

  while (1)
  {
    if((HAL_GetTick() - tick_led) > 500) {
      LED_GPIO_Port->BSRR = LED_Pin << ((!!(LED_GPIO_Port->ODR & LED_Pin))*16);
      tick_led = HAL_GetTick();
    }
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
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
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
  __disable_irq();
  uint32_t tick = 0;
  while (1)
  {
    if(SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk) {
      tick++;
      SysTick->CTRL &= ~SysTick_CTRL_COUNTFLAG_Msk;
    }
    if (tick > 100) {
      LED_GPIO_Port->BSRR = LED_Pin << ((!!(LED_GPIO_Port->ODR & LED_Pin))*16);
      tick = 0;
    }
  /* USER CODE END Error_Handler_Debug */
  }
}
#ifdef USE_FULL_ASSERT
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