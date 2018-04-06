## Sử dụng MPI để thám mã md5

###### Biên dịch:
```
mpic++ mpimd5.cpp -o mpimd5
```
###### Chạy:
```
mpirun -np [nprocess] mpimd5 [hashstring] [passwordLength].
```
Trong đó nprocess là số tiến trình, hashstring là mật khẩu cần tìm, passwordLength là độ dài mật khẩu đã biết.

