#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include <unistd.h>

const char* sample_xml = "<?xml version=\"1.0\" encoding=\"utf-8\"?> <term_update_info> <userCert>1233</userCert> <userSID>缺省 787987923429349242</userSID> <updateTime>2016-01-20 17:26:43</updateTime> <terminalCltVer>3.2.0</terminalCltVer> <terminalType>2</terminalType> <updateStatus>1</updateStatus> <terminalTypeName>SM-N9002</terminalTypeName> <terminalID>357555051008870</terminalID> <terminalHD>357555051008870</terminalHD> <CERTCARDOU>缺省</CERTCARDOU> <CERTCARDO>缺省</CERTCARDO> <terminalOS>Android 4.3</terminalOS> <res2>res2</res2> <res1>res1</res1> <vpnGateID>1234</vpnGateID> </term_update_info>";



int client_tcp(char *serIP,in_port_t serPort,char *data);
int main()
{
	int port=60090;
	while(1) {
		client_tcp("127.0.0.1",port,(char*)sample_xml);
		sleep(1);
	}
}


int client_tcp(char *serIP,in_port_t serPort,char *data)
{
	//创建socket
	int sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sock < 0) {
		printf("socket Error!");
		exit(0);
	}

	//填充sockaddr_in
	struct sockaddr_in serAddr;
	memset(&serAddr, 0, sizeof(serAddr));
	serAddr.sin_family = AF_INET;
	serAddr.sin_port = htons(serPort);
	int rtn = inet_pton(AF_INET, serIP, &serAddr.sin_addr.s_addr);
	//或者是  serAddr.sin_addr.s_addr=inet_addr(serIP);
	if (rtn <= 0) {
		printf("inet_pton Error!");
		exit(0);
	}

	printf("目标服务器地址：%s: %d\n", inet_ntoa(serAddr.sin_addr), ntohs(serAddr.sin_port));
	printf("     网络层协议：%s\n", serAddr.sin_family == 2 ? "IPv4" : "IPv6");
	printf("     传输层协议：TCP\n");


	//链接服务器
	if (connect(sock, (struct sockaddr *) &serAddr, sizeof(serAddr)) < 0) {
		printf("connect Error!!\n");
		exit(0);
	}
	//show the other side
	printf("connected Server %s : %d\n", inet_ntoa(serAddr.sin_addr), ntohs(serAddr.sin_port));

	//发送数据
	int bufsize = strlen(sample_xml);
	int num = send(sock, sample_xml, bufsize, 0);
	if (num <= 0) {
		printf("Send Error!!\n");
		exit(0);
	}

	//接收数据
	fputs("Received: ", stdout);
	char buffer[100];
	int n = recv(sock, buffer, 100 - 1, 0);
	if (n <= 0) {
		printf("Receive Error!!\n");
		exit(0);
	} else {
		buffer[n] = '\0';
		printf("%s\n", buffer);
	}

	//关闭socket
	close(sock);
	//exit(0);
	return 0;
}
