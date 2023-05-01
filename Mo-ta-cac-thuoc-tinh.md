# Class MainWindow

1. MainWindow::MainWindow(QWidget *parent): hàm khởi tạo của lớp. Nó khởi tạo giao diện người dùng và tạo một phiên bản của MyServer, một lớp xử lý giao tiếp TCP/IP. Nó cũng tạo một bộ đếm thời gian để đồng bộ hóa tập tin mỗi 30 giây.

2. MainWindow::~MainWindow(): hàm hủy của lớp. Nó lưu thư mục đã chọn và xóa đối tượng giao diện người dùng, bộ đếm thời gian, kết nối và máy chủ.

3. MainWindow::on_pushButton_selectFile_clicked(): một phương thức slot được gọi khi người dùng bấm nút "Chọn tệp". Nó mở hộp thoại chọn tệp để chọn một tệp văn bản và hiển thị đường dẫn tệp đã chọn trong một ô văn bản.

4. MainWindow::on_pushButton_syncFile_clicked(): một phương thức slot được gọi khi người dùng bấm nút "Đồng bộ hóa tệp". Nó kiểm tra xem một khách hàng có kết nối với máy chủ không và nếu có, gửi tên tệp và dữ liệu đã chọn cho client. Phương thức cũng đếm số lần dữ liệu được gửi.

5. MainWindow::onTimerTimeout(): một phương thức slot được gọi khi bộ đếm thời gian hết giờ. Nó gọi phương thức on_pushButton_syncFile_clicked() để đồng bộ hóa tệp.

6. MainWindow::get_directory(): một phương thức đọc thư mục đã chọn từ một tệp JSON và thiết lập biến thư mục.

7. MainWindow::save_directory(): một phương thức lưu thư mục đã chọn vào một tệp JSON nếu nó đã thay đổi kể từ lần cuối cùng nó được lưu.

# Class MyServer

1. Hàm xử lý một kết nối đến từ một máy khách mới (incomingConnection): Hàm này được gọi mỗi khi có một máy khách mới kết nối đến với máy chủ. Nó tạo ra một đối tượng QTcpSocket để xử lý kết nối mới và thêm nó vào một QMap để giữ các kết nối hiện tại.

2. Hàm xử lý dữ liệu được gửi đến từ máy khách (onReadyRead): Hàm này được gọi mỗi khi một máy khách gửi dữ liệu đến máy chủ. Nó đọc dữ liệu được gửi đến và hiển thị nó trên bảng điều khiển.

3. Hàm xử lý sự kiện máy khách ngắt kết nối (onDisconnected): Hàm này được gọi mỗi khi một máy khách ngắt kết nối với máy chủ. Nó loại bỏ kết nối không còn khả dụng từ QMap.

4. Hàm gửi dữ liệu đến tất cả các máy khách đang kết nối (sendResponse): Hàm này được sử dụng để gửi dữ liệu đến tất cả các máy khách đang kết nối với máy chủ.

5. Hàm kiểm tra xem có máy khách nào đang kết nối với máy chủ hay không (isClientConnected): Hàm này được sử dụng để kiểm tra xem có bất kỳ máy khách nào đang kết nối với máy chủ hay không. Nó trả về true nếu có ít nhất một kết nối đang được duy trì và false nếu không có kết nối nào.
