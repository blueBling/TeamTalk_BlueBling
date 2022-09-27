//
//  test_login.cpp
//  test_login
//
//  Created by blueBling on 22-04-26.
//  Copyright (c) 2022年blueBling. All rights reserved.
//


#include "Login.h"
#include "IM.Server.pb.h"


#include <iostream>
#include <string>


using std::cout;
using std::endl;
using std::string;

string strAudioEnc;

//#include <unistd.h>
//
//unsigned int sleep(unsigned int seconds);


int test_login() {

	IM::Server::IMValidateReq msg;

//	message IMValidateReq{
//		//cmd id:	0x0703
//		required string user_name = 1;
//		required string password = 2;
//		optional bytes attach_data = 20;
//	}

	string name("ql1");
	string passwd("e10adc3949ba59abbe56e057f20f883e");
	string passwd_err("e10adc3949ba59abbe56e057f20f883d");


	// 以错误密码间隔1分钟登陆1次
	uint32_t tmNow = time(NULL);
	int cnt = 0;

	while(cnt < 15) {

		msg.set_user_name(name.c_str(), name.size());
		msg.set_password(passwd_err.c_str(), passwd_err.size());
		msg.set_attach_data("");


		CImPdu pdu;
		pdu.SetPBMsg(&msg);
		pdu.SetSeqNum(1);
		pdu.SetServiceId(IM::BaseDefine::SID_OTHER);
		pdu.SetCommandId(IM::BaseDefine::CID_OTHER_VALIDATE_REQ);

		DB_PROXY::doLogin(&pdu, 6);

		cnt++;
		sleep(2);
	}

	return 0;
}


int main(){

	test_login();


	//这里mysql和redis连接池未释放存在内存泄漏问题，解决方法参考test_dbpool
	
	return 0;
}
