//
//  test_sessionmodel.cpp
//  test_sessionmodel
//
//  Created by blueBling on 22-03-28.
//  Copyright (c) 2022年blueBling. All rights reserved.
//

#include "RecentSession.h"


#include<iostream>


using std::cout;
using std::endl;

string strAudioEnc;

//模拟win client，获取userid为1的会话信息
int test_getrecentsession() {

	//(gdb) p pPdu->m_pdu_header 
	//$1 = {length = 39, version = 1, flag = 0, service_id = 2, command_id = 513, seq_num = 0, reversed = 0}
	//(gdb) x/16xb &pPdu->m_pdu_header 
	//0x94cd58:	0x27	0x00	0x00	0x00	0x01	0x00	0x00	0x00
	//0x94cd60:	0x02	0x00	0x01	0x02	0x00	0x00	0x00	0x00
	//(gdb) p pPdu->m_buf 
	//$2 = {m_buffer = 0x9504a0 "", m_alloc_size = 48, m_write_offset = 39}
	//(gdb) x/39xb pPdu->m_buf.m_buffer 
	//0x9504a0:	0x00	0x00	0x00	0x27	0x00	0x01	0x00	0x00
	//0x9504a8:	0x00	0x02	0x02	0x01	0x00	0x00	0x00	0x00
	//0x9504b0:	0x08	0x01	0x10	0xab	0xb4	0xa1	0x91	0x06
	//0x9504b8:	0xa2	0x01	0x0c	0x00	0x00	0x00	0x01	0x00
	//0x9504c0:	0x00	0x00	0x0e	0x00	0x00	0x00	0x00

	CImPdu pdu; //构造一个获取会话信息请求pdu

	
	unsigned char header[16] = {
		0x00,	0x00,	0x00,	0x27,	0x00,	0x01,	0x00,	0x00,
		0x00,	0x02,	0x02,	0x01,	0x00,	0x00,	0x00,	0x00
	};

	pdu.ReadPduHeader(header, 16); //构造pdu头


	unsigned char body[39] = {
		0x00,	0x00,	0x00,	0x27,	0x00,	0x01,	0x00,	0x00,
		0x00,	0x02,	0x02,	0x01,	0x00,	0x00,	0x00,	0x00,
		0x08,	0x01,	0x10,	0xab,	0xb4,	0xa1,	0x91,	0x06,
		0xa2,	0x01,	0x0c,	0x00,	0x00,	0x00,	0x01,	0x00,
		0x00,	0x00,	0x0e,	0x00,	0x00,	0x00,	0x00
	};

	pdu.Write(body, 39); //构造pdu体

	DB_PROXY::getRecentSession(&pdu, 1);

	return 0;
}


int main(){

	test_getrecentsession();

	//这里mysql连接池未释放存在内存泄漏问题，解决方法参考test_dbpool

	return 0;
}