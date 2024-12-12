# Giao thức truyền thông 
## 1. Giao thức SPI 
<p align = "center">
<img src = "https://github.com/user-attachments/assets/2bb0d6df-899c-40e8-b7dd-6fe876522e76" width = "300" height = "150">

### 1.1 Giới thiệu về SPI
__a. Đặc điểm__

Giao thức truyền thông được phát triển bởi motorola năm 1980. Còn được gọi là giao thức truyền song công toàn phần __full duplex__ với các đặc điểm sau: 
+ giao tiếp giữa 1 master vs 1 hoặc nhiều slave
+ cơ chế đồng bộ
+ có thể truyền và nhận data dồng thời
+ Hỗ trợ tốc độ từ 1 đến vài chục mbs (nhanh hơn i2c và uart)
+ sử dụng 4 dây

__b. Ứng dụng__

SPI có thể được ứng dụng để truyền dữ liệu giữa các cảm biến trên __on-board device__, với khoảng cách ngắn 

__c. Ưu điểm__

+ Khung truyền dữ liệu đơn giản so với các giao thức khác
+ dữ liệu truyền chính xác và nhanh chóng ỏ khoảng cách ngắn
+ Không cần địa chỉ riêng biệt cho mỗi slave để master chọn giao tiếp
+ dữ liệu được truyền theo 2 chiều cùng lúc.

__d. Nhược điểm__ 

+ Sử dụng nhiều dây hơn so với các giao thức khác
+ Chỉ hiệu quả với đường truyền ngắn
+ Không có các cơ chế kiểm tra lỗi hay xác nhận quá trình truyền/nhận thành công hay chưa 
### 1.2 Kết nối phần cứng
__a) Sơ đổ chân__
<p align = "center">
<img src = "https://github.com/user-attachments/assets/1284a1fb-33f3-4674-a329-3f26ec4426fa" width = "400" height = "200">

__MOSI__ : master truyền và slave nhận 

__MISO__ : slave truyền và master nhận


__SCLK__ : kiểm soát bởi master, để tạo ra xung clock đồng bộ việc   truyền nhận data giữa 2 bên

__SS__ : chân chọn chip, master dùng chân này để xác định slave muốn giao tiếp, bằng cách kéo từ high xuống low trong 1 clk trên chân __SS__ của slave

__b) Mắc song song__ 
<p align = "center">
<img src = "https://github.com/user-attachments/assets/b35998fa-03f4-45e2-8c6a-1f2ae072165e" width = "400" height = "200">

__Ưu điểm__: xác định được slave nào bị lỗi nhanh chóng nếu quá trình truyền/nhận bị lỗi do  các slave được nối độc lập với nhau

__Nhược điểm__: tốn nhiều dây ss khi số lượng slave tăng

__c) Mắc nối tiếp__ 
<p align = "center">
<img src = "https://github.com/user-attachments/assets/3ee82686-49b8-42e0-b1c2-dc569b2e2e77" width = "400" height = "200">

__Ưu điểm__:phần cứng đon giản do Chỉ sử dụng 1 dây SS để master quản lý các slave

__Nhược điểm__: khó khăn trong việc xác định slave nào bị lỗi khi data truyền/nhận sai, do các slave được mắc nối tiếp với nhau

### 1.3 Quá trình truyền dữ liệu
<p align = "center">
<img src = "https://github.com/user-attachments/assets/36c91fa7-2898-40f3-a0b8-06c170e538bd" width = "400" height = "200">

__Bước 1__ : Master kéo chân __SS__ từ high xuống low để báo hiệu cho slave biết sẵn sàng nhận data

__Bước 2__ : master gửi 1 bit trên chân __MOSI__  kèm với 1 clock trên chân __SCK__ 

__Bước 3__: Slave kiểm tra đường __SCK__ có ở mức high hay không và tiến hành đọc 1 bit qua đường __MOSI__, đồng thời gửi 1 bit qua __MISO__

__Bước 5__: Master lặp lại quá trình cho đến khi đủ 8 bit, sau đó sẽ nhả chân __SS__  lên mức high để kết thúc quá trình truyền

### 1.4 Tham số cấu hình cho SPI
Để SPI hoạt động thì ta cần cấu hình 4 loại thông số sau:

__a) chế độ truyền__

+ SONG CÔNG (__Full-duplex__): Master sẽ vừa truyền và nhận data trong 1 thòi điểm

<p align = "center">
<img src = "https://github.com/user-attachments/assets/092df91e-97d4-4497-9765-1160ead3c8e8" width = "400" height = "200">

+ BÁN SONG CÔNG (__Half-duplex__): Master chỉ có thể truyền hoặc nhận data trong 1 thời điểm

<p align = "center">
<img src = "https://github.com/user-attachments/assets/2d508edb-ca9d-4b4a-aa15-beb7ffb853b3" width = "400" height = "200">

+ ĐƠN CÔNG (__Simplex__): Master chỉ có thể truyền mà không nhận data từ slave

<p align = "center">
<img src ="https://github.com/user-attachments/assets/10d04e7e-4476-4c7c-b567-0c6df1451777" width = "400" height = "200">

__b) CPOL và CPHA__

__SCK Phase__ : trạng thái idle (không có dữ liệu) trên chân __SCK__ 
+ __CPOL = 0__ : ở mức __low__
+ __CPOL = 1__ : ở mức __high__

<p align = "center">
<img src = "https://github.com/user-attachments/assets/30fbff03-5271-40de-8124-e9e55cba50dc" width = "300" height = "150">

__SCK Polarity__: trạng thái của clk mà slave chọn để đọc data

+ __CPHA = 0__ : ở cạnh đầu tiên của clk

<p align = "center">
<img src = "https://github.com/user-attachments/assets/018f963f-2e1a-4951-837e-df80bd7f4e89" width = "300" height = "150">

+ __CPHA = 1__ : ở cạnh tiếp theo của clk

<p align = "center">
<img src = "https://github.com/user-attachments/assets/4e116f1e-b2d0-4b60-b2e9-68c97cb6bd9d"width = "300" height = "150">

__c) Tốc độ__

phụ thuộc vào nguồn clock đầu vào như HSE/HSI/PLL và qua bộ chia tầng của bus mà nó nối đến

<p align = "center">
<img src = "https://github.com/user-attachments/assets/7c83f489-7e59-496e-932d-de1da97855d2" width = "300" height = "150">

__d) Slave management__

Chân CS trong spi sẽ được cài đặt ở 1 trong 2 mode 

__+ Hardware__ : Chân CS sẽ hoàn toàn được điều khiển bởi phần cứng 

__+ Software__ : Chân CS có thể được kiểm soát bởi phần mềm   

# 2. Giao thức I2C 

<p align = "center">
<img src ="https://github.com/user-attachments/assets/9263d619-0f5d-442f-a9ea-90d45500e926" width = "400" height = "200">

## 2.1 Giới thiệu về I2C 

__a. Đặc điểm__

Giao thức truyền thông nối tiếp, với 1 số đặc điểm:
+ cơ chế đổng bộ
+ truyền bán song công
+ hỗ trợ giao tiếp 1 hoặc nhiều master với 1  hoặc nhiều slave
+ Sử dụng địa chỉ để giao tiếp giữa master và slave
+ tốc độ dao động từ 100 - 400Khz (chậm hơn SPI và nhanh hơn UART)
+ sử dụng 2 dây : SDA -> truyền data , SCL -> tạo xung clock

__b. Ưu điểm__

+ Chỉ sử dụng 2 dây 
+ tích hợp cơ chế gửi bit ACK/NACK để xác nhận quá trình truyền/nhận data
+ Khoảng cách truyền lớn hơn so với spi

__c. Nhược điểm__
 
+ Khung truyền phức tạp hơn so với spi và uart
+ Phụ thuộc vào địa chỉ để giao tiếp giữa các thiết bị, dẫn đến xung đột nếu 1 số thiết bị có địa chỉ giống nhau

__d. Cấu hình Open-drain trong I2C__

Trong 1 bus i2c, các thiết bị slave khi kết nối thường sẽ không thể tự điều khiển trạng thái trên chân của nó mà phải phụ thuộc vào master để kéo chân xuống mức __LOW__ để bắt đầu giao tiếp. Chính vì vậy chúng sẽ được đặt ở trạng thái là __open-drain__ nghĩa là điện áp trên chân sẽ được thả nổi hoặc kéo xuống __LOW__. Mà vi điều khiển (__master__) thường sẽ không hiểu được mức tín hiệu điện áp không cụ thể để điều khiển slave. Chính vì vậy các slave đều sẽ được mắc lên i2c bus kèm với __điện trở pull-up___ để luôn giữ chúng ở mức __HIGH__ 

<p align = "center">
<img src ="https://github.com/user-attachments/assets/9cc210e8-8f9c-49fc-851b-39d36a1f3526" width = "400" height = "200">

 __=>__ Nhờ đó khi master đã giao tiếp xong với slave,nó sẽ nhả chân sda vs scl của slave đó về mức __high__ trở lại nhờ cơ chế __open drain__ và __điện trở pull-up__ chứ slave hoàn toàn không tự điều khiển được điện trên chân cửa nó   

## 2.2 Quá trình truyền I2C 

### 2.2.1 Khung dữ liệu

<p align = "center">
<img src = "https://github.com/user-attachments/assets/100bceb3-865a-450e-989b-5b912f79eb79" width = "500" height = "250">

__Start Condition__: điều kiện bắt đầu được gửi bởi master để bắt đầu quá trình giao tiếp i2c

__Address Frame__: địa chỉ của slave và master muốn giao tiếp

__Read/Write bit__ : xác nhận master muốn truyền hay nhận data từ slave
 
__ACK/NACK bit__ : xác nhận quá trình truyền nhận có thành công hay chưa 

__Data Frame__: dữ liệu thực tế được truyền/nhận giữa master và slave

__Stop Condition__ : điều kiện kết thúc quá trình giao tiếp, được gửi bởi master

### 2.2.2 Các bước truyền
<p align = "center">
<img src = "https://github.com/user-attachments/assets/36f97bca-08a3-43e6-96fc-79d288b4f90a" width = "600" height = "200">

__Bước 1__: master gửi điều kiện __start__ bằng cách kéo đường __SDA__ xuống mức thấp, theo sau đó là __SCL__

__Bước 2__: master gửi 7 bit địa chỉ kèm với 1 bit read __(yêu cầu dữ liệu từ slave)__ hoặc 1 bit write __(gửi dữ liệu đến slave)__

__Bước 3__: slave  so sánh địa chỉ của nó có trùng với địa chỉ mà master gửi hay không , nếu có phản hồi bằng 1 bit __ACK__ bằng cách kéo đường __SDA__ xuống __LOW__, nếu không trùng thì không phản hồi tương ứng với bit __NACK__ trên đường __SDA__vẫn ở mức __high__

__Bước 4__; master sau khi nhận bit __ACK__ trên __SDA__ , sẽ gửi 8 bit data 

__Bước 5__: slave sau khi nhận xong data sẽ phản hồi 1 bit__ACK__, báo cho master biết nó sẵn sàng nhận tiếp data

__Bước 6__: Quá trình truyền lặp đi lặp lại đến khi toàn bộ dữ liệu được gửi xong, lúc này slave sẽ không phản hồi bit __ACK__, đường __SDA__được giữ ở mức __high__, lúc này master hiểu rằng slave không muốn nhận data nữa và nó sẽ gửi điều kiện __Stop__ để kết thúc quá trình giao tiếp với slave

# 3. Giao thức UART
## 3.1 Giới thiệu về UART

<p align = "center">
<img src = "https://github.com/user-attachments/assets/f043e57b-4525-4e2a-8616-e454c16f9e08" width = "600" height = "200">
##3.1 Giới thiệu về UART

__a. Đặc điểm__

Giao thức truyền nối tiếp giữa 2 thiết bị với các đặc điểm:
+ cơ chế không đồng bộ
+ truyền song công 
+ sử dụng 2 dây là Tx và Rx
+ Tốc độ thường là các giá trị cụ thể như: 9600, 115200...

__b. Ưu điểm__
+ chỉ sử dụng 2 dây
+ khoảng cách truyền xa hơn so với i2c và spi
+ Có tích hợp cơ chế kiểm tra lỗi bit parity

__c. Nhược điểm__
+ Chỉ giao tiếp được giữa 2 thiết bị
+ Cả 2 thiết bị cần phải có cấu hình tốc độ giống nhau 

__d. Tốc độ__

+ Để quá trình giao tiếp uart được hình thành thì cả 2 thiết bị sẽ thống nhất với nhau về tốc độ truyền được gọi là __baudrate__, hay nói cách khác thời gian để truyền đi 1 bit. Ví dụ ta __baudrate = 9600 mbs__ thì ta tính được thời gian để 1 bit truyền đi như sau

<p align = "center">
<img src = "https://github.com/user-attachments/assets/898b7614-cc5b-46f6-bb43-85c6328b3a8f" width = "600" height = "200">


## 3.2 Quá trình giao tiếp 

### 3.2.1 Khung dữ liệu UART
Dữ liệu uart sẽ được chuẩn hóa thành 1 packet và truyền đi dưới dạng song trên chân tx của thiết bị gửi, sau khi đến thiết bị nhận trên chân rx, dử liệu sẽ được chuyển đổi sang dạng nối tiếp 

<p align = "center">
<img src = "https://github.com/user-attachments/assets/3ae4c65c-e005-478b-8c5d-cb04b2d8a972" width = "600" height = "200">

### 3.2.2 Các bước truyền UART

thiết bị gửi sẽ gửi 1 bit start bằng cách kéo đường Tx sau đó 5 - 9 bit data gửi đến cho thiết bị nhận kèm với 1 bit chăn lẻ để kiểm tra data có bị sai hay không, cuối cùng là 1 đến 2 stop bit được gửi__(tùy thuộc vào cấu hình phần cứng mà sẽ quyết định số lượng stop bit__) để kết thúc quá trình giao tiếp giữa 2 thiết bị.

### 3.2.1 Cơ chế kiểm tra lỗi 

UART sử dụng bit parity để kiểm tra liệu dữ liệu gửi và nhận từ 2 bên có giống nhau hay không

<p align = "center">
<img src = "https://github.com/user-attachments/assets/2727917d-425d-4c41-bd3a-3aa289b015bc" width = "600" height = "200">

Cả 2 thiết bị sẽ được cấu hình giống nhau ở 1 trong 2 quy luật sau:

__Quy luật chẵn__: đảm bảo tổng số lượng bit 1 trong 8 bit dữ liệu và bit parity là số chẵn 


__Quy luật lẽ__: đảm bảo tổng số lượng bit 1 trong 8 bit dữ liệu và bit parity là số lẻ

__Nhược điểm__: bit parity chỉ hiệu quả khi số lượng bit 1 trong 8 bit data là số lẻ như 1,3,5,... nhưng khi số lượng bit 1 là số chẳng như 2,4,6,... thì bit parity sẽ không nhận biết được liệu dữ liệu truyền có không 















