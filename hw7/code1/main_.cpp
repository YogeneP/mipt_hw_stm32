#include "main.h"
#include "dma.h"
#include "tim.h"
#include "gpio.h"
static const uint32_t ssin[100] = {
 500,  532,  564,  595,  626,  656,  686,  715,  743,  771,
 797,  822,  845,  868,  888,  908,  925,  941,  955,  967,
 978,  986,  993,  997, 1000, 1000,  999,  995,  990,  982,
 973,  961,  948,  933,  917,  898,  878,  857,  834,  809,
 784,  757,  729,  701,  671,  641,  610,  579,  548,  516,
 500,  468,  436,  405,  374,  344,  314,  285,  257,  229,
 203,  178,  155,  132,  112,   92,   75,   59,   45,   33,
  22,   14,    7,    3,    0,    0,    1,    5,   10,   18,
  27,   39,   52,   67,   83,  102,  122,  143,  166,  191,
 216,  243,  271,  299,  329,  359,  390,  421,  452,  484
};
void SystemClock_Config(void);
int main(void)
{
  HAL_Init();
  SystemClock_Config();
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_TIM1_Init();
  HAL_TIM_PWM_Start_DMA(&htim1,TIM_CHANNEL_1,ssin,100);
  while (1)
  {
  }
}

void HAL_TIM_PWM_PulseFinishedCallback(TIM_HandleTypeDef* htim) {
  HAL_TIM_PWM_Start_DMA(&htim1,TIM_CHANNEL_1,ssin,100);
}