//
//  test_conn_filesvr.cpp
//  test_conn_filesvr
//
//  Created by blueBling on 22-07-25.
//  Copyright (c) 2022年blueBling. All rights reserved.
//

#include "netlib.h"
#include "EncDec.h"
#include "FileServConn.h"

#include <iostream>

using std::cout;
using std::endl;

CAes *pAes;


int test_conn_filesvr(){

	pAes = new CAes("12345678901234567890123456789012");

	serv_info_t server_list;
	server_list.server_ip = "192.168.49.128";
	server_list.server_port = 8601;

	init_file_serv_conn(&server_list, 1);
	
	netlib_eventloop();

	delete pAes;
	
	return 0;
}


int main(){
	test_conn_filesvr();
	return 0;
}
