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
#include "string.h"
#include "main.h"
#include "gpio.h"
#include "kk_ihex_read.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
typedef enum {
    HEX_OK = 0,
    HEX_EOF,
    HEX_ERR_FORMAT,
    HEX_ERR_CHECKSUM,
    HEX_ERR_ADDRESS,
    HEX_ERR_FLASH,
    HEX_COUNT
} HexStatus_t;
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
#define BTN_TICKS 20
#define SECTOR7_START  0x08060000U
#define SECTOR7_END    0x0807FFFFU
/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
static const char* AppHEX = // simple HAL-free blinker compiled to be placed at 0x08060000
":020000040806EC"
":100000000000022085020608D5020608D50206086F\n"
":10001000D5020608D5020608D50206080000000031\n"   
":10002000000000000000000000000000D5020608EB\n"
":10003000D502060800000000D5020608690206087D\n"
":10004000D5020608D5020608D5020608D50206081C\n"
":10005000D5020608D5020608D5020608D50206080C\n"
":10006000D5020608D5020608D5020608D5020608FC\n"
":10007000D5020608D5020608D5020608D5020608EC\n"
":10008000D5020608D5020608D502060800000000C1\n"
":10009000000000000000000000000000D50206087B\n"
":1000A000D5020608D5020608D5020608D5020608BC\n"
":1000B000D5020608D5020608D5020608D5020608AC\n"
":1000C000D5020608D5020608D5020608D50206089C\n"
":1000D000D5020608D5020608D50206080000000071\n"
":1000E000D5020608D5020608D50206080000000061\n"
":1000F000000000000000000000000000D50206081B\n"
":1001000000000000D5020608D5020608D502060840\n"
":1001100000000000000000000000000000000000DF\n"
":10012000D5020608D5020608D5020608D50206083B\n"
":10013000D5020608000000000000000000000000DA\n"
":10014000000000000000000000000000D5020608CA\n"
":10015000D5020608D5020608D5020608D50206080B\n"
":10016000D5020608D50206080000000000000000C5\n"
":10017000000000000000000000000000000000007F\n"
":1001800000000000D502060800000000000000008A\n"
":08019000D5020608D50206089D\n"
":1001980010B5054C237833B9044B13B10448AFF3B9\n"
":1001A80000800123237010BD000000200000000023\n"
":1001B8002403060808B5034B1BB103490348AFF3F2\n"
":1001C800008008BD00000000040000202403060889\n"
":1001D80072B61C4B1C4A9A60BFF34F8F62B61B491C\n"
":1001E8000A6842F4C0620A60D968194A21F4E061D9\n"
":1001F8000904090C0A43DA60A3F54F63154A5A60EB\n"
":1002080000229A601A68144942F004021A601A68B7\n"
":1002180042F002021A600A6B42F004020A630F4AB3\n"
":10022800116821F000611160116841F0806111606E\n"
":100238004FF4005191611A6842F001021A6030BF10\n"
":10024800FDE700BF00ED00E000000608003C0240AA\n"
":100258000003FA05FF117A00003802400008024046\n"
":10026800054B5A6912F4005F14BF4FF000524FF467\n"
":1002780000529A61704700BF00080240DFF834D08E\n"
":1002880000F04AF80C480D490D4A002302E0D45802\n"
":10029800C4500433C4188C42F9D30A4A0A4C0023C8\n"
":1002A80001E013600432A242FBD300F011F8FFF71B\n"
":1002B8008FFF70470000022000000020000000208F\n"
":1002C80044030608000000201C000020FEE7000090\n"
":1002D80070B50D4E0D4CA41BA4100025A54209D1E4\n"
":1002E8000B4E0C4C00F01AF8A41BA4100025A542D4\n"
":1002F80005D170BD56F8253098470135EEE756F818\n"
":10030800253098470135F2E73C0306083C03060808\n"
":100318003C0306084003060870470000F8B500BF14\n"
":10032800F8BC08BC9E467047F8B500BFF8BC08BCCE\n"
":040338009E46704726\n"
":04033C00BD010608F1\n"
":040340009901060811\n"
":040000050806028562\n"
":00000001FF\n";

struct ihex_state ihex;
uint_fast8_t transfer_ok = 1;
uint_fast8_t eof_reached = 0;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
HAL_StatusTypeDef Flash_EraseSector7(void);
HAL_StatusTypeDef Flash_WriteBuffer(uint32_t addr, const uint8_t *buf, uint16_t len);
void jumpToApp(uint32_t addr);
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
  size_t hex_len = strlen(AppHEX);

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
      if((tick - tick_led) > 200) {
        LED_GPIO_Port->BSRR = (uint32_t)LED_Pin << ((LED_GPIO_Port->ODR & LED_Pin) ? 16 : 0);
        tick_led = tick;
      } 

    if ((BTN_GPIO_Port->IDR & BTN_Pin) == RESET) {
      if (Flash_EraseSector7() != HAL_OK) Error_Handler();
      ihex_begin_read(&ihex);
      ihex_read_bytes(&ihex, AppHEX, hex_len);
      ihex_end_read(&ihex);

      if(!transfer_ok || !eof_reached) Error_Handler();

      jumpToApp(SECTOR7_START);
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

// Call once before processing the HEX stream
HAL_StatusTypeDef Flash_EraseSector7(void) {
    HAL_FLASH_Unlock();
    
    FLASH_EraseInitTypeDef eraseInit = {0};
    uint32_t sectorError = 0;
    
    eraseInit.TypeErase    = FLASH_TYPEERASE_SECTORS;
    eraseInit.VoltageRange = FLASH_VOLTAGE_RANGE_3; // 2.7V to 3.6V
    eraseInit.Sector       = FLASH_SECTOR_7;
    eraseInit.NbSectors    = 1;
    
    HAL_StatusTypeDef status = HAL_FLASHEx_Erase(&eraseInit, &sectorError);
    HAL_FLASH_Lock();
    return status;
}

HAL_StatusTypeDef Flash_WriteBuffer(uint32_t addr, const uint8_t *buf, uint16_t len)
{
    HAL_StatusTypeDef status = HAL_OK;
    uint16_t i = 0;

    HAL_FLASH_Unlock();

    // 1. Align start address to a 4-byte boundary (if needed)
    while ((i < len) && ((addr + i) & 0x3U) != 0U) {
        status = HAL_FLASH_Program(FLASH_TYPEPROGRAM_BYTE, addr + i, buf[i]);
        if (status != HAL_OK) goto cleanup;
        i++;
    }

    // 2. Program full 32-bit words (4 bytes at a time)
    while ((i + 4U) <= len) {
        uint32_t word;
        memcpy(&word, &buf[i], sizeof(uint32_t)); // Safe unaligned RAM read
        status = HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, addr + i, word);
        if (status != HAL_OK) goto cleanup;
        i += 4U;
    }

    // 3. Program any remaining trailing bytes (1 to 3 bytes)
    while (i < len) {
        status = HAL_FLASH_Program(FLASH_TYPEPROGRAM_BYTE, addr + i, buf[i]);
        if (status != HAL_OK) goto cleanup;
        i++;
    }

cleanup:
    HAL_FLASH_Lock();
    return status;
}

ihex_bool_t ihex_data_read(struct ihex_state *ihex,
                           ihex_record_type_t type,
                           ihex_bool_t checksum_error)
{
    if (checksum_error) {
        transfer_ok = 0;
        return 0;
    }

    if (type == IHEX_DATA_RECORD) {
        uint32_t addr = (uint32_t)IHEX_LINEAR_ADDRESS(ihex);

        if (addr < SECTOR7_START || (addr + ihex->length - 1U) > SECTOR7_END) {
            transfer_ok = 0;
            return 0;
        }

        if (Flash_WriteBuffer(addr, ihex->data, ihex->length) != HAL_OK) {
            transfer_ok = 0;
            return 0;
        }
    }
    else if (type == IHEX_END_OF_FILE_RECORD) {
        eof_reached = 1;
    }

    return 1;
}

void jumpToApp(uint32_t addr) {
  uint32_t appResetHandler = *((volatile uint32_t*)(addr + 4U));
  uint32_t appMSP = *((volatile uint32_t*)(addr));
  
  if(appMSP < 0x20000000 || appMSP > 0x20020000) { //128k 
    Error_Handler();  
  }
  if(appResetHandler < 0x08060000 || appResetHandler > 0x0807FFFF || (appResetHandler & 1U) == 0) {
    Error_Handler();  
  }

  __disable_irq();

  SysTick->CTRL = 0U;
  SysTick->VAL = 0U;
  SysTick->LOAD = 0U;

  HAL_RCC_DeInit();
  HAL_DeInit();

  uint8_t i = 8;
  do {
    i--;
    NVIC->ICER[i] = 0xFFFFFFFF;
    NVIC->ICPR[i] = 0xFFFFFFFF;
  } while (i > 0);

  __DSB();
  __ISB();

  void (*jump)(void);
  jump = (void (*)(void))appResetHandler; 
  jump();
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
  uint32_t tick = 0;
  while (1)
  {
    if(SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk) {
      tick++;
      SysTick->CTRL &= ~SysTick_CTRL_COUNTFLAG_Msk;
    }
    if (tick > 50) {
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
