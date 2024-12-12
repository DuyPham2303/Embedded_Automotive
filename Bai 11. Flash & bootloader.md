# 1. Lập trình FLASH
## 1.1 Tổng quan về Flash

<p align = "center">
<img src = "https://github.com/user-attachments/assets/816ea620-a2b7-4e16-9020-b398d4f46b4d" width = "600" height = "350">

## 1.2 Cấu trúc của bộ nhớ Flash trong stm32f103

<p align = "center">
<img src = "https://github.com/user-attachments/assets/6956ee23-ddc0-42b6-bf89-642e4c1976b1" width = "600" height = "250">

__Ta có thể sử dụng cách sau để xác định được kích thước bộ nhớ flash trên stm32f103__

<p align = "center">
<img src = "https://github.com/user-attachments/assets/872e2809-d5d6-4626-8b1e-8a80b05b92e9" width = "600" height = "250">

## 1.3 Phạm vi lập trình FLash 

<p align = "center">
<img src = "https://github.com/user-attachments/assets/8c3b702c-3d67-495d-9d61-aee45a180d21" width = "600" height = "300">

## 1.4 Quy trình đọc và ghi Flash
+ Để ghi giá trị vào trong flash thì trước hết ta phải tiến hành xóa flash với quy trình như sau

<p align = "center">
<img src = "https://github.com/user-attachments/assets/1f789b52-cfea-4328-9947-bcdec1b7962d" width = "600" height = "300">

__+ Hàm xóa flash__
<p align = "center">
<img src = "https://github.com/user-attachments/assets/e32128da-1945-499b-8b4b-791af9705e1e" width = "600" height = "100">

+ Tiếp theo ta sẽ tiến hành ghi giá trị vào trong flash với quy trình sau

<p align = "center">
<img src = "https://github.com/user-attachments/assets/6b5fcb39-b1e6-46b3-8c92-8bd4382def99" width = "600" height = "300">

__+ Hàm ghi và đọc ra giá trị Integer__

<p align = "center">
<img src = "https://github.com/user-attachments/assets/f5f6dfb3-1d0d-46d8-9aac-e8dbf8750c0f" width = "600" height = "100">

+ Flash chỉ ghi và đọc được theo 2 kiểu là halfword(16bit) và word(32bit), vì vậy tùy theo kích thước của giá trị mà ta sẽ chọn ghi theo kiểu tương ứng

<p align = "center">
<img src = "https://github.com/user-attachments/assets/e82d3b48-5536-4058-9791-b07301e7d919" width = "600" height = "50">

+ Để đọc ra giá trị thì ta cũng sẽ thực hiện ép lại về kiểu giá trị tương ứng mà ta muốn đọc ra, và thực hiện truy xuất giá trị tại địa chỉ đó và trả về
+ Ta sử dụng từ khỏa volatile khi ép kiểu dữ liệu trả về để đảm bảo biến trả về không bị chương trình tối ưu khi ta thao tác với 1 địa chỉ trên thanh ghi

__+ Hàm ghi và đọc ra 1 mảng__

<p align = "center">
<img src = "https://github.com/user-attachments/assets/4076fb93-3c7c-4e80-b053-8a608ed29cae" width = "600" height = "200">

+ Ta sẽ sử dụng 1 con trỏ tạm để lưu trữ 1 con trỏ kiểu uint16_t để duyệt qua từng phần tử trong mãng và thực hiện ghi 2 byte 1 lần theo kiểu halfword, 
+ Lúc này vì ta truy xuất 2 byte 1 lần nên số lần duyệt các phần tử trong mảng phải là 1 nửa số phần tử của mảng 
+ mỗi lần ghi 2 byte ta cũng phải dịch giá trị của page address 2 lần để ghi tiếp phần tử tiếp theo của mảng 

<p align = "center">
<img src = "https://github.com/user-attachments/assets/5b24a5f8-a3c4-4745-931d-9a09cd9562e4" width = "600" height = "200">

+ Để đọc ra 1 mãng đã lưu trong flash, ta cũng sẽ truyền vào địa chỉ của mảng mà ta muốn đọc về 
+ Ta cũng sử dụng 1 con trỏ tạm để trỏ tới cùng nhớ mà ta muốn đọc giá trị về 
+ ta tiến hành truy xuất vào page address mà ta đã ghi vào trước đó và đọc giá trị ra con trỏ tạm
= Ta tăng giá trị con trỏ tạm để dịch tới địa chỉ tiếp theo của mảng đọc về và lặp lại quá trình đọc cho đến khi xong

__+ Hàm ghi và đọc ra 1 giá trị kiểu float__

<p align = "center">
<img src = "https://github.com/user-attachments/assets/01569607-41aa-4983-ab92-83ec0ad3e8e2" width = "600" height = "200">

+ Ta khai báo 1 mảng uint8_t data [] có kích thước là 4 
+ Ta tiến hành ép kiểu data thành kiểu con trỏ (float*) và thực hiện truy xuất vào giá trị của nó, đồng thời gán giá trị của giá trị float vào nó
+ Ta ép lại kiểu data thành con trỏ (uint32_t*) và truy xuất vào giá trị của nó và truyền vào hàm ghi 1 word

<p align = "center">
<img src = "https://github.com/user-attachments/assets/f4eeebca-7720-42f0-a905-50afb9c54815" width = "600" height = "100">

+ Ta khai báo 1 biến 4 byte để lưu trữ giá trị đọc về trên page address 
+ Tiếp theo ta ép lại thành kiểu con trỏ (float*), đòng thời truy xuất vào giá trị của nó và trả về

## 1.5 Lập trình ghi và đọc 

### a) Khai báo các hàm,thư viện và định nghĩa các macro cần thiết

<p align = "center">
<img src = "https://github.com/user-attachments/assets/03d9e464-cb8d-41e6-bcaa-a0ad8bf66270" width = "700" height = "300">

### b) Chương trình chính 

<p align = "center">
<img src = "https://github.com/user-attachments/assets/0d05edee-36fb-4ee7-bf1f-52c02b1ec0ea" width = "700" height = "300">

### c) Kết quả 
+ Ta sẽ lần lượt ghi các biến int,float,và mảng uint8_t vào các page address 62,63,64. Kết quả đọc ra được như sau 

<p align = "center">
<img src = "https://github.com/user-attachments/assets/997b258d-efd6-4691-a983-6e71719a0236" width = "700" height = "300">

# 2. BOOT LOADER

## 2.1 Tổng quan
bootloader là 1 chương trình nhỏ bên trong flash sẽ được chạy tự động khi MCU được cấp nguồn và làm những nhiệm vụ sau: 
+ Khởi tạo môi trường để chạy hệ điều hành 
+ Khởi tạo firmware __(chương trình chính)__ mà ta muốn chạy

## 2.2 Ứng dụng 
<p align = "center">
<img src = "https://github.com/user-attachments/assets/15fecc8a-1d5e-47b3-9b70-b32f5e59166b" width = "600" height = "300">

+ Bootloader cò thể được kết hợp với FOTA __(firmware - Over The Air)__ để cập nhật chương trình từ xa (Wifi), giúp tối ưu hóa về thời gian, công sức và chi phí so với cách truyền thống.

## 2.3 Cách Bootloader xử lý firmware 

<p align = "center">
<img src = "https://github.com/user-attachments/assets/e74506e3-057f-4f6b-919c-3e3fe670173a" width = "600" height = "300">

Ta sẽ có 1 ví dụ để mô tả cách mà bootloader hoạt động như trên. Ta sẽ tiến hành cập nhật firmware là 1 chương trình blinkled cho stm32 với các bước 
+ Tải file hex lên mạng 
+ Esp32 lấy file này thông qua kết nối Wifi
+ Esp32 gửi file hex thông qua uart cho stm32
+ bootloader nhận file hex và tiến hành chạy 

### a) Các thành phần trong FLASH
__Tại 0x08000000__

+ Địa chỉ bắt đầu của FLASH
+ Chứa chương trình bootloader 

__Tại 0x08008000__

+ Địa chỉ của firmware (chương trình chính)
+ Đây là nơi mà bootloader cần lái MCU đến để nó thực thi __(vì bản thân MCU không thể chạy 1 chương trình lưu ở vùng nhớ khác)__ 

__Tại 0x0801FFF__

+ Đây là địa chỉ kết thúc của FLASH

### b) Nhiệm vụ của bootloader trong ví dụ trên
+ Nhận file hex thông qua uart 
+ Lưu firmware vào flash tại địa chỉ mà nó sẽ thực thi sau này và thực hiện 1 số công việc sau đây để MCU có thể nhảy đến đó và chạy firmware

__+ Thiết lập MSP cũng như vector table phù hợp với firmware__
__+ Nhảy đến RESET HANDLER của firmware đó__  
## 2.2 Quá trình hoạt động 
### a) Khi không sử dụng bootloader 
<p align = "center">
<img src = "https://github.com/user-attachments/assets/7f04e9e1-c28b-460c-b57f-f73052eef9d9" width = "650" height = "350">

+ Sau khi cấp nguồn cho MCU
+ Tiến hành đọc chân boot0 và boot1 để xác định vùng nhớ sẽ bát đầu đọc dữ liệu
+ Thanh ghi PC sẽ lưu địa chỉ bát đầu của vùng nhớ đó để bát đầu quá trình đọc lệnh
+ Lấy dữ liệu ở ô nhớ đầu tiên để khởi tạo MSP
+ PC nhảy đến ô nhớ tiếp theo là reset hanlder 
+ Tại reset handler sẽ thực hiện khởi tạo các dữ liệu cần thiết để chạy firmware như tải data từ flash xuống RAM, cuối cùng là nhảy đến hàm main() để thực thi 

### a) Khi có sử dụng bootloader 

<p align = "center">
<img src = "https://github.com/user-attachments/assets/93427643-7f28-4630-9a68-bdfc8038e091" width = "500" height = "350">

+ Sau Khi cấp nguồn và khởi động MCU 
+ Vi xử lý sẽ nhảy đến reset hanlder của chương trình boot 
+ tiếp tục nhảy đến hàm main() của boot 
+ PC tiến hành đọc lấy ra dữ liệu của ô nhớ đầu tiên mà lưu firmware(application program)
+ Gọi hàm bootloader để gán thanh ghi __SCB__ theo địa chỉ của firmware 
+ PC nhảy đến reset handler của firmware để khởi tạo hệ thống 
+ Lúc này firmware đã chính thức được chạy, nên dù nhấn reset chương trình boot sẽ không còn được gọi nữa mà vẫn chạy trong application __(do lúc này MCU đạ nhận diện reset handler được lưu ở địa chỉ mới)__ 
## 2.3 Lập trình bootloader

Đầu tiên ta sẽ viết hàm boot để lái MCU sang địa chỉ của firmware mà ta muốn nó thực thi sau khi được cấp nguồn 

<p align = "center">
<img src = "https://github.com/user-attachments/assets/db40f2bc-0701-42af-870b-e2d5d1eee2ca" width = "650" height = "250">

__Định nghĩa địa chỉ bát đầu của firmware__

+ ADDR_STR_BLINK cần được khai báo ở địa chỉ cách xa vùng nhớ lưu chương trình hệ thống bao gồm bootloader trong khoảng 0x08000000 - 0x08007FFF. Do đó ta chọn giá trị là 0x08008000
+ Vectoe table của firmware cũng được lưu tại địa chỉ trên

__Bước 1: Reset RCC về mặc định__

+ Đảm bảo các xung nhịp của ngoại vi ổn định trước khi chuyển tiếp sang firmware 

__Bước 2: Vô hiệu hóa các line ngắt__

+ Để đảm bảo an toàn trong việc tránh xung đột liên quan đến ngắt khi bootloader lái MCU sang chương trình applicaion (firmware), 

__Bước 3: Cập nhật Stack Pointer__

+ lấy địa chỉ đầu tiên của vector table chính là SP của chương trình application (firmware) và gán cho thanh ghi SP của MCU 
+ Điều này cần thiết vì stack của bootloader và chương trình application là khác nhau

__Bước 4: Cập nhật thanh ghi VTOR__ 

+ Để trỏ tới vector table của chương trình application tại ADDR_STR_BLINK
+ Mục đích là để đảm bảo MCU gọi đúng những hàm ISR khi có ngắt xảy ra từ vector table của của chương trình application chứ không pah3i bootloader 

__Bước 5: Lấy địa chỉ reset handler của chương trình application__

+ Mục đích là để khởi tạo các dữ liệu cần thiết như các biến và vùng nhớ, và cũng là nơi bắt đầu chạy chương trình

__Bước 6: dùng con trỏ hàm để lưu địa chỉ của reset handler__

+ Mục đích là để sử dụng con trỏ này để nhảy tới chương trình application

__Bước 7: Thực thi chương trình application__

+ Truy cập vào chương trình application bằng cách gọi ra con trỏ hàm 













