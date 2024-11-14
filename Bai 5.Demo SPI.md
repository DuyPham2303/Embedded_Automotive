# Lập trình SPI sử dụng hardware và Software 
Trong bài này chúng ta sẽ lập trình để 2 MCU stm32f1 và stm32f4 giao tiếp với nhau. Cụ thể hơn là stm32f4 sẽ gửi 1 chuỗi bất kỳ, stm32f1 sẽ nhận và gửi ngược lại chuỗi đó.
## 1. Sử dụng hardware API 
### 1.1 Cấu hình stm32f4(master)
Đầu tiên ta tạo các macro để định nghĩa cho các chân

<p align = "center">
<img src = "https://github.com/user-attachments/assets/466d2c9e-d809-4c34-b4ad-6a0d5081f437" width = "600" height = "200">

Tiếp theo ta sẽ cấu hình các chân GPIO cho SPI

+ __GPIO_Mode:__ ta cấu hình cả 4 chân là AF (chức năng thay thế), để lái sang chức năng khác ngoài chức năng IO cơ bản
+ __GPIO_PuPd:__ Ta không quản lý mức logic cụ thể của các chân mà sẽ được điểu khiển hoàn toàn bằng bộ SPI mà ta cấu hình
+ __GPIO_PinAFConfig():__ hàm này sẽ được sử dụng kích hoạt các chân GPIO hoạt động được ở chế đọ SPI

<p align = "center">
<img src = "https://github.com/user-attachments/assets/95158463-cb50-4c98-af8a-1d42f32a92e0" width = "600" height = "200">

Kế đến ra sẽ cấu hình các tham số SPI 
+ Ta sẽ chọn chế độ __NSS_Soft__ nghĩa là ta sẽ không cần quản lý chân CS bằng chân GPIO mà sẽ được MCU toàn quyền điều khiển. 

<p align = "center">
<img src = "https://github.com/user-attachments/assets/fe2f1186-3064-4ff7-8fe9-4e941b976c14" width = "700" height = "250">

### 1.2 Hàm truyền SPI

<p align = "center">
<img src = "https://github.com/user-attachments/assets/0de24230-6c03-45d7-882d-ceaee6dad164" width = "700" height = "250">

Đầu tiên ta sẽ viết hàm để nhận 1 byte với các bước 

+ Gửi tín hiệu NSS reset để bắt đầu quá trình truyền 
+ Đợi đến khi thanh ghi buffer truyền TXE không có data bên trong -> cở TXE được set 
+ Truyền 1 byte 
+ Đợi đến khi quá trinh truyền hoàn tất -> cờ BSY được reset 
+ gửi tín hiệu NSS set kết thúc quá trình truyền 

### 1.3 Cấu hình stm32f1(slave)


