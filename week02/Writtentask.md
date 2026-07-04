2. [Tu luan 4.2] Phan bo vi tri cac bien vao vung nho
Ten Bien Phan Vung Bo Nho Giai Thich Chi Tiet Gia Tri &
Vi Tri
BIKE_MODEL .rodata (Read-Only Data) La bien hang so cuc bo hoac
toan cuc co tu khoa const va
da khoi tao gia tri chuoi. Vung
nay nam o ROM/Flash de
tranh bi thay doi trong qua
trinh chay.
total_odometer .bss (Block Started by
Symbol)
La bien toan cuc chua khoi
tao (hoac khoi tao bang 0).
Vung nay nam o RAM va
duoc ma Startup xoa ve 0 luc
khoi dong chuong trinh.
current_speed Stack La bien cuc bo thong thuong
duoc khai bao ben trong ham
main(). Dynamic
allocation/deallocation theo
thoi gian song cua ham.

3. [Tu luan 4.3] Hien tuong say ra khi say ra su co tran Stack

● Tren May tinh (Moi truong OS nhu Linux/Windows): He dieu hanh quan ly bo nho
bang Virtual Memory va co co che bao ve (MMU). Khi Stack Pointer vuot khoi trang bao
ve, OS se ngay lap tuc cham dut tien trinh va dua ra thong bao loi: Segmentation fault
(core dumped) de bao ve tinh toan ven cua cac tien trinh khac.

● Tren Vi dieu khien (Embedded Hardware thuc te - ARM Cortex-M): Khong co OS dung
nghia bao ve. Khi Stack bi tran, con tron ngan xep SP se ghi de vao vung du lieu cua cac
bien toan cuc (.data, .bss) hoac vuot ra ngoai vung dia chi RAM. CPU se phat hien vi
pham phan cung va kich hoat ngoại le nghiem trong, lap tuc nhay vao trinh phuc vu ngat:
HardFault_Handler() de treo he thong hoac Reset chip

