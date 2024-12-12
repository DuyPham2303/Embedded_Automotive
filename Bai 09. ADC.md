# 1. TỔNG QUAN VỀ ADC
## 1.1 Khái niệm
ADC Là bộ chuyển đổi tín hiệu analog__(tín hiệu tương tự)__ sang digital__(tín hiệu số)__
<p align = "center">
<img src = "https://github.com/user-attachments/assets/cdb3da39-7b5a-465c-a100-a7d7ab3953b1" width ="600" height = "300">

+ Analog Là tín hiệu dạng sóng dao động ở nhiều mức tín hiệu điện áp khác nhau, và liên tục biến đổi theo thời gian. Thường thấy ở các tín hiệu về âm thanh,nhiệt độ môi trường
+ Thường thì Tín hiệu ADC sẽ được chuyển đổi sang Digital để MCU có thể hiểu và xử lý được

## 1.2 Các bước chuyển đổi tín hiệu analog sang digital

<p align = "center">
<img src = "https://github.com/user-attachments/assets/a8e14492-4929-4919-9f2b-d3a60dbc0379" width ="600" height = "300">

+ MCU sẽ chia điện áp tương tự đầu vào thường là 3.3 hoặc 5v thành các 
mức khác nhau __(mỗi mức sẽ được biểu ở các bit khác nhau)__ 
+ Tín hiệu analog được chuyển thành các tín hiệu rời rạc và phân bố đều trên các mức điện áp đã được chia nhỏ  
+ Mỗi khi ta thực hiện lấy mẫu thì nếu giá trị đó gần mức điện áp nào nhất thì nó sẽ trả về đúng mức điện áp đó



<p align = "center">
<img src = "https://github.com/user-attachments/assets/02ae02db-98f3-44e6-a94d-7aa56cc02e2d" width ="500" height = "250">

__Resolution:__ só bit biểu diễn các mức điện áp khác nhau được chia, độ phân giải càng lớn thì giá trị adc đọc được càng chính xác, sai số càng nhỏ
__Sampling frequency:__ quy định số lần lấy mẫu trong 1 khoảng thời gian, tần số càng cao, thì lấy được càng nhiều mẫu, kết quả càng chính xác.

## 1.3 Hoạt động trên MCU

<p align = "center">
<img src = "https://github.com/user-attachments/assets/2f99098c-3359-4d21-a8bb-924e164c9876" width = "600" height = "350">

+ Tín hiệu ADC có thể được đọc thông qua việc cấu hình các chân GPIO
+ Tiếp theo tín hiệu sẽ đi qua Khối ADC mux bát đầu quá trình chuyển đổi
+ Việc chuyển đổi được kích hoạt sử dụng các nguồn trong hoặc ngoài
+ Tín hiệu sau đó được đưa vào bộ ADC với các tùy chọn chuyển đổi là regular hay injected 
+ Kết quả được lưu vào các thanh ghi tương ứng với tùy chọn chuyển đổi 
+ Ta đọc ra các giá trị này thông qua các bus 

# 2. Sử dụng ADC trên stm32f103
+ Tín hiệu analog sẽ được đọc tùy thuộc vào 2 kiểu sau
<p align = "center">
<img src = "https://github.com/user-attachments/assets/3ed4e9d2-16d7-4046-a843-13d288f0ddb8" width = "350" height = "200">

__Regular conversion:__ gồm các thành phần

+ Single: đọc 1 kênh duy nhất, và chỉ đọc khi yêu cầu
+ Single continuous: Cũng đọc 1 kênh nhưng đọc liên tục sau khi gọi (thích hợp khi kết hợp với DMA)
+ Scan multi-channel: đọc nhiều kênh liên tiếp, vá chỉ đọc khi yêu cầu
+ Scan continuous multi-channel reapeat: đọc nhiều kênh liên tiếp và lặp lại liên tục

__Injected conversion:__ 
+ Được dùng trong trường hợp đọc nhiều kênh, và được cấu hình cho kênh có mức độ ưu tiên cao hơn được hoạt động trong khi những kênh khác tạm ngưng
## 2.1 Các thông só cài đặt 

### a) Cài đặt chung 
<p align = "center">
<img src = "https://github.com/user-attachments/assets/e163fcb4-8372-4564-b6cd-0d86e53c5145" width = "600" height = "300">

__+ ADC_Mode:__ Xác định chế độ hoạt động của ADC là đọc 1 kênh __Independent__ hay nhiều kênh __multi__

__+ ADC_ScanConvMode:__ Kích hoạt việc đọc nhiều kênh hay không

__+ ADC_ContinuousConvMode:__ Kích hoạt việc đọc liên tục hay không

__+ ADC_ExternalTrigConv:__ kích hoạt việc chuyển đổi từ nguồn ngoài

__+ ADC_NbrOfChannel:__ Số lượng kênh chuyển đổi

__+ ADC_DataAlign:__ Căn lề trái hoặc phải, trường hợp căn lề phải các bit được lưu vào bên phải thanh ghi DR, ngược lại với trường hợp căn lề trái

### b) Cài đặt cho mỗi kênh 
+ Ta sử dụng hàm sau để cài đặt cụ thể cách hoạt động của mõi kênh ADC

```bash
void ADC_RegularChannelConfig(ADC_TypeDef* ADCx, uint8_t ADC_Channel, uint8_t Rank, uint8_t ADC_SampleTime)
```
__+ADCx:__ Bộ ADC sẽ sử dụng

__+ADC_Channel:__ Kênh ADC thực hiện chuyển đổi 

__+Rank:__ thứ tự ưu tiên chuyển đổi 

__+SampleTime:__ Thời gian lấy mẫu

## 3. Đọc giá trị biến trở sử dụng ADC
### a) Các bước thực hiện

<p align = "center">
<img src = "https://github.com/user-attachments/assets/ed12b560-4ec3-4806-9b6d-fd0988bfccfe" width = "450" height = "200">

+ Đầu tiên ta định nghĩa các macro cho các thành phân của ADC

<p align = "center">
<img src = "https://github.com/user-attachments/assets/a1526bde-6bea-4399-bd79-3217ca0fe8fe" width = "350" height = "100">

+ Ta cấu hình chân GPIO để đọc tín hiệu analog

<p align = "center">
<img src = "https://github.com/user-attachments/assets/0e48e48b-27a8-416e-8f0d-a077ab6a2de1" width = "550" height = "250">

+ Ta cấu hình cụ thể các thông số của ADC
+ Ta cấu hình ADC ở chế độ đọc liên tục do đó ta sẽ gọi hàm __ADC_SoftwareStartConvCmd__ 1 lần duy nhất 

<p align = "center">
<img src = "https://github.com/user-attachments/assets/7020f2fb-0420-463f-94f8-3be8d1542f17" width = "750" height = "300">

+ Trong chương trình chính ta đọc ra giá trị ADC, thực hiện xoay biến trở ta sẽ đọc về những giá trị ADC khác nhau tương ứng với dải điện áp từ 0 - 3.3 hoặc 5v  

<p align = "center">
<img src = "https://github.com/user-attachments/assets/3f39c5b4-906c-4ac2-9e46-1871172d4065" width = "500" height = "200">

### b) Các phương pháp giảm nhiễu 
+ Ta biết rằng tín hiệu analog luôn biến thiên theo thời gian do sự thay đổi các mức điện áp trong MCU bị gây ra bởi nhiễu bởi các tác nhân từ môi trường, vì vậy giá trị đọc về sẽ không bao giờ cố định mà dao động liên tục. Do đó ta sẽ sử dụng 2 phương pháp sau để chống nhiễu

__LẤY GIÁ TRỊ TRUNG BÌNH__

<p align = "center">
<img src = "https://github.com/user-attachments/assets/af09c67b-2951-45e0-ac2b-a098a18a8f2f" width = "500" height = "300">

+ Ta sẽ thực hiện đọc về giá trị của 10 lần đo và lấy giá trị trung bình và đọc về kết quả này. 
=> Cách này sẽ không hiệu quả và kém chính xác khi ta cần đọc về kết quả sau số lần lấy mẫu nhất định, vì cách trên đã chia nhỏ ra số lần lấy mẫu ban đầu

__SỬ DỤNG BỘ LỌC KALMAN FILTER__

+ Bộ lọc kalman sẽ dự đoán trạng thái ổn định đầu ra của tín hiệu analog hiện tại và xuất ra tín hiệu analog sau khi đã được tinh chỉnh và không còn nhiễu 

<p align = "center">
<img src = "https://github.com/user-attachments/assets/888a0517-f5c1-45f6-9d34-414913b202ac" width = "500" height = "300">

<p align = "center">
<img src = "https://github.com/user-attachments/assets/76b2a2cc-a9b4-4b1f-a79f-40769f90e309" width = "500" height = "300">

+ Ta có 2 hàm sau đây để khởi tạo các hệ số tinh chỉnh cho bộ lọc kalman cùng với hàm trả về kết quả

<p align = "center">
<img src = "https://github.com/user-attachments/assets/1d9047e5-3126-4065-bf3e-ce198c539a55" width = "500" height = "300">

__mes_e:__ Độ lệch hay sai số của cảm biến được sử dụng để đọc về giá trị ADC

__est_e:__ Độ lệch chuẩn thể hiện mức độ tin cậy của tín hiệu đầu ra, thường được để ở giá trị cao ban đầu khi chưa xác định được giá trị tin cậy chính xác

__q_e:__ độ nhiễu của quá trình ảnh hưởng đến tốc độ thích nghi của bộ lọc, thường được để ở mức thấp và tăng dần để giảm bớt nhiễu 

__+ Phương pháp tinh chỉnh__
+ Nếu bộ lọc phản ứng quá chậm -> tăng q
+ Nếu giá trị không ổn định -> giảm mes_e và __est_e

Ta sử dụng bộ lọc kalman trong chương trình chính như sau

<p align = "center">
<img src = "https://github.com/user-attachments/assets/e476a2c3-7200-44a7-baf9-6c2697ac6429" width = "500" height = "300">








