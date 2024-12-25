# 1. Tổng Quan về CAN
## a) Khái niệm CAN
<p align = "center">
<img src = "https://github.com/user-attachments/assets/b8dcbec0-f70b-4304-89e0-63ae13a78de7" width = "750" height = "250">

Đây là 1 giao thức truyền thông được sử dụng rộng tãi trong ngành công nghiệp ô tô với các đặc trưng cơ bản như:
+ giao thức nối tiếp và song công toàn phần (full-duplex) cho phép gửi và nhận đồng thời
+ Giao tiếp theo kiểu truyển thông quảng bá (broadcast type) cho phép các node trên can bus đều có thể lắng nghe lẫn nhau
+ Tốc độ truyền cao và xa hơn so với spi,i2c,uart
+ Phần cứng đơn giản cho phép kết nối với nhiều thiết bị (ECU) trong 1 CAN BUS
+ Cơ chế xử lý nhiễu tốt đối với 1 hệ thống phức tạp và di động như ô tô (tồn tại nhiễu cực kỳ lớn)
## b) Khái niệm ECU (Electronic Control Unit)

<p align = "center">
<img src = "https://github.com/user-attachments/assets/17ec8f1a-7128-49e7-93e7-1d585dbbdcd4" width = "600" height = "300">

+ Đây là những bộ điều khiển điện tử được sử dụng để thực hiện 1 chức năng nhất định nào đó trong ô tô ví dụ như ECU bật túi khí, điều khiển động cơ, hệ thóng làm mát, giải trí. 
+ Các ECUs sẽ gửi tín hiệu để điều khiển các hệ thống phần cứng

<p align = "center">
<img src = "https://github.com/user-attachments/assets/8f3280d0-6162-4ceb-ba03-7bbff1a11263" width = "500" height = "200">

+ Mạng CAN cho phép các ECUs có thể trao đổi dữ liệu lẫn nhau để kích hoạt và xuất tín hiệu điều khiển 1 tác vụ nào đó ví dụ như ECU bật túi khí cần nhận thông tin từ các ECU liên quan khác để quyết định thời điểm bung túi khí chính xác.

## 1.1 Đặc điểm nổi bật 
Ngoài những đặc trưng cơ bản CAN còn tích hợp các khả năng nâng cao cho phép nó kết nối và trao đổi dữ liệu của nhiều thiết bị với nhau như:
+ Cho phép nhiều Node trên cùng 1 can bus -> lắng nghe lẫn nhau
+ Các Node sử dụng ID để phân biệt và phân quyền để truyền/nhận 
+ Cho phép truyền lại dữ liệu bị lỗi
+ Tự động ngắt kết nối những node nghi ngờ bị lõi
+ Có thể thêm Node bất kỳ kể cả bus đang hoạt động
+ Sử dụng tín hiệu vi sai (differential signal) thay cho tín hiệu số (digital signal), giúp tối ưu và ổn định quá trình truyền/nhận trong môi trường nhiễu lớn như hệ thống ô tô

__Các lợi ích của CAN__

<p align = "center">
<img src = "https://github.com/user-attachments/assets/dd940e4c-8f6a-4b81-ab29-41713372eb6d" width = "500" height = "250">

## 1.2 Hoạt động của các Nodes trên CAN bus
<p align = "center">
<img src = "https://github.com/user-attachments/assets/6d937b32-ab58-4e9d-9596-de019f708913" width = "500" height = "250">

+ CAN BUS có thể được coi là __(broadcast type)__ khi 1 node có thể gửi message đến tất cả các Node còn lại trên bus
+ Các node trên bus đều có quyền ngang nhau và lắng nghe lẫn nhau
+ Có thể thêm 1 node mới vào bus bất kể lúc nào

## 1.3 Tổng quan về CAN node
+ 1 node trên CAN bus có thể được cấu tạo từ những thành phần như sau 
<p align = "center">
<img src = "https://github.com/user-attachments/assets/f62f7aac-82da-4c51-a43e-3a2755c6bfab" width = "500" height = "250">

__CAN Controller:__ thuộc về MCU, cho phép xuất các tín hiệu số qua các chân CAN_TX và CAN_RX

__CAN Transceiver:__ mạch chuyển đổi tín hiệu (digital) từ MCU sang vi sai (differential) để có thể hoạt động trên CAN bus


### a) Tín hiệu vi sai (differential signal)

Đây là tín hiệu được dùng để giao tiếp giữa các node trên 1 can bus với các đặc điểm:
+ Cho phép truyền/nhận data với khoảng cách lớn 
+ Tích hợp khả năng khử nhiễu bằng cách xác định giá trị hiệu điện thế giữa 2 đầu dây CAN H và CAN L
+ Tín hiệu logic 0 (dominant) và 1 (recessive) được dùng để truyèn đi các bit của message gửi bởi các Node

<p align = "center">
<img src = "https://github.com/user-attachments/assets/45912bd1-40ff-43a2-80d5-2035c4720a16" width = "500" height = "250">

__Dominant state:__ Được xác định bằng hiệu điện thế __(giá trị tuyệt đối)__ chêch lệch giữa 2 dây CAN H và CAN L __(applied voltage CAN H > CAN L)__ 

__+ Với:__ V diff ≥ 1.5V (theo tiêu chuẩn ISO 11898-2)

__Recessive state:__ Giống như trên nhưng giá trị hiệu điện thế sẽ là gần hoặc bằng 0 __(applied voltage CAN H >= CAN L)__ 

__+ Với:__ −0.5V ≤V diff ≤0.5V (theo tiêu chuẩn ISO 11898-2)

### b) Ảnh hưởng và phương pháp khử nhiễu trên CAN BUS

__Ảnh hưỡng của sóng phản xạ__

Hiện tượng này diễn ra do tín hiệu phản xạ lại trên đường truyền dữ liệu ở trên mạng CAN dẫn đến gây lỗi và sai lệch dữ liệu lý do là vì CAN BUS yêu cầu trở kháng cấu tạo nên đường dây cáp, và các thiết bị trên mạng CAN phải đồng nhất __(nếu không 1 phần tín hiệu sẽ phản xạ lại ở điểm trỏ kháng không đồng nhất)__ , do đó ta sẽ có 1 số phương pháp sau để giải quyết được vấn đề đó

__+ Mác điện trở 120 Ohm ở cuối 2 đầu bus:__ giá trị điện trở này trùng với giá trị của trở kháng (Impedance Z) đường bus CAN quyết định dựa trên cấu tạo của nó bao gồm __(tiết diện,độ dài,vật liệu của dây)__ do đó trung hòa và triệt tiêu ảnh hưởng của sóng phản xạ ở 2 đầu dây

__+ Xoắn 2 dây CAN H và CAN L với nhau:__ mục đích là để cân bằng mức nhiễu giữa 2 đầu dây, và nhờ vào tín hiệu vi sai để triệt tiêu chúng

__Ví dụ:__ khi 1 CAN transceiver được cấp điện áp 5V

+ khi không có nhiễu:
V_HIGH = 3.5V , V_LOW = 1,5V 

=> V differential = 3.5 - 1.5 = 2V
+ Khi có nhiễu: (giả sử 2 dây đều chịu 1 mức nhiễu với điện áp 1v)
V_HIGH = 3.5V + 1V = 4.5V

V_LOW = 1.5V + 1V = 2.5V

=> V differential = 4.5 - 2.5 = 2V

__Qua đó ta thấy được cách mà tín hiệu vi sai đã triệt tiêu giá tín hiệu nhiễu ở 2 đầu dây__ 

# 2. Khung truyền của CAN

<p align = "center">
<img src = "https://github.com/user-attachments/assets/cd926715-c5ab-40b0-9296-b54db1cc49ce" width = "700" height = "250">

__+ SOF:__ 1 node khi muốn gửi message sẽ gửi bit 0 (dominant state) để các node khác sẵn sàng lắng nghe

__+ ID:__ Đinh danh của Node, đươc dùng để xác định mức ưu tiên về thứ tự gửi message

__+ RTR:__ Được dùng để xác yêu cầu gửi/nhận của node 

__+ Control:__ Được dùng đẻ xác định loại CAN sủ dụng (với bit 0 là standard can 11-bit) và kích thước của message (4-bit)

__+ Data:__ 64-bit data của message gửi đi

__+ CRC:__ kiểm tra lỗi trong quá trình truyền

__+ ACK:__ Nếu nhận thành công sẽ phản hồi 1 ACK về cho bên truyền 

__+ EOF:__ Quy định kết thúc quá trình truyền/nhận

## 2.1 Message priority và Arbitration process

### a) Khái niệm về Message priority
+ Các message có giá trị binary càng tháp thì mức ưu tiên càng cao
+ Message có chứa ID với giá trị binary hoàn toàn bằng 0 có mức ưu tiên cao nhất
+ Nếu 2 Node cùng gửi message đồng thời, node gửi ID bit là 0 (dominant) sẽ ghi đè và ưu tiên truyền khi đối với node gửi ID là 1 (recessive)

### b) Khái niệm về Arbitration
+ Xác định mức ưu tiên của message khi có nhiều node cùng truyền trên bus
+ Node nào thắng trong quá trình tranh chấp (Arbitration) được quyền truyền message trước, trong khi Node thua sẽ đợi đến lượt để tiếp tục truyền

### c) Arbitration process

<p align = "center">
<img src = "https://github.com/user-attachments/assets/b48a70ec-6920-4b23-8105-b467eed72cf5" width = "650" height = "250">

Khi các node truyền đi message, chúng sẽ tham gia vào quá trình tranh chấp (arbitration) để xác định node có ID với mức ưu tiên cao nhất cho phép truyền đi trước

+ Các node sẽ so sánh từng bit trên Trường định danh của chúng (ID field)
+ Khi các node truyền đi cùng 1 bit giống nhau thì sẽ tiếp tục so sánh bit tiếp theo 
+ Bất kể node nào truyền đi bit 0 thì sẽ dừng quá trình tranh chấp, và chờ đến lượt sau để tiếp tục tranh chấp
+ khi bit cuối của ID field được so sánh, thì node nào có bit bằng 0 (dominant) sẽ thắng tranh chấp và được cho phép ưu tiền truyền message đi  
+ Những node thua trong quá trình tranh chấp hiện tại sẽ đợi đến lượt tranh chấp tiếp theo __(lúc này message vẫn được bảo toàn mà không bị mất)__

## 2.2 Đặc điểm của CAN FRAME

<p align = "center">
<img src = "https://github.com/user-attachments/assets/96b1f1eb-bbbf-4c93-a456-33bd57a19bfc" width = "650" height = "250">

+ Data và Remote frame có thể được cấu hành thủ công phần mềm
+ Error và Overload frame được xử lý tự động bởi phần cứng

### a) DATA FRAME 

<p align = "center">
<img src = "https://github.com/user-attachments/assets/28149610-ddb4-40bf-bbb7-b3b7b45c6703" width = "650" height = "250">

+ Đây là frame được dùng để truyền đi data với các thành phần

__ARBITRATION FIELD__ (TRƯỜNG TRANH CHẤP)
<p align = "center">
<img src = "https://github.com/user-attachments/assets/f6e806ce-2ff3-40c1-af58-354b5ffb3a79" width = "650" height = "250">

+ 1 bit SOF, để bắt đầu quá trình truyền
+ 11 bit ID để xác định quyền ưu tiên của các node
+ 1 bit RTR luôn bằng 0 (quy định data truyền đi)

__Sự khác biệt giữa Standard và Extended CAN__

<p align = "center">
<img src = "https://github.com/user-attachments/assets/cfb640eb-be5d-40e0-ba76-74fd6f95115c" width = "550" height = "150">

+ Standard CAN: hỗ trợ 11-bit ID
+ Extended CAN: Hỗ trợ 11-bit ID và 18-bit mở rộng (có thể tương thích vói standard CAN)

<p align = "center">
<img src = "https://github.com/user-attachments/assets/7bf3ecb1-719e-415f-a1e1-0b64b1b8b3b1" width = "650" height = "250">

__RTR bit__ (Remote Transmission Request)

<p align = "center">
<img src = "https://github.com/user-attachments/assets/1e028425-d5de-4d2d-a54e-603a12b56708" width = "650" height = "250">

+ RTR bit = 0(dominant) -> message truyền đi (data frame)
+ RTR bit = 1(recessive) -> yêu cầu message (remote frame) và không chứa data field

__DLC, DATA and CRC Fields__

<p align = "center">
<img src = "https://github.com/user-attachments/assets/0705a074-fbdb-4959-a160-63373eb404ab" width = "650" height = "250">

+ Gồm 4-bit xác định kích thước của message
+ Gồm 8-bit xác định message truyền đi
+ Gồm 15-bit checksum để kiểm tra lỗi (Node nhận sẽ tính toán trường CRC field của chính nó và kiểm có trùng với CRC field của node truyền không -> nếu trùng message nhận được thành công) -> gửi lại 1 bit ACK 

__EOF AND ITF__
<p align = "center">
<img src = "https://github.com/user-attachments/assets/48e74943-fd33-4aaf-8068-16b23281021b" widht = "650" height = "250">

+ Gồm 7-bit EOF ở trạng thái recessive để kết thúc
+ Gồm 3-bit ITE recessive để bus vào trạng thái IDLE trước khi gửi SOF

### b) REMOTE FRAME

<p align = "center">
<img src = "https://github.com/user-attachments/assets/5c6e99a9-000d-4439-90b9-aef0b8489a65" widht = "650" height = "250">

+ Giống vói dataframe ngoại trừ không có data field, và bit RTR sẽ mặc định là 1 (recessive) để chờ nhận message từ node khác

### c) ERROR FRAME

<p align = "center">
<img src = "https://github.com/user-attachments/assets/3be16771-1a63-4834-ae36-0711b07abb3b" widht = "650" height = "250">

+ Khung này sẽ được kiểm tra bởi CRC field bên node nhận, và nếu có lỗi xảy ra nó sẽ bỏ qua frame này

### d) OVERLOAD FRAME

<p align = "center">
<img src = "https://github.com/user-attachments/assets/ffc60e76-98d2-45dc-9a77-ad6f7de7477b" widht = "650" height = "250">

+ Khi bên node nhận đang bận thì nó sẽ gửi frame này cho bên truyền để tạm dừng việc truyền dữ liệu 

# 3. Quá trình cấu hình và thiết lập
## 3.1 Thiết lập baudrate 
+ Khi các node giao tiếp với nhau trên mạng CAN thì sẽ cần phải được cấu hình đồng bộ về mặt thời gian để có thể truyền thông dữ liệu 1 cách ổn định. Lý do là vì Hệ thống ô tô rất phức tạp và nhiễu cực kỳ lớn. Chính vì vậy cần phải điều chỉnh thời gian chính xác để dữ liệu có thể được truyền tải 1 cách ổn định mà không bị lỗi. Do 1 số 
### a) Sampling Point 
+ Thời điểm mà tín hiệu trên CAN bus được đọc để xác định 1 bit là dominant (0) hay recessive (1)
+ Được tính toán dựa trên phần trăm thời gian 1 bit. Thời điểm lý tưởng là 75 - 90% thời gian 1 bit
### b) Time Quatum và Bit Timing

__Time quantum__ 

+ Đơn vị thời gian nhỏ nhất mà 1 bit được chia ra
+ Xác định dựa trên Clock của CAN và prescaler
+ Một bit thưỡng sẽ chia thành nhiều segment, với mỗi segment có 1 số lượng time quantum nhất định

__Bit timing__

+ Thời gian truyền đi của 1 bit
+ Chính là tổng số time quantum 

### c) Time segmnet

<p align = "center">
<img src = "https://github.com/user-attachments/assets/91b8f96b-f46f-4702-ae25-bc1712cd7020" widht = "650" height = "250">

Đây là thành phần để đồng bộ và điều chỉnh thời gian giao tiếp giữa các node CAN. Được chia làm 4 phần

__Syn Segment__

+ Độ dài cố định là 1 tq
+ giúp các node biết được sự bắt đầu của 1 bit tại 1 thời điểm để tiến hành xử lý
+ Nó phát hiện bằng cách theo dõi sự chuyển đổi rising/falling edge của tín hiệu CAN để hiệu chỉnh thời gian của chính nó -> đãm bảo đồng bộ với các node trên bus

__Propagation segment__

+ Bù đắp khoảng thời gian trễ của tín hiệu khi truyền trên can bus từ sender đến receiver 
+ Đoạn này sẽ được điều chỉnh dựa trên __chiều dài bus__ và __tốc độ truyền__
+ Được cấu hình hợp lý để đồng bộ được với __độ trễ lang truyền__ và __thời gian trễ xử lý__ trên phần cứng CAN

__Phase segment 1:__ 

+ Khoàng thời gian được đièu chỉnh trước khi lấy mẫu, đảm bảo clock được đồng bộ với thời gian thực tế trên bus do ảnh hưởng từ độ dài bus hoặc nhiễu 
__Phase segment 2:__ 

+ Khoảng thời gian được điều chỉnh sau lấy mẫu, đảm bảo các node đồng bộ clock với nhau (do sự sai lệch nhỏ giữa các node)

=> Nếu cạnh tín hiệu đến sớm : Đoạn này có thể kéo dài

=> Nếu cạnh tín hiệu đến trễ : Đoạn này có thể rút ngắn 

__Tỗng thời gian của 1 bit và baudrate__

+ bit_timing = Sync + Propagation + phase1 + phase2

+ Baudrate = 1 / bit_timing (bps)

## 3.2 Thiết lập bộ lọc
Ta biết rằng CAN là một giao thức truyền thông quản bá __(broadcast protocol)__ ,nghĩa là 1 node trên bus có thể lắng nghe thống điệp gửi từ các node khác. Tuy nhiên node nhận ở mỗi thời điểm chỉ xử lý 1 thông điệp duy nhất và ta cần có 1 cơ chế để giúp node xử lý đúng thông điệp mà nó muốn và loại bỏ đi những phần không quan trọng. Chính vì vậy bộ lọc CAN sẽ giải quyét vấn đề đó
### a) Tại sao nó cần thiết

<p align = "center">
<img src = "https://github.com/user-attachments/assets/09a67832-e9a8-4dad-87c9-da3812fc4bcf" widht = "500" height = "200">

Bộ lọc là 1 thành phần cần thiết vì nó cho phép node nhận chỉ xử lý những thông điệp mà nó muốn và bỏ qua những cái không quan trọng giúp 
+ Giảm tải quá trình xử lý cho MCU
+ Giảm băng thông để bus hoạt động hiệu quả hơn
### b) Các thành phần 

__Mask__ 

__+ Đặc điểm:__ là 1 chuỗi các bit với 

=> các bit 1: được dùng để kiểm tra ở 1 phần nào đó của ID. 

=> các bit 0: ở các vị trí tương ứng trên ID mà không cần quan tâm

__+ Chức năng:__ Được dùng để xác định phạm vi ID mà node quan tâm (thông điệp cho phép xử lý)

=> Những bit của ID mà ta muốn kiểm tra thì mask sẽ đặt giá trị tại các vị trí tương ứng lên 1 và tiến hành áp dụng lên ID bằng cách thực hiện phép logic AND

__Filter__

__+ Đặc điểm:__ chuỗi các bit mà ID phải khớp với sau khi đã áp dụng bọ lọc 

__+ Chức năng:__ So sánh với kết quả của ID sau khi đã áp dụng mask, nếu trùng khớp thì cho phép xử lý, nếu không thì loại bỏ













