#include <time.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "protocol.h"
#define MSGBUFSIZE 1024
#define RC_KB 1024
#define RC_MB (1024*RC_KB)
#define FRAME_BIT 0x01
#define FRAME_SIZ (10*RC_MB)
#define FRAME_BUF_PATH "/dev/rc_mem"
#define BUFSIZE 512
#define PORT_NUM 9487
#define MINI(x,y) ((x<y)? x:y)
static char *frame[2];
void error_handling (char *message)
{
	fputs (message, stderr);
	fputc ('\n', stderr);
	exit (1);
}


void HandleFileUpload (int clnt_sock);
void FileDownloadProcess (int clnt_sock);
ssize_t getMsg(int clnt_sock, char* msg){
    
    ssize_t tmpSize = recv (clnt_sock, msg, MSGBUFSIZE, MSG_WAITALL);
    
    int i=0;
    while(1){
        if(msg[i] == '!'){
            msg[i] = '\0';
            break;
        }
        i++;
    }
    
    if (tmpSize == -1)
        error_handling ("recv() error");
    else if (tmpSize == 0)
        error_handling ("peer connection closed2");
    else if (tmpSize != MSGBUFSIZE)
        error_handling ("recv unexpected number of bytes");
    
    return tmpSize;
}

int main (void)
{
	/* 명령 파라미터의 개수(argc)가 2인지 검사 */
	//if (argc != 2)
	//{
	//	printf ("Usage : %s <port>\n", argv[0]);
	//	exit (1);    
	//}

	in_port_t servPort = PORT_NUM;


	frame[0] = NULL;
	frame[1] = NULL;
	frame[0] = (char *)malloc(sizeof(FRAME_SIZ));
	frame[1] = (char *)malloc(sizeof(FRAME_SIZ));
	if(NULL == frame[0] || NULL == frame[1]){
		printf("Sorry out of memory\n");
		return 0;
	}
	/* socket() : TCP 서버 소켓 생성 */
	int serv_sock = socket (PF_INET, SOCK_STREAM, 0);
	if (serv_sock == -1)
		error_handling ("socket() error");

	/* bind() : 서버가 사용할 포트/주소를 서버소켓과 묶음 */
	struct sockaddr_in serv_addr;
	memset (&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl (INADDR_ANY);
	serv_addr.sin_port = htons (servPort);
	if (bind (serv_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1)
		error_handling ("bind() error");

	/* listen() : 서버 소켓을 리슨 소켓으로 변경 */
	if (listen (serv_sock, 5) == -1)
		error_handling ("bind() error");

	/* accept() : 연결 후 생성된 클라이언트소켓을 리턴 */
	struct sockaddr_in clnt_addr;
	socklen_t clnt_addr_len = sizeof (clnt_addr);
	int clnt_sock = accept (serv_sock, (struct sockaddr*)&clnt_addr, &clnt_addr_len);
	if (clnt_sock == -1)
		error_handling ("accept() error");

	/* 접속한 클라이언트의 정보를 화면에 출력 */
	printf ("Connected from %s:%d\n", inet_ntoa(clnt_addr.sin_addr), ntohs(clnt_addr.sin_port) );

	while(1){
		/* msgType 수신 : 필드 크기는 1Bytes(uint8_t)로 고정 */
		uint8_t msgType;
		//ssize_t numBytesRcvd = recv (clnt_sock, &msgType, sizeof (msgType), MSG_WAITALL);
		//if (numBytesRcvd == -1)
		//	error_handling ("recv() error");
		//else if (numBytesRcvd == 0)
		//	error_handling ("peer connection closed");
		//else if (numBytesRcvd != sizeof (msgType))
		//	error_handling ("recv unexpected number of bytes");

		/* msgType 값에 따라 분기 처리 */
		//if (msgType == FileUpReq)
		if(1){
			HandleFileUpload (clnt_sock); 

		return 0;}
		/* 파일 업로드 처리 */
		else if(msgType == FileDownReq)
			FileDownloadProcess (clnt_sock); 

		/*파일 다운로드 처리 */
		else if (msgType == ExitReq)


			break;

		else
			printf("error_not define msg");
	}

	/* close() : 소켓을 닫음 */
	close (clnt_sock);
	close (serv_sock);
	return 0;
}


void HandleFileUpload (int clnt_sock) 
{ 

	/* 파일 이름을 수신 : 필드 크기는 256Bytes로 고정 */ 
//	char fileName[FILENAMESIZE]; 
	//sssize_t numBytesRcvd=0; //= recv (clnt_sock, fileName, FILENAMESIZE, MSG_WAITALL); 
	//if (numBytesRcvd == -1) 
	//	error_handling ("recv() error"); 
	//else if (numBytesRcvd == 0) 
	//	error_handling ("peer connection closed"); 
	//else if (numBytesRcvd != FILENAMESIZE) 
	//	error_handling ("recv unexpected number of bytes"); 
	//strcat (fileName, "_up"); 
	//printf ("fileName = %s\n", fileName); 

	/* 파일 크기를 수신 : 필드 크기는 uint32_t로 고정 */ 
	static int frame_num = 0;
	uint32_t netFileSize; 
	uint32_t fileSize; 
//	char netFileSize[4];
	//numBytesRcvd = recv (clnt_sock, &netFileSize, 4, MSG_WAITALL);
    char rcvFileName[MSGBUFSIZE] = {'\0',};
    ssize_t numBytesRcvd = getMsg(clnt_sock, rcvFileName);
	if (numBytesRcvd == -1)
		error_handling ("recv() error"); 
	else if (numBytesRcvd == 0) 
		error_handling ("peer connection closed"); 
//	else if (numBytesRcvd != 4) 
//		error_handling ("recv unexpected number of bytes"); 
	//fileSize = ntohl (atoi(netFileSize)); 
    	//printf("file size : %s", rcvFileName);
	fileSize = atoi(rcvFileName);
	///* 파일 내용을 수신 : 필드 크기는 위 uint32_t 내용 */ 
	int fd = open("./test.png", O_RDWR|O_CREAT|O_TRUNC, 0666); 
	if (fd <= 0) 
		error_handling ("fopen() error"); 
	printf("Comming...\n");
	uint32_t rcvdFileSize = 0; 
	while (rcvdFileSize < fileSize)
	{ 
		numBytesRcvd = recv (clnt_sock, frame[frame_num]+rcvdFileSize, 512,0); 
		if (numBytesRcvd == -1) {
			printf("oops\n");
			//usleep(50000);
			continue;
		}
		else if (numBytesRcvd == 0) 
			error_handling ("peer connection closed"); 

		//if (ferror (fp)) 
		//	error_handling ("fwrite() error"); 

		rcvdFileSize += numBytesRcvd; 
		usleep(250);
	}
	printf("File Size %d\n", rcvdFileSize);

	write(fd, frame[frame_num], rcvdFileSize);
	//sync();
	close(fd);

	/* 파일 수신 완료 */ 
	//close(fd);
//	for(int i=0; i<rcvdFileSize; i++){
//		printf("%c", frame[frame_num][i]);
//	}
//
//	printf("\n");
	return;
	frame_num ^= FRAME_BIT;

	//exit(-1);
	/* 파일수신 성공메시지(msgType:FileAck)를 클라이언트에게 전송 */ 
	uint8_t msgType = FileAck; 
	ssize_t numBytesSent = send (clnt_sock, &msgType, sizeof (msgType), 0); 
	if (numBytesSent == -1) 
		error_handling ("send() error"); 
	else if (numBytesSent != sizeof (msgType)) 
		error_handling ("sent unexpected number of bytes"); 
} 

void FileDownloadProcess (int clnt_sock) 
{ 

	/* 파일 이름을 수신 : 필드 크기는 256Bytes로 고정 */ 
	char fileName[FILENAMESIZE]; 
	ssize_t numBytesRcvd = recv (clnt_sock, fileName, FILENAMESIZE, MSG_WAITALL); 
	if (numBytesRcvd == -1) 
		error_handling ("recv() error"); 
	else if (numBytesRcvd == 0) 
		error_handling ("peer connection closed"); 
	else if (numBytesRcvd != FILENAMESIZE) 
		error_handling ("recv unexpected number of bytes"); 

	/* 파일크기를 클라에 전송 : 필드 크기는 uint32_t로 고정 */ 
	struct stat sb; 
	if (stat (fileName, &sb) < 0) 
		error_handling ("stat() error"); 
	uint32_t fileSize = sb.st_size; 
	uint32_t netFileSize = htonl (fileSize); 
	ssize_t numBytesSent = send (clnt_sock, &netFileSize, sizeof (netFileSize), 0); 
	if (numBytesSent == -1) 
		error_handling ("send() error"); 
	else if (numBytesSent != sizeof (netFileSize)) 
		error_handling ("sent unexpected number of bytes"); 

	/* 파일내용을 클라에 전송 : 필드 크기는 위 fileSize 변수값 */ 
	FILE *fp = fopen (fileName, "r"); 
	if (fp == NULL) 
		error_handling ("fopen() error"); 

	while (!feof(fp)) 
	{ 
		char fileBuf[BUFSIZE]; 
		size_t numBytesRead = fread (fileBuf, sizeof (char), BUFSIZE, fp); 
		if (ferror (fp)) 
			error_handling ("fread() error"); 

		numBytesSent = send (clnt_sock, fileBuf, numBytesRead, 0); 
		if (numBytesSent == -1) 
			error_handling ("send() error"); 
		else if (numBytesSent != numBytesRead) 
			error_handling ("sent unexpected number of bytes"); 
	} 
	fclose (fp); 

	/* 파일전송완료 */ 

	/* 클라이언트로부터의 ACK메시지 수신 후 화면에 성공여부 출력 */

	uint8_t msgType; 
	numBytesRcvd = recv (clnt_sock, &msgType, sizeof (msgType), MSG_WAITALL); 
	if (numBytesRcvd == -1) 
		error_handling ("recv() error");
	else if (numBytesRcvd == 0)
		error_handling ("peer connection closed");
	else if (numBytesRcvd != sizeof (msgType))
		error_handling ("recv unexpected number of bytes");

	if (msgType == FileAck)
		printf ("%s 파일 전송 성공!!(%u Bytes)\n", fileName, fileSize);
	else
		printf ("%s 파일 전송 실패!!\n", fileName);
}
