//
//  test_netlib.cpp
//  test_netlib
//
//  Created by blueBling on 22-1-25.
//  Copyright (c) 2022年blueBling. All rights reserved.
//

#include "netlib.h"

#include<iostream>
#include<string>

#include <unistd.h> //sleep
#include <string.h>

using std::cout;
using std::endl;
using std::string;

/*
"HTTP/1.1 200 OK\r\nConnection:close\r\nContent-Length:268\r\nContent-Type:text/html;charset=utf-8\r\n\r\n{\n   \"backupIP\" : \"192.168.49.128\",\n   \"code\" : 0,\n   \"discovery\" : \"http://127.0.0.1/api/discovery\",\n   "

*/

void imconn_callback(void* callback_data, uint8_t msg, uint32_t handle, void* pParam)
{
	switch (msg)
	{
	case NETLIB_MSG_CONFIRM:
		cout << "NETLIB_MSG_CONFIRM" << endl;
		break;
	case NETLIB_MSG_READ:
	{
		cout << "NETLIB_MSG_READ" << endl;
		char szBuf[1024] = "";
		int ret = netlib_recv(handle, szBuf, 2048);
		cout << "recv " << szBuf << endl;

		memset(szBuf, '\0', sizeof(szBuf));
		sprintf(szBuf, "%s", "HTTP/1.1 200 OK\r\nConnection:close\r\nContent-Length:268\r\nContent-Type:text/html;charset=utf-8\r\n\r\n{\n   \"backupIP\" : \"192.168.49.128\",\n   \"code\" : 0,\n   \"discovery\" : \"http://127.0.0.1/api/discovery\",\n   \"msfsBackup\" : \"http://192.168.49.128:8700/\",\n   \"msfsPrior\" : \"http://192.168.49.128:8700/\",\n   \"msg\" : \"\",\n   \"port\" : \"8000\",\n   \"priorIP\" : \"192.168.49.128\"\n}\n");
		ret = netlib_send(handle, (void*)szBuf, strlen(szBuf));
		cout << "ret" << ret << endl;	
		netlib_close(handle);
	}
		break;
	case NETLIB_MSG_WRITE:
		cout << "NETLIB_MSG_WRITE" << endl;
		break;
	case NETLIB_MSG_CLOSE:
		cout << "NETLIB_MSG_CLOSE" << endl;
		break;
	default:
		cout << "!!!imconn_callback error msg: " << msg << endl;
		break;
	}
}


void proxy_serv_callback(void* callback_data, uint8_t msg, uint32_t handle, void* pParam)
{
	if (msg == NETLIB_MSG_CONNECT)
	{
		cout << "msg is NETLIB_MSG_CONNECT" << endl;
		string peer_ip;
		uint16_t peer_port;
		netlib_option(handle, NETLIB_OPT_SET_CALLBACK, (void*)imconn_callback);
		netlib_option(handle, NETLIB_OPT_GET_REMOTE_IP, (void*)&peer_ip);
		netlib_option(handle, NETLIB_OPT_GET_REMOTE_PORT, (void*)&peer_port);

		cout << "connect from " << peer_ip.c_str() << ":" << peer_port << endl;
	}
	else //只要连接上，m_callback就被修改成其他了 netlib_option(handle, NETLIB_OPT_SET_CALLBACK, (void*)imconn_callback);
	{
		cout << "!!!error msg:" << msg << endl;
	}
}

int test_netlib(){
	char ip[] = "0.0.0.0";
	uint16_t listen_port = 8080;
	int ret = netlib_listen(ip, listen_port, proxy_serv_callback, NULL);
	if (ret == NETLIB_ERROR)
		return ret;

	netlib_eventloop(10);

	return 0;
}

int main(){
	test_netlib();
	return 0;
}
