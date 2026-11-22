#include "stm32f4xx.h"

void My_SysTick_Handler(void);

int main(void) {
    __disable_irq();
	SCB->VTOR = 0x08060000;
    __DSB();
	__enable_irq();
	
    SET_BIT(FLASH->ACR, FLASH_ACR_ICEN|FLASH_ACR_DCEN|FLASH_ACR_ICEN);
    NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4); 
    SysTick->LOAD  = (uint32_t)(7999999);
    SysTick->VAL   = 0UL;
    SET_BIT(SysTick->CTRL, SysTick_CTRL_CLKSOURCE_Msk);
    SET_BIT(SysTick->CTRL, SysTick_CTRL_TICKINT_Msk);

    SET_BIT(RCC->AHB1ENR, RCC_AHB1ENR_GPIOCEN);
    CLEAR_BIT(GPIOC->MODER, 1UL << (2*13 + 1));
    SET_BIT(GPIOC->MODER, 1UL << (2*13));

    GPIOC->BSRR = GPIO_PIN_13;

    SET_BIT(SysTick->CTRL, SysTick_CTRL_ENABLE_Msk);

    while(1) {
        __WFI();
    }
}

void My_SysTick_Handler(void) {
    GPIOC->BSRR = GPIO_PIN_13 << (16*(!!(GPIOC->ODR & GPIO_PIN_13))); 
}