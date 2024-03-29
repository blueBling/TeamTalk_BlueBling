//
//  test_filemodel.cpp
//  test_filemodel
//
//  Created by blueBling on 22-03-17.
//  Copyright (c) 2022年blueBling. All rights reserved.
//

#include "FileAction.h"
#include "FileModel.h"


#include<iostream>


using std::cout;
using std::endl;

string strAudioEnc;

//模拟记录用户1给用户17发送离线文件tmp.txt
int test_addofflinefile() {

	//(gdb) p pPdu->m_pdu_header 
	//$1 = {length = 70, version = 1, flag = 0, service_id = 5, command_id = 1291, 
	//seq_num = 0, reversed = 0}
	//(gdb) x/16xb &pPdu->m_pdu_header 
	//0x964258:	0x46	0x00	0x00	0x00	0x01	0x00	0x00	0x00
	//0x964260:	0x05	0x00	0x0b	0x05	0x00	0x00	0x00	0x00
	//(gdb) p pPdu->m_buf
	//$2 = {m_buffer = 0x964270 "", m_alloc_size = 87, m_write_offset = 70}
	//(gdb) x/70xb  pPdu->m_buf.m_buffer 
	//0x964270:	0x00	0x00	0x00	0x46	0x00	0x01	0x00	0x00
	//0x964278:	0x00	0x05	0x05	0x0b	0x00	0x00	0x00	0x00
	//0x964280:	0x08	0x01	0x10	0x11	0x1a	0x24	0x33	0x35
	//0x964288:	0x32	0x62	0x36	0x30	0x61	0x32	0x2d	0x65
	//0x964290:	0x36	0x31	0x65	0x2d	0x34	0x61	0x31	0x31
	//0x964298:	0x2d	0x38	0x64	0x62	0x62	0x2d	0x33	0x35
	//0x9642a0:	0x61	0x34	0x65	0x35	0x65	0x63	0x31	0x30
	//0x9642a8:	0x65	0x66	0x22	0x07	0x74	0x6d	0x70	0x2e
	//0x9642b0:	0x74	0x78	0x74	0x28	0xe1	0x01

	CImPdu pdu; //构造一个部门信息请求pdu

	
	unsigned char header[16] = {
		0x00,	0x00,	0x00,	0x46,	0x00,	0x01,	0x00,	0x00,
		0x00,	0x05,	0x05,	0x0b,	0x00,	0x00,	0x00,	0x00
	};

	pdu.ReadPduHeader(header, 16); //构造pdu头


	unsigned char body[70] = {
		0x00,	0x00,	0x00,	0x46,	0x00,	0x01,	0x00,	0x00,
		0x00,	0x05,	0x05,	0x0b,	0x00,	0x00,	0x00,	0x00,
		0x08,	0x01,	0x10,	0x11,	0x1a,	0x24,	0x33,	0x35,
		0x32,	0x62,	0x36,	0x30,	0x61,	0x32,	0x2d,	0x65,
		0x36,	0x31,	0x65,	0x2d,	0x34,	0x61,	0x31,	0x31,
		0x2d,	0x38,	0x64,	0x62,	0x62,	0x2d,	0x33,	0x35,
		0x61,	0x34,	0x65,	0x35,	0x65,	0x63,	0x31,	0x30,
		0x65,	0x66,	0x22,	0x07,	0x74,	0x6d,	0x70,	0x2e,
		0x74,	0x78,	0x74,	0x28,	0xe1,	0x01
	};

	pdu.Write(body, 70); //构造pdu体

	DB_PROXY::addOfflineFile(&pdu, 11);

	return 0;
}

int main(){

	test_addofflinefile();

	//这里mysql连接池未释放存在内存泄漏问题，解决方法参考test_dbpool

	return 0;
}
