//
//  test_httpparser.cpp
//  test_httpparser
//
//  Created by blueBling on 22-2-10.
//  Copyright (c) 2022年blueBling. All rights reserved.
//

#include "HttpParserWrapper.h"

#include <iostream>

using std::cout;
using std::endl;

int test_httpparser(){

	CHttpParserWrapper cHttpParser;
	
	char in_buf[1024] = "GET /msg_server HTTP/1.1\r\nHost:192.168.49.128:8080\r\nAccept: */*\r\nConnection: Keep-Alive\r\n\r\n";
	uint32_t buf_len = 91;
	in_buf[buf_len] = '\0';

	cHttpParser.ParseHttpContent(in_buf, buf_len);

	//http_parser解析会调用CALLBACK_NOTIFY(message_complete)实际调用了SetReadAll
	if (cHttpParser.IsReadAll()) {
		string url =  cHttpParser.GetUrl();
		if (strncmp(url.c_str(), "/msg_server", 11) == 0) {		// 路由判断
            string content = cHttpParser.GetBodyContent();
			cout << "_HandleMsgServRequest" << endl;
		} else {
			cout << "url unknown, url=" <<  url.c_str() << endl;
		}
	}

	return 0;
}


int main(){
	test_httpparser();
	return 0;
}
