# DMA (Direct memory access) 
## 1. Khái Niệm 
Đây là 1 cơ chế truyền dữ liệu giữa memory và memory hoặc giữa memory và Peripheral (ngoại vi) không có sự can thiệp xử lý trung gian thông qua CPU. Giúp tối ưu và giảm tải cho CPU đối với 1 chương trình chạy nhiều tác vụ cùng 1 lúc.
## 2. Quá trình CPU xử lý trên MCU 
### 2.1 Khi không dùng DMA

<p align = "center">
<img src = "https://github.com/user-attachments/assets/62bdc883-6785-4c0c-8370-f39d4c135ab0" width = "600" height = "350">

+ CPU lấy lệnh từ flash để xử lý 
+ CPU thực hiện dọc/ghi ngoại vi thông qua các Peripheral bus (APB)
+ CPU lấy data từ ngoại và ghi vào SRAM để tính toán xử lý thông qua memory bus
+ cpu lấy data đã xử lý trong SRAM và ghi ngược trở lại ngoại vi
=> CPU chiếm dụng nhiều bước -> chậm trể chương trình nếu có nhiều tác vụ -> CPU quá tải

### 2.2 Khi dùng DMA 

<p align = "center">
<img src = "https://github.com/user-attachments/assets/072ca112-e37c-4ef6-ab30-f2ac246330a3" width = "600" height = "350">

__Quy trình truyền data từ ngoại vi đến SRAM__
+ CPU kích hoạt DMA hoạt động
+ Ngoại vi gửi request đến DMA cho phép gửi/nhận từ SRAM
+ DMA lấy data từ ngoại vi thông qua BUS matrix đến các bus ngoại vi (APB) 
+ Data chạy ngược lại bus matrix và gửi đến SRAM thông qua mem bus
+ DMA kích hoạt ngắt -> báo CPU quá trình truyền/nhận DMA đã hoàn tất-> để CPU xử lý data trong SRAM

### 2.3 Đặc điểm của DMA
DMA sẽ hoat động theo cơ chế master - slave 

<p align = "center">
<img src = "https://github.com/user-attachments/assets/afd05d44-bafe-4c40-96aa-4b5ad6236996" width = "500" height = "250">

CPU __(master)__ kích hoạt DMA __(slave)__ -> bus __(master)__ sẽ điều khiển quá trình cho phép giao tiếp giữa ngoại vi với memory hoặc giữa memory với memory
+ __Ngoại vi gửi data đến mem__: DMA gửi request dến ngoại vi -> DMA lấy data từ ngoại vi gửi đếm memory 
+ __Peri nhận data từ mem__: DMA gửi request đến memory -> memory gửi dữ liệu cho DMA đưa trở lại cho ngoại vi
+ sau khi quá trình truyền/nhận hoàn tất -> DMA tạo ngắt báo về CPU 
### 2.4 Cấu hình bộ DMA trên stm32f103

__a) Tính năng của DMA trên stm32f103__

<p align = "center">
<img src = "https://github.com/user-attachments/assets/191eba64-9399-41ff-a0bf-8332329812f5" width = "450" height ="250">

- 1 Bộ DMA sẽ có các kênh(channel) được dùng để cấu hình các tính năng truyền/nhận data trên các ngoại vi
- Mỗi channel sẽ có các lựa chọn để cấu hình tương ứng với các chức năng từ ngoại vi
- Tuy nhiên không thể cấu hình để xử lý DMA 2 ngoại vi cùng lúc trên cùng 1 channel

__b) Các tham số của DMA__

+ __DMA_MODE__: Chế độ circular hay normal 

+ __DMA_DIR__: Cấu hình hướng truyền của data

+ __DMA_M2M__: enable/diable memory to memory 

+ __BufferSize__:  số lượng phần tử  muốn nhận/hoặc truyền

+ __MemoryBaseAddress__: địa chỉ trên RAM mà DMA sẽ đọc/ghi dữ
  
  __Quá trình đọc:__
  
  => Khi cấu hình DMA từ bộ nhớ đến ngoại vi __(Memory-to-Peripheral)__, đây là địa chỉ nguồn.

   ```bash
      dmaHandle.Init.Direction = DMA_MEMORY_TO_PERIPH;
      dmaHandle.Init.MemoryBaseAddr = (uint32_t)buffer;  // buffer nằm trong RAM
  ```
  __Quá trình ghi:__
  
  => Khi cấu hình DMA từ ngoại vi đến bộ nhớ __(Peripheral-to-Memory)__, đây là địa chỉ đích.

   ```bash
      dmaHandle.Init.Direction = DMA_PERIPH_TO_MEMORY;
      dmaHandle.Init.MemoryBaseAddr = (uint32_t)&adc_buffer; // Bộ nhớ đệm ADC
    ```
+ __MemoryDataSize__: kích thước mỗi phần tử trong bộ nhớ.

+ __MemoryInc__: enable/diable việc cộng lên địa chỉ tiếp theo để truyền/nhận

+ __PeriBaseAddr:__ địa chỉ của ngoại vi mà DMA sẽ truy cập

    => Truyền từ bộ nhớ (RAM) đến UART (Peripheral
  
    ```bash
     dmaHandle.Init.Direction = DMA_MEMORY_TO_PERIPH;
     dmaHandle.Init.PeripheralBaseAddr = (uint32_t)&USART1->TDR;  // Địa chỉ thanh ghi truyền dữ liệu của UART1
    ```
    => Truyền từ ADC (Peripheral) đến RAM (Memory)
    
    ```bash
      dmaHandle.Init.Direction = DMA_PERIPH_TO_MEMORY;
      dmaHandle.Init.PeripheralBaseAddr = (uint32_t)&ADC1->DR; // Địa chỉ thanh ghi dữ liệu ADC
    ```
    
+ __PeriInc:__ enable/diable cộng địa chỉ thanh ghi -> di chuyển sang 
thanh ghi khác 

+ __DMA_Priority__: mức ưu tiên 

## 3. Giao tiếp UART sử dụng DMA
Ta sẽ sử dụng DMA để cấu hình bộ USART để nhận data gửi từ terminal xuống cho smt32f1
### 3.1 Khai báo các macro cần thiết

<p align = "center">
<img src = "https://github.com/user-attachments/assets/7e328d9d-24d7-4dc9-9e7b-a20192f0a21a" width = "450" height ="150">

### 3.2 Kích hoạt clock cho các ngoại vi 

<p align = "center">
<img src = "https://github.com/user-attachments/assets/58a9255c-4f68-423a-a956-5b6f07832285" width = "450" height ="100">

### 3.3 Cấu hình bộ USART 
__a) Cấu hình GPIO cho các chân USART__

<p align = "center">
<img src = "https://github.com/user-attachments/assets/f15deffe-3e69-428b-8407-434629d33d55" width = "450" height = "250">

__b) Cấu hình các tham số USART__

<p align = "center">
<img src = "https://github.com/user-attachments/assets/81979bb2-bb82-49c2-9a26-e1cc28178716" width = "450" height ="250">

### 3.4 Cấu hình bộ DMA

<p align = "center">
<img src = "https://github.com/user-attachments/assets/be0ff067-cd29-49b7-85f5-2556685d5136" width = "700" height = "350">

### 3.5 Chương trình chính
+ Mở phần mềm hercules, và nhập 1 chuỗi bất kỳ trên terminal và bấm gửi, quan sát data nhận về thông qua phần debug 

<p align = "center">
<img src = "https://github.com/user-attachments/assets/634a9423-a2c2-4283-bf28-700a30221164" width = "700" height = "300">

# PWM (PULSE WITH MODULUS)

## 1. Khái niệm 
Đây là 1 phương pháp để điều chế động xung từ 1 chu kỳ xung cố định. Chúng ta có thể áp dụng để điều khiển khoảng điện áp đầu ra với các mức khác nhau

<p align = "center">
<img src = "https://github.com/user-attachments/assets/7f8f0ba8-8a15-4919-82a7-8e302ae33bfe" width = "500" height = "250">

## 2. Ứng dụng của PWM

<p align = "center">
<img src = "https://github.com/user-attachments/assets/ef574569-9282-48c7-9a45-1ff063dd5c05" width = "500" height = "250">

+ Thông thường mức điện áp logic chỉ tồn tại ở 2 mức 0V và 5 hoặc 3.3 V tương tư với mức logic 0 và 1. Vì vậy để điều khiển được tốc độ của động cơ hoặc cường độ sáng của 1 bóng đèn ta phải tạo ra 1 dải điện áp biến thiên giữa 2 mức trên
+ Ta có thể dùng bộ DAC hoặc PWM
=> Tuy nhiên thông thường ta sẽ sử dụng PWM, vì chúng khá phổ biến và dễ sử dụng 

__Ví dụ:__ Điều khiển cường độ sáng của 1 đèn led ở mức 50% so với mức tối đa (3.3v)

<p align = "center">
<img src = "https://github.com/user-attachments/assets/88b28169-5b34-48a2-8081-ce508e8e67e7" width = "500" height = "250">

Điện áp điều khiển = %thời gian xung HIGH / thời gian toàn bộ chu kỳ
Với duty_cycle = 50% -> V = 50% * 3.3 = 1.65V

## 3. Cấu hình PWM trên stm32f103
+ Mỗi timer trên stm32f1 sẽ có 4 channel được dùng để xuất ra tín hiệu PWM và sẽ có các chân GPIO tương ứng 

<p align = "center">
<img src = "https://github.com/user-attachments/assets/b09a3f81-f214-4cbf-a751-d34afd94af45" width = "500" height = "250">

## 3.1 Cấu hình chân GPIO cho chức năng PWM của timer1

<p align = "center">
<img src = "https://github.com/user-attachments/assets/ebf11d92-9337-47b1-a513-eb772e85bed9" width = "650" height = "150">

## 3.2 Cấu hình thông số PWM

### a) Thông số PWM 

__TIM_OCMode:__ Chế độ hoạt động của timer với

-> PWM1: xung ở mức high khi nhỏ hơn __TIM_Pulse__
-> PWM2: xung ở mức low khi nhỏ hơn __TIM_Pulse__

__TIM_OutputState:__ cho phép xuất tín hiện PWM ở chân cấu hình tương ứng 

__TIM_Pulse:__ Độ rộng xung so với cả chu kỳ (giá trĩ mà timer dùng để so sánh)

__TIM_OCPolarity__ Cực tính của tín hiệu output, chức năng tương tự __TIM_OCMode__

### b) Hàm cấu hìnn PWM

__TIM_OCxInit():__ dùng để cấu hình cho channel X tương ứng

__TIM_OCxPreloadConfig():__ KÍch hoạt tính năng nạp lại giá trị so sánh mới khi 1 có sự kiện __Update Event__ được tạo ra bởi timer sau khi kết thúc điều chế động rộng xung cho giá trị trước đó

__TIM_Cmd():__ cho phép timer hoạt động 

__TIM_SetCompareX(Timx, pulseWidth):__ hàm xuất ra tín hiệu PWM dựa trên pulseWidth (dùng để so sánh và làm cho xung ở mức HIGH khi nhỏ hơn giá trị này)

## 4. Lập trình led sáng tắt dần sử dụng PWM
__Ví dụ:__ ta sẽ điều khiển cường độ sáng của led với chu kỳ T = 100ms, độ phân giải __(resolution)__ là 1000 (số lần thay đổi độ rộng xung) độ rông xung tăng dần từ 0 = 100%, và giảm dần từ 100 - 0&, với tốc độ 5ms mỗi lần tăng giảm

### a) Tính toán các giá trị 

Ta có clock cấp cho timer1 trên stm32f103 là 72MHZ

-> timer_cnt = T / resolution = 100ms / 1000 = 0.1ms

Ta được: 1ms =  72MHz / (prescaler + 1) <=> 1000 hz = 72,000,000 hz (prescaler + 1)
-> prescaler  = 7199

### b) Cấu hình các tham số và chạy chương trình

+ Ta tiến hành cấu hình các tham số trong KeilC như sau

__TIM_Period:__ Đây chính là chu kỳ sáng của led T = 100, tương tự như giá trị Prescaler ta cũng trừ 1, vì bộ đếm trong MCU luôn bắt đầu từ 0

__TIM_Pulse:__ Giá trị đọ rộng xung ban đầu ta để là 0, và sẽ được điều khiển trong chương trình chính thông qua hàm __TIM_SetCompare__

<p align = "center">
<img src = "https://github.com/user-attachments/assets/f941191a-4e28-4cad-be1d-110ead1e16e3" width = "600" height = "350">

+ Trong chương trình chính ta chạy như sau và quan sát độ sáng của led thay đổi trên chân GPIO đã được cấu hình PWM trước đó

<p align = "center">
<img src = "https://github.com/user-attachments/assets/93d1efdf-e60f-47a0-ac85-026c6ce476f3" width = "600" height = "350">
















