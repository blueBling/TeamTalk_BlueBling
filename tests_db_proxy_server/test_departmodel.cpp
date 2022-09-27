//
//  test_depart.cpp
//  test_depart
//
//  Created by blueBling on 22-03-01.
//  Copyright (c) 2022年blueBling. All rights reserved.
//

#include "DepartAction.h"
#include "ImPduBase.h"

#include<iostream>


using std::cout;
using std::endl;

string strAudioEnc;

//message IMDepartmentReq{
//	//cmd id:		0x0210
//	required uint32 user_id = 1;
//	required uint32 latest_update_time = 2;
//	optional bytes attach_data = 20;
//}
//
//message IMDepartmentRsp{
//	//cmd id:		0x0211
//	required uint32 user_id = 1;
//	required uint32 latest_update_time = 2;
//	repeated IM.BaseDefine.DepartInfo dept_list = 3;
//	optional bytes attach_data = 20;
//}


int test_depart(){
	uint32_t conn_uuid = 11;

	CImPdu pdu; //构造一个部门信息请求pdu

	//
	//(gdb) p pPdu->m_pdu_header
	//$1 = {length = 35, version = 1, flag = 0, service_id = 2, command_id = 528, seq_num = 0, reversed = 0}

	unsigned char header[16] = {
		0x00,	0x00,	0x00,	0x23,	0x00,	0x01,	0x00,	0x00,
		0x00,	0x02,	0x02,	0x10,	0x00,	0x00,	0x00,	0x00
	};

	pdu.ReadPduHeader(header, 16); //构造pdu头

	
	//(gdb) p pPdu->m_buf 
	//$4 = {m_buffer = 0x92a010 "", m_alloc_size = 43, m_write_offset = 35}

	unsigned char body[35] = {
		0x00,	0x00,	0x00,	0x23,	0x00,	0x01,	0x00,	0x00,
		0x00,	0x02,	0x02,	0x10,	0x00,	0x00,	0x00,	0x00,
		0x08,	0x11,	0x10,	0x00,	0xa2,	0x01,	0x0c,	0x00,
		0x00,	0x00,	0x01,	0x00,	0x00,	0x00,	0x0e,	0x00,
		0x00,	0x00,	0x00
	};

	pdu.Write(body, 35); //构造pdu体
	
	DB_PROXY::getChgedDepart(&pdu, conn_uuid);

	//这里mysql连接池未释放存在内存泄漏问题，解决方法参考test_dbpool
	
	return 0;
}

int main(){
	test_depart();
	return 0;
}
