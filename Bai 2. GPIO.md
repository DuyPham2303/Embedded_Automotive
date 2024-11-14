# BÀI 2: GPIO

## 1. Khởi tạo project trên KeilC
+ Đầu tiên ta mở KeilC chọn vào tab như hình 
<p align = "center">
<img src = "https://github.com/user-attachments/assets/14e71286-6bae-4256-b076-d97c7d019cb8" width = "600" height = "300">

+ Tạo sẵn 1 folder chứa project và đặt tên 

<p align = "center">
<img src = "https://github.com/user-attachments/assets/b01e2987-4833-40a6-9d87-d1db3bbf946f" width = "600" height = "300">

+ Chọn dòng vdk, ở đây mình sử dụng con stm32f103c8t6, sau đó click ok

<p align = "center">
<img src = "https://github.com/user-attachments/assets/db00d149-c95c-445c-8fe6-3d5045a4b567" width = "600" height = "300">

+ Ở trong mục sau ta chọn các file sẽ generate ra sau khi hoàn tất cài đặt.Đầu tiên ta click chọn Startup, và click vào tab Stdperiph Drivers

<p align = "center">
<img src = "https://github.com/user-attachments/assets/451a9e75-875f-413f-9d59-0b2a38b60334" width = "600" height = "300">

+ Trong tab Stdperiph Drivers, ta chọn như bên dưới, sau đó click vào ô Resolve, để hệ thống tự động cập nhật chọn những file cần thiết cho project

<p align = "center">
<img src ="https://github.com/user-attachments/assets/ddff8eab-0bfc-48a8-a690-f43d3d60b594" width = "600" height = "300">

## 2. Include thư viện và cấu hình debug
### 2.1 Include thư viện
+ Sau khi khởi tạo project thành công ta tạo 1 file main.c bằng cách sau

<p align = "center">
<img src ="https://github.com/user-attachments/assets/6ce5a9b1-ded1-4519-a37e-483422eba43d" width = "600" height = "300">

+ Ta chọn vào file muốn tạo, và đặt tên 

<p align = "center">
<img src ="https://github.com/user-attachments/assets/d25e8b56-aa37-4fd9-a022-e2997b1887d2" width = "600" height = "300">

+ TIếp theo ta thực hiện include các thư viện cần thiết cho project

<p align = "center">
<img src = "https://github.com/user-attachments/assets/f4d66af0-9302-46d6-8694-a3237b0dd603" width = "600" height = "300">

### 2.2 Cấu hình debug
+ Chọn vào tab option for target trên thanh công cụ, và chọn bộ compiler mặc định phù hợp với version hiện tại của KeilC

<p align = "center">
<img src = "https://github.com/user-attachments/assets/05bf215f-34a9-4c51-b42d-27f1a2b2a21f" width = "600" height = "300">

+ Chọn vào tab C/C++ và click chọn C99 Mode

<p align = "center">
<img src = "https://github.com/user-attachments/assets/a550241d-bfef-42e4-86e6-853fbd8197e6" width = "600" height = "300">

+ Trong mục debugger, chọn vào ST-Link debugger, sau đó ta click vào setting để tiếp tục chỉnh sửa 

<p align = "center">
<img src = "https://github.com/user-attachments/assets/f1bb49ba-8e6a-4aea-b5e5-0d626749fdb8" width = "600" height = "300">

+ chọn Flash Download -> Reset and Run

<p align = "center">
![image](https://github.com/user-attachments/assets/53eecc2d-86d3-4584-8d68-e5151e547c62" width = "600" height = "300">

+ Tiếp theo ta chọn Debug -> click vào 2 ô như hình

<p align = "center">
<img src = "https://github.com/user-attachments/assets/7d1adb69-631f-4dbb-aecf-8ad8e654cda9" width = "600" height = "300">

+ Ta quay trở lại chương trình và bấm vào run -> chương trình sẽ trả về kết quả như màn hình khi ta cấu hình đúng

<p align = "center">
<img src = "https://github.com/user-attachments/assets/2f3b47a2-0ed9-4e93-9c66-c76e8b3bf5c6" width = "600" height = "300">

## 3. Các bước để cấu hình GPIO
Để thao tác với các chân GPIO trên stm32f1, thì ta cần phải xác định được
+ __GPIO Port__: ví dụ ta muốn cấu hình chân PC13, thì có nghĩa là chân đó thuộc port GPIOA và có thứ tự là 13 
+ __Bus truyền data__: ta sẽ dựa vào block diagram của stm32f1 để xác định .Dựa vào ảnh dưới đây ta xác định được GPIOC sẽ thuộc bus APB2

<p align = "center">
<img src = "https://github.com/user-attachments/assets/c4f18309-c9d2-41b5-a09d-f313da0d0125" width = "600" height = "300">

### 3.1 Cấu hình Clock
+ để bất kỳ ngoại vi nào hoạt động được ta sẽ phải cấp xung clock cho nó. Trong KeilC, ta sẽ tạo 1 hàm để cấp clock cho GPIOC như sau
+ Ta gọi hàm dùng để cấp clock và truyền vào 2 tham số lần lượt là macro trỏ đến thanh ghi quản lý clock của GPIOC, và macro ENABLE cho phép kích hoạt clock. 

<p align = "center">
<img src = "https://github.com/user-attachments/assets/d1ab1326-aaab-4da1-a0e9-902720ca61d1" width = "800" height = "50">

### 3.2 Cấu hình GPIO
+ Ta sẽ tìm đến thư struct sau đây để cấu hình cho 1 chân GPIO, gồm có 3 giá trị pin,mode,speed

<p align = "center">
<img src = "https://github.com/user-attachments/assets/7543c5b3-a5d1-43ac-bceb-79c3d79c8c2b" width = "700" height = "300">

+ Trong file main.c ta tạo 1 hàm để cấu hình cho chân pin13 thuộc GPIOC như sau

<p align = "center">
<img src = "https://github.com/user-attachments/assets/25358ba5-6113-43a8-8246-2dea78bf2fcf" width = "500" height = "250">

## 4. Lập trình ứng dụng GPIO
### 4.1 Nháy led pc13 
+ Trong hàm main,ta sẽ gọi ra các hàm đã viết trong phần cấu hình
+ Trong vòng while(1) ta sẽ sử dụng 2 hàm set và reset giá trị bit thứ 13 trên port GPIOC, và delay 1 khoảng nhất định để tạo hiệu ứng nhấp nháynháy 

<p align = "center">
<img src ="https://github.com/user-attachments/assets/35bf36d4-b849-48ba-9a09-eafa6c6028be" width = "700" height = "500">

### 4.2 Led nháy đuổi
+ Trong ví dụ dưới đây ta sẽ lâp trình để sáng tắt led lần lượt trên 4 chân từ 2 -> 5 của GPIOA 

<p align = "center">
<img src = "https://github.com/user-attachments/assets/eecb8dfa-25cc-4d8b-86cb-e80d05d22e0d" width = "700" height = "500">

+ Để quan sát rõ hơn trạng thái các chân này bên trong vi điều khiển hoạt động như thế nào, ta sẽ làm như sau

<p align = "center">
<img src ="https://github.com/user-attachments/assets/47545dd9-dc55-4b5a-b3e0-8b9822ea1846" width = "600" height = "400">

### 4.3 điều khiển trạng thái led với nút nhấn
+ Đầu tiên ta sẽ cấu hinh chân đọc tín hiệu từ nút nhấn, cũng như chân để sáng led 

<p align = "center">
<img src = "https://github.com/user-attachments/assets/0cd02ddc-8ee2-40c6-8cfd-e4bab0467bdd" width = "600" height = "300">

+ Trong hàm main ta thực hiện xử lý việc đọc nút nhấn và điều khiển led pc13 như sau

<p align = "center">
<img src ="https://github.com/user-attachments/assets/ead3bdcb-a7c6-41d7-9106-95a602c1c50b" width = "600" height = "300">







