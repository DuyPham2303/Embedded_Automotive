# Lập trình SPI sử dụng hardware và Software 
Trong bài này chúng ta sẽ lập trình để 2 MCU stm32f1 và stm32f4 giao tiếp với nhau. Cụ thể hơn là stm32f4 sẽ gửi 1 chuỗi bất kỳ, stm32f1 sẽ nhận và gửi ngược lại chuỗi đó.
## 1. Sử dụng hardware API 
### 1.1 Cấu hình thư mục (giống nhau ở master và slave)

<p align =  "center">
<img src = "https://github.com/user-attachments/assets/45a6225d-8e34-4561-a386-e5788a9482f6" width = "700" height = "350">

### 1.2 Định nghĩa file.h (giống nhau ở master và slave)

+ Trong file.h ,ta định nghĩa macro các chân và clock RCC cho bộ SPI sẽ sử dụng cùng với khai báo các hàm 
+ Ta sẽ định nghĩa 1 struct để cấu hình và chọn bộ SPI muốn sử dụng.

<p align =  "center">
<img src = "https://github.com/user-attachments/assets/ddea1939-cc5d-4978-a865-14711bfb9c31" width = "700" height = "350">


### 1.3 Hàm truyền và nhận SPI 

<p align =  "center">
<img src = "https://github.com/user-attachments/assets/71e7c5db-97b6-45c1-affd-704a5c739686" width = "700" height = "250">

+ Đầu tiên ta sẽ kiểm tra buffer truyền có chứa data không, nếu không có cờ TXE sẽ được set, ta tiến hành gửi từng byte
+ Tiếp theo kiểm tra buffer nhận có chứa data không, nếu có data, cờ RXNE sẽ được set, ta tiến hành đọc data 
+ Sau khi truyền và nhận xong data, ta đợi đến khi cờ BSY được reset, nghĩa là quá trình giao tiếp SPI đã kết thúc.

### 1.4 Hàm nhận và truyền SPI

<p align =  "center">
<img src = "https://github.com/user-attachments/assets/a93a506e-ddf9-454b-9096-12452e578269" width = "700" height = "250">


### 1.5 Cấu hình slave(stm32f1) và master(stm32f4) 

<p align =  "center">
<img src = "https://github.com/user-attachments/assets/6212fbe7-d160-45b5-bcbc-908a2e4b3970" width = "750" height = "400">

+ Cả 2 MCU được cấu hình là NSS soft, vì vậy chân CS có thể bỏ qua, ta sẽ điều khiển tín hiệu NSS bằng phần mềm 
+ Slave sẽ không cấu hình tốc độ vì nó phụ thuộc vào clock cấp bên master

### 1.6 Chương trình chính 

<p align =  "center">
<img src = "https://github.com/user-attachments/assets/26a7c37f-4660-4299-b88a-6ab0e4782d8f" width = "750" height = "400">

+ Bên master ta sẽ tiến hành gửi 1 chuỗi kèm biến cnt tăng lên cứ sau 1s. Nó sẽ không điều khiển tín hiệu NSS mà sẽ liên tục gửi cho slave
+ Bên slave ta sẽ có hàm SPI_NSSInternalSoftwareConfig() để mô phỏng tín hiệu NSS. Nhờ vậy slave có thể quyết định thời điểm nó muốn nhận từ master

### 1.7 Kết quả 

<p align =  "center">
<img src = "https://github.com/user-attachments/assets/94eb5850-3311-45bf-b63f-0565e08b0c86" width = "750" height = "400">

## 1.2 Sử dụng software API 

### 1.1 Định nghĩa file.h 
+ Trong file.h ta cũng khai báo các hàm và định nghĩa macro cho các thành phần của SPI
+ Ta không định nghĩa struct để cấu hình SPI như phần hardware, mà sẽ lập trình các hàm mô phỏng SPI thay cho việc sử dụng thư viện

<p align =  "center">
<img src = "https://github.com/user-attachments/assets/de978b80-21da-4da3-a56d-d24e011c75af" width = "750" height = "400">

### 1.2 Hàm truyền 1 byte 

<p align =  "center">
<img src = "https://github.com/user-attachments/assets/da7b239c-c029-4a2b-b354-4e89f2c80c3f" width = "750" height = "400">

### 1.3 Hàm nhận 1 byte

<p align =  "center">
<img src = "https://github.com/user-attachments/assets/a65ce59a-8316-4986-b14f-8ad31fac5700" width = "750" height = "400">

### 1.4 Hàm truyền và nhận nhiều byte

<p align =  "center">
<img src = "https://github.com/user-attachments/assets/a9d9c1f7-f188-4c9d-9b91-fbf862063783" width = "750" height = "400">







