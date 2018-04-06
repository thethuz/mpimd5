Sử dụng MPI để thám mã md5
Biên dịch:
mpic++ mpimd5.cpp -o mpimd5
Chạy:
mpirun -np [nprocess] mpimd5 [hashstring] [passwordLength]

