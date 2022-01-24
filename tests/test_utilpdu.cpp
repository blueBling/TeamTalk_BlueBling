//
//  test_utilpdu.cpp
//  test_utilpdu
//
//  Created by coderl on 22-1-12.
//  Copyright (c) 2022年blueBling. All rights reserved.
//

#include "UtilPdu.h"

#include<stdint.h> //uint16_t
#include<stdio.h>
#include<string.h>

#include<vector>

using std::vector;

int test_simplebuffer(){
	CSimpleBuffer	in_buf;
	in_buf.Extend(32);

	char src[16] = "hello";
	
	in_buf.Write(src, strlen(src));

	printf("in_buf:%s\r\n", in_buf.GetBuffer());
	printf("in_buf allocsize:%d writeoffset:%d\r\n", in_buf.GetAllocSize(), in_buf.GetWriteOffset());

	char dst[16] = "";
	in_buf.Read(dst, in_buf.GetWriteOffset());
	printf("dst is:%s\r\n", dst);
	
	return 0;
}

int test_byteArry(){
	vector<uint16_t> vec = {1, 2, 3, 4, 5, 6, 7, 8};

	unsigned char* pDst = (unsigned char*)malloc(16 * sizeof(unsigned char));
	memset(pDst, '\0', 16 * sizeof(unsigned char));
	for(int i = 0; i < vec.size(); i++) {
		CByteStream::WriteUint16(pDst + i * 2, vec[i]);
	}

	for(int j = 0; j < 16; j += 2) {
		uint16_t num = CByteStream::ReadUint16(pDst + j);
		printf("num is:%d\r\n", num);
	}


	free(pDst);
	pDst = NULL;
	
	return 0;
}

typedef struct PduHeader{
    uint32_t 	length;		  // the whole pdu length
    uint16_t 	version;	  // pdu version number
    uint16_t	flag;		  // not used
    uint16_t	service_id;	  //
    uint16_t	command_id;	  //
    uint16_t	seq_num;     // 包序号
    uint16_t    reversed;    // 保留
} PduHeader_t;

int test_byteArry_2() {
	uchar_t in_buf[80] = {0x00, 0x00, 0x00, 0x10, 0x00, 0x01, 0x00, 0x00,
						  0x00, 0x07, 0x07, 0x01, 0x00, 0x00, 0x00, 0x00};
	CByteStream is(in_buf, 16);

	PduHeader_t pdu_header;
	memset(&pdu_header, '\0', sizeof(PduHeader_t));

	is >> pdu_header.length;
	is >> pdu_header.version;
	is >> pdu_header.flag;
	is >> pdu_header.service_id;
	is >> pdu_header.command_id;
	is >> pdu_header.seq_num;
    is >> pdu_header.reversed;

	printf("length:%d version:%u flag:%d service_id:%d command_id:%d seq_num:%d reversed:%d\r\n", 
			pdu_header.length, pdu_header.version, pdu_header.flag, pdu_header.service_id, pdu_header.command_id, pdu_header.seq_num, pdu_header.reversed);

	return 0;
}

int test_pduexception() {
	try {
		throw CPduException(1, "pdu_len is 0");
	} catch (CPduException& ex) {
		printf("catch exception, err_code=%u, err_msg=%s, close the connection\r\n", ex.GetErrorCode(), ex.GetErrorMsg());
	}

	return 0;
}

int main(){
	test_simplebuffer();
	test_byteArry();
	test_byteArry_2();
	test_pduexception();
	return 0;
}