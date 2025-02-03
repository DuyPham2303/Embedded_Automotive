 
 #define ADDR_STR_BLINK 0x08008000
 
 void Boot() {
    // Thi?t l?p l?i h? th?ng clock
    RCC_DeInit();
    // Vô hi ?u hóa các l?i ng?t d? tránh l?i trong quá trình chuy ?n giao
    SCB->SHCSR &= ~(SCB_SHCSR_USGFAULTENA_Msk | SCB_SHCSR_BUSFAULTENA_Msk | SCB_SHCSR_MEMFAULTENA_Msk);
    // C?p nh?t Stack Pointer (SP) t ? b?ng vector ng?t c?a chuong trình chính
    __set_MSP(*(__IO uint32_t*)(ADDR_STR_BLINK));
    // C?p nh?t thanh ghi SCB->VTOR d ? tr? d?n b?ng vector ng?t c?a chuong trình chính
    SCB->VTOR = ADDR_STR_BLINK;
    // L?y d?a ch? Reset Handler c ?a chuong trình chính
    uint32_t JumpAddress = *(__IO uint32_t*)(ADDR_STR_BLINK + 4);
    // T?o con tr? hàm d?n Reset Handler
    void (*reset_handler)(void) = (void (*)(void)) JumpAddress;
    // Nh?y vào Reset Handler c ?a chuong trình chính
    reset_handler();
 }
