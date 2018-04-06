#include <stdio.h>
#include <mpi.h>
#include <openssl/md5.h>
#include <time.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>

using namespace std;

char character[26] = {'a', 'b', 'c', 'd', 'e', 'f'
    , 'g', 'h', 'i', 'j', 'k', 'l'
    , 'm', 'n', 'o', 'p', 'q', 'r'
    , 's', 't', 'u', 'v', 'w', 'x', 'y', 'z'};

// chuyển một số thành một chuỗi ký tự chữ cái
void convertPassword(int password, int len, char convertPassword[]);

// Mã hóa Md5 cho một chuỗi ký tự chữ cái
void encodePasswordMD5(const char *string, char *mdString);

// Tìm kiếm mật khẩu
void findPassword(int begin, int end, char *mdString, int len, int rank);

// Đánh giá quá trình dò mật khẩu
void appreciate(char *mdString, int numberProcess, int passwordLength, int rank);

int main(int argc, char* argv[])
{
  int size, rank;
  char hostname[50];
  MPI_Init(&argc, &argv);
  char mdString[33];
  int passwordLength;
  if(argc != 3) return -1;
  sscanf(argv[1], "%s", mdString);
  sscanf(argv[2], "%d", &passwordLength);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	gethostname(hostname, 50);
	printf("My rank is %d , %s \n", rank, hostname);
	appreciate(mdString, size, passwordLength, rank);
	MPI_Finalize();
	return 0;
}

void convertPassword(int password, int len, char convertPassword[]){
	int i = len - 1;
	int j;
	int point;
	for (j =0 ; j < len ; j++){
		convertPassword[j] = 'a';
	}
	while(1){
		if (password>0){
			point = password% 26;
			convertPassword[i] = character[point];
			i--;
			password= password/ 26;
		}else{
			break;
		}
	}
}

void encodePasswordMD5(const char *string, char *mdString){
    unsigned char digest[16];
    MD5_CTX ctx;
    MD5_Init(&ctx);
    MD5_Update(&ctx, string, strlen(string));
    MD5_Final(digest, &ctx);
    for (int i = 0; i < 16; i++)
        sprintf(&mdString[i*2], "%02x", (unsigned int)digest[i]);
}

void findPassword(int begin, int end, char *mdString, int len, int rank){
	char pas[len];
	char temp_mdString[33];
	while(1){
		convertPassword(begin, len, pas);
		encodePasswordMD5(pas, temp_mdString);
		if(!strcmp(temp_mdString, mdString)){
			printf("passwordis %s %d\n", pas, rank);
			break;
		}else if(begin == end){
			break;
		}else{
			begin ++;
		}
	}
	return;
}

void appreciate(char *mdString, int numberProcess, int passwordLength, int rank){
  clock_t t1,t2;
  t1 = clock();
  int begin, end;
  int total_password = pow(26, passwordLength);
  int part = total_password / numberProcess;
  begin = part * (rank);
  if (rank != numberProcess - 1){
  	end = begin + part -1;
  }else{
  	end = total_password -1;
  }
	printf("%d %d begin end \n", begin, end);
	findPassword(begin, end, mdString, passwordLength, rank);
	t2=clock();
  float diff ((float)t2-(float)t1);
  float seconds = diff / CLOCKS_PER_SEC;
  printf("Time required = %f second in rank %d \n", seconds, rank);
}
