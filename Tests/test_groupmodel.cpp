//
//  test_groupmodel.cpp
//  test_groupmodel
//
//  Created by blueBling on 22-03-15.
//  Copyright (c) 2022年blueBling. All rights reserved.
//

#include "GroupAction.h"
#include "GroupModel.h"
#include "ImPduBase.h"

#include<iostream>


using std::cout;
using std::endl;

//模拟win_client 点击创建群组，成员有3个，id分别是1 17 18
int test_create_group(){

	//(gdb) p pPdu.m_pdu_header 
	//$2 = {length = 53, version = 1, flag = 0, service_id = 4, command_id = 1029, seq_num = 0, reversed = 0}

	
	//(gdb) x/16xb &pPdu.m_pdu_header 
	//0x968378:	0x35	0x00	0x00	0x00	0x01	0x00	0x00	0x00
	//0x968380:	0x04	0x00	0x05	0x04	0x00	0x00	0x00	0x00


	//(gdb) p pPdu.m_buf 
	//$1 = {m_buffer = 0x94bb20 "", m_alloc_size = 66, m_write_offset = 53}


	
	//(gdb) x/53xb pPdu.m_buf.m_buffer
	//0x94bb20:	0x00	0x00	0x00	0x35	0x00	0x01	0x00	0x00
	//0x94bb28:	0x00	0x04	0x04	0x05	0x00	0x00	0x00	0x00
	//0x94bb30:	0x08	0x01	0x10	0x02	0x1a	0x08	0x66	0x61
	//0x94bb38:	0x6d	0x69	0x6c	0x79	0x5f	0x32	0x22	0x00
	//0x94bb40:	0x28	0x01	0x28	0x11	0x28	0x12	0xa2	0x01
	//0x94bb48:	0x0c	0x00	0x00	0x00	0x01	0x00	0x00	0x00
	//0x94bb50:	0x0e	0x00	0x00	0x00	0x00
	
	CImPdu pdu; //构造一个群组创建请求pdu

	
	unsigned char header[16] = {
		0x00,	0x00,	0x00,	0x35,	0x00,	0x01,	0x00,	0x00,
		0x00,	0x04,	0x04,	0x05,	0x00,	0x00,	0x00,	0x00
	};

	pdu.ReadPduHeader(header, 16); //构造pdu头


	unsigned char body[53] = {
		0x00,	0x00,	0x00,	0x35,	0x00,	0x01,	0x00,	0x00,
		0x00,	0x04,	0x04,	0x05,	0x00,	0x00,	0x00,	0x00,
		0x08,	0x01,	0x10,	0x02,	0x1a,	0x08,	0x66,	0x61,
		0x6d,	0x69,	0x6c,	0x79,	0x5f,	0x32,	0x22,	0x00,
		0x28,	0x01,	0x28,	0x11,	0x28,	0x12,	0xa2,	0x01,
		0x0c,	0x00,	0x00,	0x00,	0x01,	0x00,	0x00,	0x00,
		0x0e,	0x00,	0x00,	0x00,	0x00
	};

	pdu.Write(body, 53); //构造pdu体

	DB_PROXY::createGroup(&pdu, 3);
	
	return 0;
}

//模拟win client获取该用户所在永久(GROUP_TYPE_NORMAL)群组列表信息
int test_getnormalgrouplist() {

	//(gdb) p pPdu->m_pdu_header 
	//$1 = {length = 33, version = 1, flag = 0, service_id = 4, command_id = 1025, seq_num = 0, reversed = 0}
	//(gdb) x/16xb &pPdu->m_pdu_header 
	//0x964028:	0x21	0x00	0x00	0x00	0x01	0x00	0x00	0x00
	//0x964030:	0x04	0x00	0x01	0x04	0x00	0x00	0x00	0x00


	//(gdb) p pPdu->m_buf 
	//$2 = {m_buffer = 0x964040 "", m_alloc_size = 41, m_write_offset = 33}
	//(gdb) x/33xb pPdu->m_buf.m_buffer 
	//0x964040:	0x00	0x00	0x00	0x21	0x00	0x01	0x00	0x00
	//0x964048:	0x00	0x04	0x04	0x01	0x00	0x00	0x00	0x00
	//0x964050:	0x08	0x01	0xa2	0x01	0x0c	0x00	0x00	0x00
	//0x964058:	0x01	0x00	0x00	0x00	0x0e	0x00	0x00	0x00
	//0x964060:	0x00

	CImPdu pdu; //构造一个获取该用户所在群组列表信息请求pdu

	
	unsigned char header[16] = {
		0x00,	0x00,	0x00,	0x21,	0x00,	0x01,	0x00,	0x00,
		0x00,	0x04,	0x04,	0x01,	0x00,	0x00,	0x00,	0x00
	};

	pdu.ReadPduHeader(header, 16); //构造pdu头


	unsigned char body[33] = {
		0x00,	0x00,	0x00,	0x21,	0x00,	0x01,	0x00,	0x00,
		0x00,	0x04,	0x04,	0x01,	0x00,	0x00,	0x00,	0x00,
		0x08,	0x01,	0xa2,	0x01,	0x0c,	0x00,	0x00,	0x00,
		0x01,	0x00,	0x00,	0x00,	0x0e,	0x00,	0x00,	0x00,
		0x00
	};

	pdu.Write(body, 33); //构造pdu体

	DB_PROXY::getNormalGroupList(&pdu, 12);

	return 0;
}

//模拟win client移除指定群组里指定成员
int test_modifymember() {

	//(gdb) p pPdu->m_pdu_header 
	//$1 = {length = 39, version = 1, flag = 0, service_id = 4, command_id = 1031, seq_num = 0, reversed = 0}
	//(gdb) x/16xb &pPdu->m_pdu_header 
	//0x966098:	0x27	0x00	0x00	0x00	0x01	0x00	0x00	0x00
	//0x9660a0:	0x04	0x00	0x07	0x04	0x00	0x00	0x00	0x00

	
	//(gdb) p pPdu->m_buf
	//$3 = {m_buffer = 0x960560 "", m_alloc_size = 48, m_write_offset = 39}
	//(gdb) x/39xb pPdu->m_buf.m_buffer 
	//0x960560:	0x00	0x00	0x00	0x27	0x00	0x01	0x00	0x00
	//0x960568:	0x00	0x04	0x04	0x07	0x00	0x00	0x00	0x00
	//0x960570:	0x08	0x01	0x10	0x02	0x18	0x01	0x20	0x11
	//0x960578:	0xa2	0x01	0x0c	0x00	0x00	0x00	0x01	0x00
	//0x960580:	0x00	0x00	0x0e	0x00	0x00	0x00	0x00


	
	CImPdu pdu; //构造一个移除群组成员请求pdu

	
	unsigned char header[16] = {
		0x00,	0x00,	0x00,	0x27,	0x00,	0x01,	0x00,	0x00,
		0x00,	0x04,	0x04,	0x07,	0x00,	0x00,	0x00,	0x00
	};

	pdu.ReadPduHeader(header, 16); //构造pdu头


	unsigned char body[39] = {
		0x00,	0x00,	0x00,	0x27,	0x00,	0x01,	0x00,	0x00,
		0x00,	0x04,	0x04,	0x07,	0x00,	0x00,	0x00,	0x00,
		0x08,	0x01,	0x10,	0x02,	0x18,	0x01,	0x20,	0x11,
		0xa2,	0x01,	0x0c,	0x00,	0x00,	0x00,	0x01,	0x00,
		0x00,	0x00,	0x0e,	0x00,	0x00,	0x00,	0x00
	};

	pdu.Write(body, 39); //构造pdu体

	DB_PROXY::modifyMember(&pdu, 4);

	return 0;
}

//模拟win client显示群消息
int test_setgrouppush() {

	//(gdb) p pPdu->m_pdu_header 
	//$1 = {length = 37, version = 1, flag = 0, service_id = 4, command_id = 1033, seq_num = 0, reversed = 0}
	//(gdb) x/16xb &pPdu->m_pdu_header 
	//0x964d18:	0x25	0x00	0x00	0x00	0x01	0x00	0x00	0x00
	//0x964d20:	0x04	0x00	0x09	0x04	0x00	0x00	0x00	0x00
	//(gdb) p pPdu->m_buf 
	//$2 = {m_buffer = 0x963d60 "", m_alloc_size = 46, m_write_offset = 37}
	//(gdb) x/37xb pPdu->m_buf->m_buffer 
	//0x963d60:	0x00	0x00	0x00	0x25	0x00	0x01	0x00	0x00
	//0x963d68:	0x00	0x04	0x04	0x09	0x00	0x00	0x00	0x00
	//0x963d70:	0x08	0x01	0x10	0x01	0x18	0x01	0xa2	0x01
	//0x963d78:	0x0c	0x00	0x00	0x00	0x01	0x00	0x00	0x00
	//0x963d80:	0x0e	0x00	0x00	0x00	0x00

	CImPdu pdu; //构造一个显示群消息请求pdu

	
	unsigned char header[16] = {
		0x00,	0x00,	0x00,	0x25,	0x00,	0x01,	0x00,	0x00,
		0x00,	0x04,	0x04,	0x09,	0x00,	0x00,	0x00,	0x00
	};

	pdu.ReadPduHeader(header, 16); //构造pdu头


	unsigned char body[37] = {
		0x00,	0x00,	0x00,	0x25,	0x00,	0x01,	0x00,	0x00,
		0x00,	0x04,	0x04,	0x09,	0x00,	0x00,	0x00,	0x00,
		0x08,	0x01,	0x10,	0x01,	0x18,	0x01,	0xa2,	0x01,
		0x0c,	0x00,	0x00,	0x00,	0x01,	0x00,	0x00,	0x00,
		0x0e,	0x00,	0x00,	0x00,	0x00
	};

	pdu.Write(body, 37); //构造pdu体


	DB_PROXY::setGroupPush(&pdu, 3);


	return 0;
}

int main(){
	//test_create_group();
	//test_getnormalgrouplist();
	//test_modifymember();
	test_setgrouppush();
	return 0;
}
