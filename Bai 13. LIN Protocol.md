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