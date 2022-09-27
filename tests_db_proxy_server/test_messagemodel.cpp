//
//  test_messagemodel.cpp
//  test_messagemodel
//
//  Created by blueBling on 22-04-25.
//  Copyright (c) 2022年blueBling. All rights reserved.
//

#include "MessageContent.h"
#include "MessageCounter.h"
#include "IM.Message.pb.h"


#include<iostream>


using std::cout;
using std::endl;

string strAudioEnc;

int test_getmsg() {


//	message IMGetMsgListReq {
//		//cmd id:		0x0309
//		required uint32 user_id = 1;
//		required IM.BaseDefine.SessionType session_type = 2;
//		required uint32 session_id = 3;
//		required uint32 msg_id_begin = 4;
//		required uint32 msg_cnt = 5;
//		optional bytes attach_data = 20;
//    }


	IM::Message::IMGetMsgListReq msg;
	msg.set_user_id(1);
	msg.set_session_type(IM::BaseDefine::SESSION_TYPE_SINGLE);
	msg.set_session_id(17);
	msg.set_msg_id_begin(2);
	msg.set_msg_cnt(10);
	msg.set_attach_data("");


	CImPdu pdu;
	
	pdu.SetPBMsg(&msg); // 必须先设置这个，这里会为m_buf分配空间

	pdu.SetVersion(1);
	pdu.SetFlag(0);
	pdu.SetServiceId(IM::BaseDefine::SID_MSG);
	pdu.SetCommandId(IM::BaseDefine::CID_MSG_LIST_REQUEST);
	pdu.SetSeqNum(65282);
	pdu.SetReversed(0);

	DB_PROXY::getMessage(&pdu, 1);

	return 0;
}


int main(){

	test_getmsg();


	//这里mysql和redis连接池未释放存在内存泄漏问题，解决方法参考test_dbpool
	
	return 0;
}
