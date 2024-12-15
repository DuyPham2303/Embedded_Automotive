# 1. Tổng quan về LIN (Local Interconnect Network)

<p align = "center">
<img src = "https://github.com/user-attachments/assets/9c700bc2-5a95-44fd-aa6e-f11bd1e4962c" width = "550" , height = "300">

## 1.1 Khái niệm 

+ LIN là 1 giao thức truyền thông được sử dụng trong xe hoi nhằm đi sự phức tạp cũng như chi phí trong việc truyền thông giữa 1 số bộ phận mà không yêu cầu quá cao về tốc độ truyền, chính xác, hay độ an toàn
+ LIN sẽ được sử dụng kết hợp với CAN trong việc truyền tải dữ liệu và điều khiển các bộ phận của xe hơi.

## 1.2 Đặc điểm
__+ TỐC ĐỘ:__ từ 1 - 20 kbps => hoạt động tốt nhất ở 19.2 kbps
__+ CHI PHÍ VÀ ĐỘ PHỨC TẠP:__ thấp hơn CAN, phù hợp điều khiển các bộ phận không yêu cầu tốc độ và độ an toàn cao
__+ GIAO TIẾP UART:__ dựa trên mô hình master - slave (1 master -> tối đa 15 slave)  => không hỗ trợ giao tiếp real-time
__+ GATEWAY:__ dùng cho giao tiếp với CAN
__+ HARDWARE:__ sử dụng module uart và transceiver => chuyển đổi tín hiệu uart sang lin
## 1.3 So sánh với các giao thức khác 

<p align = "center">
<img src = "https://github.com/user-attachments/assets/e5418f6d-d766-43cd-97bc-06bb112a8a98" width = "550" , height = "300">

## 1.4 Một số ứng dụng thực tế

<p align = "center">
<img src = "https://github.com/user-attachments/assets/baf60222-1e08-4a21-ba3a-4c6da1fd6501" width = "550" , height = "300">

__Ví dụ:__ Điều khiển cửa sổ 

### a) Các thành phần hệ thống
+ CAN bus: kết nối ECU chính (BCM, ECU động cơ)

__BCM__: quản lý tín hiệu điều khiển chính chuyển tới __LIN__

+ LIN bus: kết nối công tắc cửa sổ, motor điều khiển cửa sổ
### b) Quy trình hoạt động

__Bước 1:__ nhấn công tắc => tín hiệu truyền qua LIN bus đến LIN master -> gateway chuyển tín hiệu đến CAN

__Bước 2:__ BCM trên CAN bus xử lý tín hiệu -> đưa ra quyết định về cách thức điều khiển

__Bước 3:__ CAN gửi tín hiệu đến LIN thông qua gateway 

__Bước 4:__ LIN master nhận lệnh -> gửi tín hiệu điều khiển tới motor để điều khiển cửa sổ

## 1.5 Sơ lược về giao tiếp và khung truyền
### a) Giao tiếp giữa master - slave

<p align = "center">
<img src = "https://github.com/user-attachments/assets/ceb41898-fb8e-4196-9cd0-c7194380c3ac" width = "650" , height = "300">

__Đặc điểm__
+ Master sẽ quét yêu cầu tới tất cả các slave để thăm dò thông tin
+ Slave sẽ chỉ phản hồi nếu có yêu cầu từ master
+ Dữ liệu master nhận từ slave sẽ được gửi lên CAN bus để truyền đến các LIN khác
+ Sử dụng UART để truyền thông: __1 start bit + 1 byte data + 1/2 stop bit__

__Sơ lược Cách thức giao tiếp__

+ master gửi khung __header__ cho các slave
+ Nếu master muốn đọc từ slave -> slave sẽ gửi khung __response__ cho master
+ Nếu master muốn gửi đến slave -> master gửi kèm khung __response__ cho slave
+ Slave so sánh ID của mình có khớp với master -> có thì xử lý , không thì bỏ qua

### b) Cấu trúc khung truyền 

<p align = "center">
<img src = "https://github.com/user-attachments/assets/fdf96c78-3d6a-48c9-bcd1-c2f70f7507d3" width = "650" , height = "250">

__+ BREAK field (14 bits):__ xác định thời điểm bắt đầu truyền -> slave bắt đầu dữ liệu từ master
__+ SYNC field (1 byte):__ Đồng bộ tốc độ giữa master và slave -> mặc định là __0x55__ (giá trị xen kẽ các bit 0,1 -> thích hợp kiểm tra lỗi)

__+ ID field (6 bits id + 2 bits parity):__ xác định loại dữ liệu và slave cần phản hồi -> tối đa 64 ID

__+ DATA field (2 - 8 bytes):__ dữ liệu của thông điệp

__+ CHECKSUM field (1 byte):__ Kiểm tra lỗi trong quá trình truyền

# 2. Hoạt động của LIN
## 2.1 Nhiệm vụ của tửng khung truyền 

<p align = "center">
<img src = "https://github.com/user-attachments/assets/b0041939-0cb4-47c0-8291-d76845fed405" width = "500" , height = "400">

## 2.2 Quá trình truyền dữ liệu

__Master gửi header__ 

+ Break field được gửi đầu tiên để báo hiệu cho slave (tín hiệu bắt đầu khung)
+ Sync field được gừi tiếp theo để đồng bộ tốc độ truyền __(0x55)__
+ ID field được truyền cuối cùng để xác định loại dữ liệu và slave cần phản hồi

__Repsonse được gửi từ master hoặc slave__

=> Response được master/slave gửi dựa trên yêu cầu của ID trong header

+ data field chứa 2 - 8 byte (lệnh điều khiển hoặc dữ liệu cảm biến)
+ Checksum field chứa 1 byte để kiểm tra lỗi

## 2.3 Quá trình kiểm tra lỗi
# 3. Lập trình giao tiếp LIN và MCU
## 3.1 Lớp phần mềm giao tiếp trên stm32f103
## 3.2 Demo giao tiếp master/slave LIN