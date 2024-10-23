# Giao thức truyền thông 
## 1. SPI 
![image](https://github.com/user-attachments/assets/2bb0d6df-899c-40e8-b7dd-6fe876522e76)

### 1.1 Giới thiệu về SPI
__a. Khái niệm__

Giao thức truyền thông được phát triển bởi motorola năm 1980. Còn được gọi là giao thức truyền song công toàn phần __full duplex__ với các đặc điểm sau: 
+ giao tiếp giữa 1 master vs 1 hoặc nhiều slave
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
__+ Nhược điểm__ 

+ Sử dụng nhiều dây hơn so với các giao thức khác
+ Chỉ hiệu quả với đường truyền ngắn
+ Không có các cơ chế kiểm tra lỗi hay xác nhận quá trình truyền/nhận thành công hay chưa 
### 1.2 Kết nối phần cứng
__a) Sơ đổ chân__

![image](https://github.com/user-attachments/assets/1284a1fb-33f3-4674-a329-3f26ec4426fa) 

__MOSI__ : master truyền và slave nhận 

__MISO__ : slave truyền và master nhận


__SCLK__ : kiểm soát bởi master, để tạo ra xung clock đồng bộ việc   truyền nhận data giữa 2 bên

__SS__ : chân chọn chip, master dùng chân này để xác định slave muốn giao tiếp, bằng cách kéo từ high xuống low trong 1 clk trên chân __SS__ của slave

__b) Mắc song song__ 

![image](https://github.com/user-attachments/assets/b35998fa-03f4-45e2-8c6a-1f2ae072165e)

__Ưu điểm__: xác định được slave nào bị lỗi nhanh chóng nếu quá trình truyền/nhận bị lỗi do  các slave được nối độc lập với nhau

__Nhược điểm__: tốn nhiều dây ss khi số lượng slave tăng

__c) Mắc nối tiếp__ 

![image](https://github.com/user-attachments/assets/3ee82686-49b8-42e0-b1c2-dc569b2e2e77)

__Ưu điểm__:phần cứng đon giản do Chỉ sử dụng 1 dây SS để master quản lý các slave

__Nhược điểm__: khó khăn trong việc xác định slave nào bị lỗi khi data truyền/nhận sai, do các slave được mắc nối tiếp với nhau

### 1.3 Quá trình truyền dữ liệu

![image](https://github.com/user-attachments/assets/36c91fa7-2898-40f3-a0b8-06c170e538bd)

__Bước 1__ : Master kéo chân __SS__ từ high xuống low để báo hiệu cho slave biết sẵn sàng nhận data

__Bước 2__ : master gửi 1 bit trên chân __MOSI__  kèm với 1 clock trên chân __SCK__ 

__Bước 3__: Slave kiểm tra đường __SCK__ có ở mức high hay không và tín hành đọc 1 bit qua đường __MOSI__

__Bước 4__: Slave sau đó gửi lại 1 bit cho master qua đường __MISO__

__Bước 5__: Master lặp lại quá trình truyền 1 bit cho đến khi đủ 8 bit, sau đó sẽ nhả chân __SS__  lên mức high để kết thúc quá trình truyền

### 1.4 Tham số cấu hình cho SPI
Để SPI hoạt động thì ta cần cấu hình 4 loại thông số sau:

__a) chế độ truyền__

+ SONG CÔNG (__Full-duplex__): Master sẽ vừa truyền và nhận data trong 1 thòi điểm
![image](https://github.com/user-attachments/assets/092df91e-97d4-4497-9765-1160ead3c8e8)

+ BÁN SONG CÔNG (__Half-duplex__): Master chỉ có thể truyền hoặc nhận data trong 1 thời điểm

![image](https://github.com/user-attachments/assets/2d508edb-ca9d-4b4a-aa15-beb7ffb853b3)

+ ĐƠN CÔNG (__Simplex__): Master chỉ có thể truyền mà không nhận data từ slave

![image](https://github.com/user-attachments/assets/10d04e7e-4476-4c7c-b567-0c6df1451777)

__b) CPOL và CPHA__

__SCK Phase__ : trạng thái idle (không có dữ liệu) trên chân __SCK__ 
+ __CPOL = 0__ : ở mức __low__
+ __CPOL = 1__ : ở mức __high__
![image](https://github.com/user-attachments/assets/30fbff03-5271-40de-8124-e9e55cba50dc)

__SCK Polarity__: trạng thái của clk mà slave chọn để đọc data

+ __CPHA = 0__ : ở cạnh đầu tiên của clk

![image](https://github.com/user-attachments/assets/018f963f-2e1a-4951-837e-df80bd7f4e89)
+ __CPHA = 1__ : ở cạnh tiếp theo của clk

![image](https://github.com/user-attachments/assets/4e116f1e-b2d0-4b60-b2e9-68c97cb6bd9d)

__c) Tốc độ__

phụ thuộc vào nguồn clock đầu vào như HSE/HSI/PLL và qua bộ chia tầng của bus mà nó nối đến

![image](https://github.com/user-attachments/assets/7c83f489-7e59-496e-932d-de1da97855d2)

__d) Slave mmanagement__

