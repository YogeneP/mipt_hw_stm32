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
#include "main.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
typedef enum __attribute__ ((packed)) {
  OPT_1 = 0,
  OPT_2,
  OPT_3,
  OPT_4,
  OPT_COUNT
} enum_t;

#define STR32_LEN 19
typedef union {
  struct {
    uint16_t val1;
    uint16_t val2;
    uint8_t small_vals[OPT_COUNT];
    enum_t enum_val;
    char msg[64]; //16
  };
  uint32_t STR_32[STR32_LEN];
} str_t;
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
#define BTN_TICKS 20
/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
uint32_t HAL_Write_To_Flash(str_t* data);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

str_t str = {
  .val1 = 1024, 
  .val2 = 2048,
  .small_vals = {
    [OPT_1] = 6,
    [OPT_2] = 7,
    [OPT_3] = 8,
    [OPT_4] = 9
  },
  .enum_val = OPT_2,
  .msg = "It's a deeper kind of slumber..."
};
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

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

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  uint32_t tick_led = 0;
  uint32_t tick_btn = 0;
  uint32_t written = 0; 
  uint32_t blink_period = 200;

  if((HAL_GetTick() - tick_led) > 200) {
    LED_GPIO_Port->BSRR = (uint32_t)LED_Pin;
    tick_led = HAL_GetTick();
  } 
  if((HAL_GetTick() - tick_led) > 400) {
    LED_GPIO_Port->BSRR = (uint32_t)LED_Pin << 16;
    tick_led = HAL_GetTick();
  } 
  while (1)
  {
    uint32_t tick = HAL_GetTick();
    if(!written) {
      if((tick - tick_led) > blink_period) {
        LED_GPIO_Port->BSRR = (uint32_t)LED_Pin << ((LED_GPIO_Port->ODR & LED_Pin) ? 16 : 0);
        tick_led = tick;
      } 
    } 

    if ((tick - tick_btn) > BTN_TICKS) {
      if ((BTN_GPIO_Port->IDR & BTN_Pin) == RESET) {
        if (!written) {
          if(HAL_Write_To_Flash(&str) == 0) {
          written = 1;
          } else {
            blink_period = 50;
          }
        }
        LED_GPIO_Port->BSRR = (uint32_t)LED_Pin;
      } else {
        if (written) {
          LED_GPIO_Port->BSRR = (uint32_t)LED_Pin << 16;
        }
      }
      tick_btn = tick;
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
#define START_ADDR 0x0800C000 //page 3 - 16kb
uint32_t ERRSector = 0xFFFFFFFF;

uint32_t HAL_Write_To_Flash(str_t* data) {
  HAL_FLASH_Unlock();
  int err = HAL_FLASHEx_Erase(&(FLASH_EraseInitTypeDef) { 
                          .TypeErase = FLASH_TYPEERASE_SECTORS,
                          .VoltageRange = FLASH_VOLTAGE_RANGE_3, 
                          .Sector = FLASH_SECTOR_3,
                          .NbSectors = 1 }, &ERRSector); 
  if(err != 0) {
    HAL_FLASH_Lock();
    return err;
  }
  if(ERRSector != 0xFFFFFFFF) {
    HAL_FLASH_Lock();
    return ERRSector;
  } 
  uint32_t* packed_data = data->STR_32; 
  uint32_t addr = START_ADDR + sizeof(packed_data[0]) * STR32_LEN;
  int i = STR32_LEN;
  do {
    i--;
    addr -= sizeof(packed_data[0]);
    err = HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, addr, packed_data[i]);
    if(err != HAL_OK) {
      HAL_FLASH_Lock();
      return err;
    }
  } while (i > 0);
  HAL_FLASH_Lock();
  return 0;
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
