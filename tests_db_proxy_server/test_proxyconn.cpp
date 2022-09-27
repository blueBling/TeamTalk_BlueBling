//
//  test_proxyconn.cpp
//  test_proxyconn
//
//  Created by blueBling on 22-04-29.
//  Copyright (c) 2022年blueBling. All rights reserved.
//

#include "ProxyConn.h"

#include <iostream>


using std::cout;
using std::endl;

string strAudioEnc;

// this callback will be replaced by imconn_callback() in OnConnect()
void proxy_serv_callback(void* callback_data, uint8_t msg, uint32_t handle, void* pParam)
{
	if (msg == NETLIB_MSG_CONNECT)
	{
		CProxyConn* pConn = new CProxyConn();
		pConn->OnConnect(handle);
	}
	else
	{
		log("!!!error msg: %d", msg);
	}
}


int test_proxyconn() {

	init_proxy_conn(3);

	netlib_listen("192.168.49.128", 7777, proxy_serv_callback, NULL);

	netlib_eventloop(10);

	return 0;
}


int main(){

	test_proxyconn();

	//这里mysql和redis连接池未释放存在内存泄漏问题，解决方法参考test_dbpool

	return 0;
}
