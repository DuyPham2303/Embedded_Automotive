# 1. Lập trình UART bằng hardware API 
Quá trình truyền nhận sau đây sẽ được thực hiện thông qua stm32f103c8t6 
## 1.1 Khởi tạo các macro,hàm, thư viện cần thiết 
Đầu tiên ta xác định các chân và bus của bộ USART sẽ sử dụng

<p align = "center">
<img src = "https://github.com/user-attachments/assets/2ea5dd5f-3d53-406a-b585-b93bef05d38f" width = "400" height = "300">

Tiếp theo ta include các thư viện và khai báo các macro, function protype cần thiết

<p align = "center">
<img src = "https://github.com/user-attachments/assets/e6108bf7-075a-4d84-b47b-78d466461578" width = "500" height = "300">

Ta tiến hành kích hoạt clock cho USART và các ngoại vi  

<p align = "center">
<img src = "https://github.com/user-attachments/assets/fa707b87-8581-49d9-8869-fb3effd21737" width = "500" height = "100">

Ta cũng khởi tạo timer để tạo ra hàm delay cho việc truyền uart 

<p align = "center">
<img src = "https://github.com/user-attachments/assets/fceeb4ac-8cc7-48d6-aafa-77af91b4515a" width = "400" height = "250">

## 1.2 khởi tạo hàm config cho UART
Ta tiến hành init các chân GPIO cho bộ UART và các chân khác sẽ sử dụng cho các mục đích sau này 
+ Chân Tx được cấu hình ở chế độ Ouput AF để lái nó sang chức năng UART
+ Chân Rx được cấu hình ở chế độ Input Floating để tránh can thiệp vào tín hiệu gửi từ Tx, không bị xung đột dữ liệu và giảm tối ưu điện năng hoat động  

<p align = "center">
<img src = "https://github.com/user-attachments/assets/458bd66e-e890-4168-93a5-6b0dedd98af2" width = "600" height = "300">

Tiếp theo ta khởi tạo các thông số cho UART 

<p align = "center">
<img src = "https://github.com/user-attachments/assets/3c8ba82f-2ac1-43f3-ae0f-3c7cc75b1db3" width = "600" height = "250">

## 1.3 Hàm truyền UART
__Hàm truyền 1 byte__
+ Kiểm tra và đợi đến khi thanh ghi data có empty 
+ Tiến hành gửi data lên thanh ghi
+ Đợi đến khi quá trình truyền thành công thì dừng gửi  
__Hàm truyền nhiều byte__
+ Ta sẽ truyền từng byte 1 vào hàm truyền 1 byte và dùng tham số len để xác định số byte cần gửi
<p align = "center">
<img src = "https://github.com/user-attachments/assets/8e491ea8-7c48-4da6-a272-c5e5e6de315d" width = "600" height = "250">

__Ví dụ:__ Ta sẽ truyền data lên máy tính thông qua module usbttl và sử dụng phần mềm hercules để quan sát data

<p align = "center">
<img src = "https://github.com/user-attachments/assets/0bbba376-1ac0-4f21-9fd4-f8aec3818ddf" width = "400" height = "150">

+ Trong chương trình chính ta sẽ truyền chuỗi hello world kèm với biến cnt tăng dần sau mỗi 1s  

<p align = "center">
<img src = "https://github.com/user-attachments/assets/e2ea2232-b193-470c-93f0-6db13dd07dc1" width = "400" height = "250">

+ Ta mở phần mềm hercules chọn tab serial -> cổng COM -> baudrate (phải trùng với cài đặt trên code) -> open 

<p align = "center">
<img src = "https://github.com/user-attachments/assets/1dfbbccd-6c54-42db-bf06-13830cc9e62c" width = "500" height = "350">

## 1.4 Hàm nhân UART
+ Trước khi đọc data thì ta phải đợi đến khi cờ RXNE được set nghĩa là thanh ghi có data
+ Ta tiến hành đọc data
+ Ta lưu vào 1 biến và trả về

<p align = "center">
<img src = "https://github.com/user-attachments/assets/80d62221-2a71-4de1-806e-85b707c9749d" width = "500" height = "200">

__Ví dụ:__ Ta sẽ điều khiển trạng thái các chân PA0,PA1 thông qua việc gửi các command, sau đó UART sẽ nhận và xử lý set hoặc reset tương ứng với command gửi đến

+ Đầu tiên ta sẽ tạo ra 2 mảng chứa các cmd và buffer để lưu data nhận về từ hercules
+ mảng control_cmd được dùng để so sánh với data nhận về từ terminal và thực hiện các lệnh xử lý
+ mảng status_cmd được dùng để gửi ngược lại cho terminal

<p align = "center">
<img src = "https://github.com/user-attachments/assets/c2884398-2d65-4bf7-8f35-1c9b846a2a4f" width = "800" height = "100">

+ Hàm xử lý nhận data từ hercules, và thực hiện các xử lý tương ứng 

<p align = "center">
<img src = "https://github.com/user-attachments/assets/119e8036-2774-45c5-8498-44af2026c5d3" width = "700" height = "500">

+ Trong chương trình chính, ta gọi ra hàm trên như sau

<p align = "center">
<img src = "https://github.com/user-attachments/assets/d3f35fb3-650e-4f85-96e1-7adc41806f2f" width = "500" height = "200">

+ Kết quả khi gửi các cmd trên hercules và nhận về đồng thời set hoăc reset các chân PA0 và PA1

<p align = "center">
<img src = "https://github.com/user-attachments/assets/af8d4900-9994-4add-baac-3baa3e7d9424" width = "700" height = "400">

+ Ký tự <LF> được gửi sau mỗi cmd được dùng để xuống dòng 






