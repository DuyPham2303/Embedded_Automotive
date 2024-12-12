# 1. Interrupt
## 1.1 Khái niệm
Ngắt là 1 tín hiệu khẩn cấp được kích hoạt bởi những tín hiêu đếnu72 trong hoặc ngoài phần cứng trên vi điều khiển, mục đích là để báo cho CPU biết phải tạm dừng luồng thực thi chương trình hiện tại để nhảy đến 1 hàm thủ tục ngắt (ISR) để xử lý 1 công việc nào đó, xong rồi mới trở lại thực hiện tiếp công việc trước đó trong chương trình chính

__Tại sao phải sử dụng ngắt__

Xem xét chương trình dùng để kiểm tra trạng thái nút nhấn trong hàm main như sau không sử dụng ngắt

<p align = "center">
<img src = "https://github.com/user-attachments/assets/6feaaec7-d59e-42de-abd8-5b2416f884e9" width = "300" height = "350">

+ Hàm kiểm tra trạng thái nút nhấn sẽ được gọi liên tục, tuy nhiên trong 1 chương trình đương nhiên chúng ta sẽ còn rất nhiều các hàm và câu lệnh khác chạy cùng 1 lúc, và chương trình sẽ xử lý lần lượt các lệnh từ trên xuống dưới chứ không phải cùng 1 lúc. 
+ Vì vậy ở 1 thời điểm nào đó khi ta nhấn nút, chương trình lại đang xử lý ở các vị trí khác trong chương trình ví dụ như hàm __delay()__. Thì lúc này tín hiệu từ nút nhấn sẽ không được đọc, dẫn đến những lệnh thực thi khác dựa trên tín hiệu từ nút nhấn sẽ không được thực hiện, làm sai chương trình

Trường hợp khi ta xử lý ngắt trên nút nhấn sẽ khắc phục được tình trạng trên như sau

<p align = "center">
<img src = "https://github.com/user-attachments/assets/0aef92c1-3ebc-48ab-8e79-02ded4234560" width = "400" height = 300">

+ Lúc này sẽ không cần 1 hàm kiểm tra điều kiện nút nhấn trong chương trình chính. Việc xử lý tín hiệu từ nút nhấn sẽ phụ thuộc vào 1 bộ ngắt ngoài (EXTI), và chỉ xử lý yêu cầu từ nút nhấn mỗi khi ta nhấn nút. Dẫn đến chương trình sẽ trở nên linh hoạt, và được xử lý nhanh chóng đối với các tín hiệu đến từ bên ngoài.

__Lợi ích của ngắt__ 
+ Giúp xử lý nhanh chóng, chính xác đối với các tín hiệu được gửi đến đột ngột đến từ các tác nhân từ bên trong/ngoài của vi điều khiển
+ Giúp giảm tải quá trình xử lý, làm cho chương trình hoạt động trơn tru.

__CÁC LOẠI NGẮT__
### a) Ngắt ngoài
Đây là loại ngắt được kích hoạt từ các tín hiệu bên ngoài làm thay đổi điện áp trên các chân GPIO. Chúng ta sẽ có 4 loại cấu hình để phát hiện được tín hiệu ngắt.

![image](https://github.com/user-attachments/assets/2b29ac92-43f7-46d8-9bb0-aa8c0c7b3906)

__RISING EDGE__: Tin hiệu ngắt được phát hiện khi có sự chuyển đổi trạng thái hiện áp từ LOW sang HIGH

__FALLING EDGE__: Tín hiệu ngắt được phát hiện khi có sự chuyển đổi điện áp từ HIGH sang LOW

__HIGH STATE__: tín hiệu ngắt được phát hiện khi ở mức HIGH

__LOW STATE__: tín hiệu ngắt được phát hiện khi ở mức LOW

+ Các tín hiệu ngắt trên sẽ được phát hiện và xử lý thông qua 1 bộ điều khiển ngắt ngoài 
+ Tùy vào mỗi vi điều khiển sẽ có số lượng bộ EXTI khác nhau tùy thuộc vào số lượng chân của 1 port GPIO.
+ 1 bộ EXTI sẽ gồm các chân cùng tên từ các port khác nhau.
![image](https://github.com/user-attachments/assets/fabe93d0-4f47-4a55-b990-1c6b77278348)

__QUY TRÌNH XỬ LÝ NGẮT BÊN TRONG HARDWARE__

<p align = "center">
<img src = "https://github.com/user-attachments/assets/3e99b3dd-146d-4393-b6c2-86652f110df9" width = "600" height = "300">

+ __Bước 1__: bộ EXTI phát hiện tín hiệu ngắt trên 1 chân GPIO nào đó, và kích hoạt __IRQ_Number__ tương ứng với bộ EXTI đó
+ __Bước 2__: Tín hiệu sẽ được gửi đến __NVIC__ để báo hiệu cho CPU biết có 1 ngắt xảy ra
+ __Bước 3__: CPU sẽ gọi ra hàm __ISR__ (dịa chỉ lưu trong __vector table__) tương ứng với __IRQ_Number__ để xử lý ngắt
+ __Bước 4__: 1 cờ ngắt sẽ được kích hoạt, và ta có thể dựa vào cái flag này để xử lý 1 việc nào đó, sau đó thì reset nó. 
+ __Bước 5__: reset giá trị trong __pending register__ để tránh việc chương trình phát hiện ngắt liên tục

### b) Ngắt timer
Là 1 tín hiệu được kích hoạt khi giá trị đếm trong counter register bị tràn hay nói cách khác là chạm đến giá trị đếm tối đa và sẽ được reset về giá trị ban đầu để tiếp tục đếm, mục đích là để tạo 1 khoảng thời gian delay cho timer.
![image](https://github.com/user-attachments/assets/1655e4c8-e10c-4707-8578-c3fca58702e9)
### c) Ngắt truyền thông
Là 1 tín hiệu được kích hoạt khi 1 thiết bị đã nhận/gửi dữ liệu thành công từ/đến 1 thiết bị khác. Mục đích là để đồng bộ quá trình truyền nhận dữ liệu từ 1 hoặc nhiều thiết bị, tránh được sự sai sót về dữ liệu so với truyền/nhận thông thường
![image](https://github.com/user-attachments/assets/678db955-2e09-4945-9e68-f7e704a80e17)

## 1.2 Quá trình xử lý khi phát hiện tín hiệu ngắt
+ Khi 1 chương trình đang thực thi ở vị trí __Executing__, lúc này __Program counter__ sẽ trỏ tới địa chỉ của câu lệnh tiếp theo 
+ CPU phát hiện có 1 tín hiệu ngắt, chương trình sẽ tiếp tục hoàn thành câu lệnh tại __Executing__, trong khi đó __program counter__ sẽ trỏ tới địa chỉ của ISR
+ Tuy nhiên trước khi __PC__ trỏ tới ISR(), nó sẽ lưu trữ địa chỉ của hàm trước đó mà nó trỏ đến vào trong stack pointer (__push__ vào __call stack__)
+ Sau khi ISR hoàn thành địa chỉ ở trong __stack__ sẽ được đẩy ra (__pop__) khôi phục lại vị trí địa chỉ của hàm xứ lý tiếp theo trước khi có ngắt 
![image](https://github.com/user-attachments/assets/e935eebe-0533-4791-8447-8d15e408fce1)
## 1.3 Độ ưu tiên trong ngắt
Trong 1 chương trình sẽ có nhiều tín hiệu ngắt đến từ các ngoại vi khác nhau, và độ ưu tiên của chúng sẽ quyết định cái nào được thực hiện trước.
![image](https://github.com/user-attachments/assets/361edede-003c-471e-b26f-7ae3421d25ef)
+ Hình trên mô tả việc xử lý nhiều ngắt xảy ra với độ ưu tiên khác nhau
+ Địa chỉ của các hàm ngắt cũng sẽ được lưu trữ trong __stack__ khi có ngắt có độ ưu tiên cao hơn xảy ra 

+ Sau khi ngắt có độ ưu tiên cao nhất được thực hiện xong, địa chỉ của các hàm lưu trong __stack__ sẽ được trả về để thực thi tiếp theo cơ chế __(LIFO)__
# 2. Timer
## 2.1 Khái niệm
timer là 1 bộ đếm chu kỳ xung clock để tạo ra thời gian delay, timer có thể được cấu hình để đếm lên/xuống. 

![image](https://github.com/user-attachments/assets/3c6c5573-e61a-4276-999f-55e9a28506d6)

Để cấu hình cho timer hoạt động, ta cần nắm được các tham số sau đây:

__TIM_CLK__: Nguồn clock đầu vào cấp cho timer hay nói cách khác là __PCLK__ tương với bus APB mà nó nối đến 

__ARR__: số lần đếm tôi đa, trước khi thanh ghi đếm tràn 

__Prescaler(PSC)__: số xung sinh ra để đếm lên 1 lần hay nói cách khác là giá trị để chia nhỏ tần số của clock source

__Update_event__: chu kỳ tràn, hay nói cách khác chính là khoảng thời gian delay mà ta mong muốn timer tạo ra 

![image](https://github.com/user-attachments/assets/db18e84a-aec4-46d8-9465-14d0e38b6dbb)

## 2.2 Xác định giá trị Clock cấp cho timer
Để lập trình cho timer hoạt động, ta phải biết được nguồn clock đầu vào cấp là bao nhiêu để xác định được giá trị __TIM_CLK__
![image](https://github.com/user-attachments/assets/3df7e40e-3e43-4ccc-9e1a-b8cae7c9cbdf)
__Ví dụ__: xác định giá trị __TIM_CLK__ cho timer1, với nguồn clock đầu vào là HSI trên stm32f4
+ HSI sẽ có giá trị là 16MHZ, làm clock source cho stm32
+ Lúc này __SYSCLK__ sẽ là 16MHZ, đi qua __AHBx prescaler__ với giá trị là 1, ta sẽ được clock cấp cho __HCLK__ vẫn là 16MHZ
+ Bộ IIM2 sẽ nằm ở bus APB1APB1, vì vậy sau khi clock từ __HCLK__ đi qua __APB1 pprescaler__ với giá trị 1, thì sẽ đưa vào __PCLK1__, chính là nguồn clock chính cấp cho timer1 hoạt động   

# 3. Lập trình timer chớp tắt led trên stm32f407 với thời gian 1s
+ Trước tiên ta sẽ chọn bộ timer phù hợp cho chức năng mà ta muốn làm. Vì timer sẽ được chia ra làm nhiều loại ứng với các chức năng tương ứng. Ở đây ta muốn tạo ra 1 khoảng thời gian delay, tương ứng với chức năng __Update Event__, chính vì vậy ta sẽ chọn các timer 2 -> 5
![image](https://github.com/user-attachments/assets/80f9bbef-585d-4e62-a7fb-0889d437861b)
+ Tiếp theo dựa vào bảng __register boundary aaddresses__ trong hardware manual, ta xác định được TIM2 nối đến bus APB1, Mục đích là để xác định được bus cần cấp clock cho timer hoạt động
__Cấu hình Clock cho timer và port GPIO__
![image](https://github.com/user-attachments/assets/90a76436-91aa-4997-89fb-cb49bd2e1ae6)
__Cấu hình chân led PD12__
![image](https://github.com/user-attachments/assets/9f2aadb7-97cb-4bf4-a6ab-7a9b210538cf)   
__Tính toán giá trị Update event__ 

Để tạo ra thời gian delay 1s: 
+ Ta sẽ tạo ra 1 khoảng delay cứ 10 ms thì đếm lên 1 lần với giá trị 

  prescaler = 1599
+ Thời gian đếm lên 1 lần: T_CLK = 1 / F_CLK

   F_CLK = TIM_CLK / prescaler + 1 = 16 MHZ / 1599 = 10 KHz
   
  => T_CLK = 0.1 ms
+ Giá trị period  sẽ cho đếm đến giá trị tối đa, chúng ta sẽ dùng 1 hàm khác để tạo ra thời gian 1 giây 

![image](https://github.com/user-attachments/assets/a7d350fe-d990-4980-b5ce-5eed6e372f96)

+ Ta tạo ra 1 hàm __delay_ms__, và đặt bộ đếm về 0, và tiến hành so sánh với giá trị truyền vào ở đây là time * 10 để quy đối giá trị so sánh từ 0.1ms sang 1 ms

![image](https://github.com/user-attachments/assets/8aa95b74-486b-4f8e-9406-92a862486c9d)

__Viết chương trình chớp tắt led trong hàm main__

![image](https://github.com/user-attachments/assets/84134ce3-2b5e-4c46-8c9f-c118d15a99d5)




