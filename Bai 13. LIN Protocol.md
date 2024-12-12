+ Áp dụng trong việc điều khiển các thành phần 
không yêu cầu tốc độ cao, và không yêu cầu độ an toàn
+ Ví dụ: điều khiển cửa kính,mở cốp xe,đèn pha,gương
+ giảm thiểu chi phí và độ phức tạp so với CAN
+ Hoạt động theo cơ chế master-slave
+ giao tiếp với CAN thông qua gateway 
KHUNG LIN
+ 2 phần header và reponse 
CÁCH HOẠT ĐỘNG:
+ master quét qua từng slave 
+ master gửi kèm với 1 header -> yêu cầu slave read/write
+ nếu: 
header(read): master gửi kèm với 1 response 
header(write): master nhận response từ slave
ĐẶC ĐIỂM CỦA LIN
+ Tốc độ vừa phải tối đa : 20 kbps
+ Chi phí và độ phức tạp thấp hơn CAN
+ mô hình master-slave -> sử dụng uart 
+ Không hỗ trợ giao tiếp real-time
+ ứng dụng cho các hệ thống không yêu cầu độ an toàn hay tốc độ cao 
+ Kết nối các thành phần chi phí thấp 
+ Sử dụng gateway để giao tiếp với CAN 
KHUNG TRUYỀN CAN