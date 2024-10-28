
# Project Title

A brief description of what this project does and who it's for

# Configure STM32F4 for using (External/Timer/UART) Interrupt 
# 1. External Interrupt
Ta sẽ làm 1 ví dụ đơn giản để mô phỏng cách mà ngắt ngoài hoạt động thông qua việc nhấn nút và tăng biến count mỗi khi có 1 tín hiệu ngắt được phát hiện
## 1.1 Setup chương trình
Để lập trình cho các chức năng liên quan đến interrupt ta sẽ cần phải include thư viện __#include<misc.h>__ chứa các hàm liên quan đến cấu hình __NVIC__ (bộ quản lý các ngắt và kích hoạt các hàm ISR)

<p align = "center">
<img src = "https://github.com/user-attachments/assets/20eb4394-30a8-4eff-8902-4fa30d7483ee" width = "600" , height = "300">

Tiếp theo ta phải include file __misc.c__ vào thư mục project để có thể sử dụng được các API của nó

+ Ta click chuột trái vào folder Lib -> chọn đến đường dẫn chứa file __misc.c__

+ Ta cũng chọn thêm file __stm32f4xx_exti_.c__ chứa API về cấu hình bộ ngắt ngoài __EXTI__  

<p align = "center">
<img src = "https://github.com/user-attachments/assets/7bfeed33-2b45-44f7-b6e8-cc5233f59cbd" width = "600" , height = "300">


+ Trong folder Lib ta có được 2 file _.c__ vừa thêm vào, đồng thời ta cũng include các thư viện cần thiết vào file chương trình chính 


<p align = "center">
<img src = "https://github.com/user-attachments/assets/6f4a3f46-83a0-4224-bda3-8f8b4e41ae78" width = "650" , height = "300">


## 1.2 Các thông số cài đặt
__a) Cấu hình chân và line ngắt__ 

Chế độ ngắt ngoài sẽ được quản lý dựa vào các line ngắt cùng với chân GPIO tương ứng của line đó như sau

<p align = "center">
<img src = "https://github.com/user-attachments/assets/890d72ef-b16d-42a9-b449-d465335393a1" width = "400" , height = "250">

Ta sẽ sử dụng chân PA0 cũng là nút nhấn tích hợp sẵn trên stm32f4 để cấu hình là chân ngắt. ta sẽ tạo 1 hàm để config ngắt ngoài cho PA0 như sau 
```bash
void EXTI_Config(){
	EXTI_InitTypeDef EXTI0_Init; 
	EXTI0_Init.EXTI_Mode = EXTI_Mode_Interrupt;      //cấu hình chế độ ngắt
	EXTI0_Init.EXTI_Line = EXTI_Line0;               //chọn line ngắt thứ 0 do nó quản lý chân PA0
	EXTI0_Init.EXTI_Trigger = EXTI_Trigger_Falling; //cấu hình kích hoạt tín hiệu ngắt cạnh xuống
	EXTI0_Init.EXTI_LineCmd = ENABLE;               //kích hoạt bộ ngắt ngoài
	EXTI_Init(&EXTI0_Init); 
}

```
__b) Enable clock cho ngoại vi ngắt__

Đẻ bộ ngoại vi ngắt hoạt động được ta sẽ cần phải cấp clock cho nó hoạt động như những peripheral khác. Trong stm32f4, ngoại vi SYSCFG sẽ quản lý việc cấp clock cho các bộ __EXTI__ hoạt động. 
+ Trước hết ta xác định SYSCFG được nối với bus nào dựa vào __memory map__

<p align = "center">
<img src = "https://github.com/user-attachments/assets/1cc02327-ed3a-4645-a140-08310d8383ac" width = "450" , height = "150">

+ SYSCFG sẽ được chia ra các thanh ghi để quản lý các bộ ngắt ngoài từ __EXTI0 -> 15__ , vì vậy ta phải xác định được line ngắt mà ta muốn kích hoạt là ở thanh ghi SYSCFG nào.

<p align = "center">
<img src = "https://github.com/user-attachments/assets/366691e7-116e-44e1-a70a-052d9e9687b4" width = "450" , height = "150">

+ Ta có hàm để enable clock cho ngoại vi ngắt như sau

```bash
 void RCC_Config()
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG,ENABLE);
}
```
__c) Cấu hình GPIO cho nút nhấn__
```bash
void GPIO_Config()
{
	GPIO_InitTypeDef buttonpin_Init;
	
	buttonpin_Init.GPIO_Pin = GPIO_Pin_0;
	buttonpin_Init.GPIO_Mode = GPIO_Mode_IN;
	buttonpin_Init.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&buttonpin_Init);
	
	GPIO_WriteBit(GPIOA,GPIO_Pin_0,Bit_SET); //trạng thái ban đầu của PA0 ở mức logic HIGH
}
```
__d) Cấu hình bộ NVIC__

NVIC là một bộ quản lý ngắt lồng, nó cho phép MCU xử lý các ngắt đến từ nhiều ngoại vi khác nhau, cũng như là quản lý các mức độ ưu tiên của các ngắt.

<p align = "center">
<img src = "https://github.com/user-attachments/assets/4f4652b6-3fa5-4554-be97-1f38d42f783c" width = "550" , height = "240">

__PeemptionPriority__: mức ưu tiên chính của ngắt, giá trị ưu tiên giảm dần từ 0 - 15 tương ứng với số lượng __line ngắt__  

__SubPriority__: mức ưu tiên phụ quyết định ngắt nào đươc thực hiện trước nếu có nhiều ngắt xảy ra mà có cùng mức ưu tiên chính

__PriorityGroup__: cho phép số lượng các mức ưu tiên chính và phụ có thể được cấu hình tùy vào các group từ 0 - 4

+ Ta sẽ có struct sau để cấu hình các tham số cho NVIC

<p align = "center">
<img src = "https://github.com/user-attachments/assets/8671f8a5-1192-491e-90d7-fb0c63cd7527" width = "650" , height = "300">

__NVIC_IRQChannel__: số thứ tự yêu cầu ngắt  tương ứng với line ngắt mà ta muốn cấu hình. Mỗi ngoại vi ngắt sẽ có __IRQ__ tương ứng được lưu trong __vector table__ 


<p align = "center">
<img src = "https://github.com/user-attachments/assets/0ac1298d-0526-4b53-9053-2bcdeee5ba40" width = "550" , height = "240">

+ Giá trị IRQ tương ứng với các ngắt ngoại vi sẽ được lưu ở trong file __stm32f4xx.h__ , để ta có thể gọi ra sử dụng

<p align = "center">
<img src = "https://github.com/user-attachments/assets/09a451b6-bcc4-45c9-a3d3-d0ffaae198fa" width = "600" , height = "250">

+ Trong chương trình chính ta sẽ tạo ra hàm sau đây để config __NVIC__ và __mức ưu tiên__

```bash
void NVIC_Config(){
	NVIC_InitTypeDef NVIC_InitStruct;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	
	NVIC_InitStruct.NVIC_IRQChannel = EXTI0_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStruct);

}	
```
__e) Gọi hàm thủ tục ngắt ISR để xử lý__

Sau khi ngắt được phát hiện bởi bộ ngắt ngoài __EXTI__ và gửi tín hiệu ngắt __IRQ number__ đến bộ __NVIC__ , thì sau đó __NVIC__ sẽ dựa vào __vector table__ để gọi ra hàm __ISR__ tương ứng 

+ Trong chương trình các hàm __ISR__ sẽ được khởi tạo trong file __startup__ như sau 

<p align = "center">
<img src = "https://github.com/user-attachments/assets/65dda1c3-e1e1-487d-88c5-1742d46f17c8" width = "550" , height = "240">

+ Ta sẽ gọi hàm __ISR__ tương ứng với __EXTI0__ để xử lý đếm lên biến count mỗi khi nhấn nút __PA0__

```bash
uint16_t press = 0;
int main(){
RCC_Config();
GPIO_Config();
NVIC_Config();
EXTI_Config();
TIM_Config();
	return 0;
}
void EXTI0_IRQHandler(){
	if(EXTI_GetITStatus(EXTI_Line0) != RESET){	// kiểm tra có đúng ngắt xảy ra trên EXTI0  
		press += 1;
	}
	EXTI_ClearITPendingBit(EXTI_Line0); //xóa cờ ngắt để tránh chương trình phát hiện ngắt liên tục
}
```

# 2. Timer Interrupt



