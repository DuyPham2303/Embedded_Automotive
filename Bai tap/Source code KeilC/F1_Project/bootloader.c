 
 #define ADDR_STR_BLINK 0x08008000
 
 void Boot() {
    // Thi?t l?p l?i h? th?ng clock
    RCC_DeInit();
    // V� hi ?u h�a c�c l?i ng?t d? tr�nh l?i trong qu� tr�nh chuy ?n giao
    SCB->SHCSR &= ~(SCB_SHCSR_USGFAULTENA_Msk | SCB_SHCSR_BUSFAULTENA_Msk | SCB_SHCSR_MEMFAULTENA_Msk);
    // C?p nh?t Stack Pointer (SP) t ? b?ng vector ng?t c?a chuong tr�nh ch�nh
    __set_MSP(*(__IO uint32_t*)(ADDR_STR_BLINK));
    // C?p nh?t thanh ghi SCB->VTOR d ? tr? d?n b?ng vector ng?t c?a chuong tr�nh ch�nh
    SCB->VTOR = ADDR_STR_BLINK;
    // L?y d?a ch? Reset Handler c ?a chuong tr�nh ch�nh
    uint32_t JumpAddress = *(__IO uint32_t*)(ADDR_STR_BLINK + 4);
    // T?o con tr? h�m d?n Reset Handler
    void (*reset_handler)(void) = (void (*)(void)) JumpAddress;
    // Nh?y v�o Reset Handler c ?a chuong tr�nh ch�nh
    reset_handler();
 }
