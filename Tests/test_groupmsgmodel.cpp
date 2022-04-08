//
//  test_groupmsgmodel.cpp
//  test_groupmsgmodel
//
//  Created by blueBling on 22-04-07.
//  Copyright (c) 2022年blueBling. All rights reserved.
//

#include "GroupMessageModel.h"

#include<iostream>


using std::cout;
using std::endl;


// 测试用户id为18所有未读消息数（该用户仅在群组id为1的群组里）
// redis unread库 key为1_im_group_msg的hash表保存了群组id为1所有消息数
// redis unread库 key为18_1_im_user_group的hash表保存了用户id为18的用户在群组id为1的所有已读消息数
int test_getunreadmsgcnt() {

	uint32_t nTotalCnt = 0;
	
	list<IM::BaseDefine::UnreadInfo> lsUnreadCount;

	CGroupMessageModel::getInstance()->getUnreadMsgCount(18, nTotalCnt, lsUnreadCount);


	cout << "nTotalCnt:" << nTotalCnt << endl;

//	message UnreadInfo{
//		required uint32 session_id = 1;
//		required SessionType session_type = 2;
//		required uint32 unread_cnt = 3;
//		required uint32 latest_msg_id = 4;
//		required bytes latest_msg_data = 5;
//		required MsgType latest_msg_type = 6;
//		required uint32 latest_msg_from_user_id = 7;		//发送得用户id
//	}

	for(const auto &item : lsUnreadCount) {

		cout << "session_id:" << item.latest_msg_id();
		cout << " session_type:" << item.session_type();
		cout << " unread_cnt:" << item.unread_cnt();
		cout << " latest_msg_id:" << item.latest_msg_id();
		cout << " latest_msg_data:" << item.latest_msg_data();
		cout << " latest_msg_type:" << item.latest_msg_type();
		cout << " latest_msg_from_user_id:" << item.latest_msg_from_user_id();

		cout << endl;
	}
	


	return 0;
}


int main(){

	test_getunreadmsgcnt();


	//这里mysql和redis连接池未释放存在内存泄漏问题，解决方法参考test_dbpool
	
	return 0;
}
