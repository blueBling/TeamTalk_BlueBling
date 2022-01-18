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

int main(){
	test_simplebuffer();
	test_byteArry();
	return 0;
}