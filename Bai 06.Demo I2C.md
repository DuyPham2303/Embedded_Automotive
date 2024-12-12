# Lập trình I2C sử dụng API Hardware và Software 
# 1. Sử dụng API hardware 

+ Ta sẽ sử dụng các API có sẵn trong thư viện hệ thống, để viết các hàm truyền và nhận I2C 

## 1.1 Khai báo các hàm,thư viện và định nghĩa macro 

<p align =  "center">
<img src = "https://github.com/user-attachments/assets/c1192010-39b1-48c6-a460-8aad923f95c0" width = "700" height = "250">

## 1.2 Hàm truyền 7bit địa chỉ và bit read/write

<p align = "center">
<img src = "https://github.com/user-attachments/assets/a165b8aa-f756-4c63-80e5-ad8b33043ed9" width = "700" height = "100">

## 1.3 Hàm truyền I2C 

<p align =  "center">
<img src = "https://github.com/user-attachments/assets/9066c4f9-7230-48e4-b4ce-675e17132922 " width = 700" height = "250">

+ master sẽ gửi start condition bằng cách kéo đường SDA xuống low trong 1 chu kỳ clock
+ Tiếp theo ta đợi đến khi bus không bị chiếm dụng (trạng thái rãnh)
+ Tiến hành gửi địa chỉ của slave cùng với bit write 
+ Đợi slave phản hồi 
+ Bắt đầu gừi và kiểm tra từng byte có truyền đi thành công hay chưa
+ Gửi stop condition sau khi hoàn tất việc truyền data
 
## 1.4 Hàm nhận I2C

<p align =  "center">
<img src = "https://github.com/user-attachments/assets/9e1c4f8c-fbbe-46ff-86a3-f579328499b4 " width = 700" height = "250">

+ master gửi start condition 
+ Đợi đến khi bus rảnh hoàn toàn
+ Gửi địa chỉ của slave và bit read
+ Đợi slave phản hồi
+ Bắt đầu quá trình đọc data và phản hồi lại ACK cho slave
+ Hủy việc gửi ACK khi gặp byte cuối (len = 1), Đồng thời gửi stop condition
+ Tiến hành kich hoạt lại việc gửi ACK để chuẩn bị cho chu kỳ đọc tiếp theo
+ Đợi đến khi kết thúc quá trình giao tiếp I2C(cờ BSY được reset)


# 2. Sử dụng API software
+ Ta sẽ viết các hàm mô phỏng việc truyền nhận I2C không sử dụng thư viện hệ thống

## 2.1 Khai báo các hàm, thư viện và định nghĩa các macro 

<p align =  "center">
<img src = "https://github.com/user-attachments/assets/dcd11138-a2ee-4600-a02c-d3a6489d24b2" width = 700" height = "250">

## 2.2 Các hàm tạo start và stop condtion, clock I2C

<p align =  "center">
<img src = "https://github.com/user-attachments/assets/2ea55a44-4549-4bd8-aded-b7e780a18fe5" width = 700" height = "250">

## 2.3 Hàm gửi và nhận 1 byte

<p align =  "center">
<img src = "https://github.com/user-attachments/assets/aa4ed5af-2350-4464-a741-3d8a10ddbfec" width = 750" height = "250">

<p align =  "center">
<img src = "https://github.com/user-attachments/assets/6ff0ddb0-66d7-4a5c-b0f6-9bc0f0e4a74d" width = "750" height = "250">

## 2.4 Hàm gửi địa chỉ slave và bit read/write

<p align =  "center">
<img src = "https://github.com/user-attachments/assets/8fbc994f-a1de-4dd5-815a-ed2b490e0ae8" width = "750" height = "100">

+ Hàm này trả về trạng thái OK/NOT_OK cho biết slave đã xác nhận đúng địa chỉ của nó mà master gửi đến cùng với yêu cầu read/write 

## 2.5 Hàm gừi nhiều byte

<p align =  "center">
<img src = "https://github.com/user-attachments/assets/1a48be06-81dd-48dc-a56f-1c347f46f912" width = "750" height = "250">

+ Mỗi khi hàm gửi 1 byte được gọi nó sẽ đợi đến khi kết quả trả về là OK, cho biết đã truyền thành công rồi mới tiếp tục gửi byte tiếp theo 

## 2.6 Hàm nhận nhiều byte

<p align =  "center">
<img src = "https://github.com/user-attachments/assets/8267e3d1-2a22-4ceb-80f7-a36e02bd42e9" width = "750" height = "250">

+ Master sẽ phản hồi 1 bit ACK cho slave. Khi đọc byte cuối nó sẽ gửi bit NACK 

# 3. Lập trình giao tiếp stm32f103 và module thời gian thực ds3231

<p align =  "center">
<img src = "https://github.com/user-attachments/assets/1f3ab8a4-1a26-4472-8758-585687fce124" width = "400" height = "250">

+ Trong dự án này ta sẽ sử dụng các hàm truyền/nhận i2c để ghi và đọc các giá trị giờ/phút/giây từ module ds3231

## 3.1 Tổng quan về module ds3231

### a) Thanh ghi lưu trữ giá trị thời gian

<p align "center">
<img src ="https://github.com/user-attachments/assets/5379d304-8fd6-4a9c-bb3d-4fb17b802d4b" width = "750" height = "150">

+ Các giá trị lưu trong thanh ghi này sẽ được biểu diễn ở hệ BCD (binary-code-decimal), nghĩa là khi ta ghi 1 giá trị vào module ds3231 phần đơn vị của nó sẽ lưu dưới dạng 4 bit riêng và phần chục sẽ lưu ở 4 bit còn lại 
+ mỗi giá trị giờ/phút/giây sẽ lưu trữ trong các WordAddress tương ứng là 02h,01h,00h
+ bit thứ 7 của WordAddress luôn là 0 để giới hạn giá trị tối đa của các giá trị giờ/phút/giây
+ bit thứ 6 của wordAddress lưu trữ giá trị giờ có giá trị mặc định là 0, cho phép lưu trữ giá từ 0 - 23 giờ , bit thứ 5 lc1 này sẽ được tự động được set để lưu trữ giá trị thời gian từ 20 - 23 giờ
### b) Quá trình ghi và đọc 

<p align "center">
<img src ="https://github.com/user-attachments/assets/23f93e71-96ba-4bd8-8e5c-8c8d8808d195" width = "750" height = "300">

__Quá trình ghi data vào ds3231__

+ stm32 gửi địa chỉ của ds3231 kèm với bit Write
+ ds3231 phản hồi lại stm32 bằng 1 bit ACK
+ stm32 gửi địa chỉ của WordAddress kèm với data mà nó muốn ghi vào ds3231
+ mỗi 1 byte ghi vào WordAddress thành công sẽ được ds3231 phản hồi lại 1 bit ACK

__Quá trình đọc data từ ds3231__

+ stm32 gửi địa chỉ của ds3231 kèm với bit read
+ ds3231 phản hồi lại stm32 bằng 1 bit ACK
+ stm32 gừi 1 con trỏ đến WordAddress mà nó muốn đọc ra từ ds3231
+ stm32 tiếp tực gửi cho ds3231 địa chỉ của biến mà nó muốn đọc data về 
+ Sau khi quá trình đọc hoàn tất stm32 sẽ phản hồi NACK kết thức quá trình giao tiếp với ds3231

## 3.2 Triển khai các hàm thao tác với module ds3231

### a) Khai báo các hàm,thư viên, macro cần thiết

<p align "center">
<img src ="https://github.com/user-attachments/assets/67c0347a-d262-4bbb-ba71-db4c9cad128e" width = "750" height = "300">

+ Ta khai báo 1 struct để lưu trữ các mảng dùng để ghi và đọc từ ds32321

### b) Cấu hình các thông số I2C và GPIO 

<p align "center">
<img src ="https://github.com/user-attachments/assets/3dec920a-2d35-449d-bc5b-d39c31ea25a3" width = "750" height = "300">

### c) Hàm ghi vào ds3231 

<p align "center">
<img src ="https://github.com/user-attachments/assets/3bdf8a4a-5133-4e2a-abb8-0ecab6d31c7c" width = "750" height = "200">

### d) Hàm đọc về từ ds3231 

<p align "center">
<img src ="https://github.com/user-attachments/assets/ea1129ba-7ee2-4db8-95b2-cd1fba3a3c00" width = "750" height = "200">

### e) Hàm chuyển đổi giá trị khi lưu vào WordAddress và ghi đọc ra 

<p align "center">
<img src ="https://github.com/user-attachments/assets/c5bed928-4911-4f5c-bf28-1de0e8a98201" width = "750" height = "200">

### f) Hàm cài đặt và đọc ra giá trị thời gian 

<p align "center">
<img src ="https://github.com/user-attachments/assets/5f535b0e-9628-4470-9735-b05f36dbb307" width = "750" height = "200">

## 3.3 Bố cục thư mục dự án

<p align "center">
<img src ="https://github.com/user-attachments/assets/11e4b72b-c9cd-4c89-b0d9-c92c6ae41af1" width = "850" height = "350">

__Folder HW_Drivers và SW_Drivers__ sẽ chứa các API hardware và software được sử dụng ở trong file rtc.c1192010

__folder Module__ chứa các hàm static để thao tác với module ds3231 và 2 hàm set_time và get_time, cũng như hàm delay timer được gọi và sữ dụng trên file rtc.c

## 3.4 Chương trình chính

<p align "center">
<img src ="https://github.com/user-attachments/assets/0da2f87c-7779-47cc-82f9-369c41c3566d" width = "750" height = "200">
























