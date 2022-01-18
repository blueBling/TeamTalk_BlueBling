//
//  test_encdec.cpp
//  test_encdec
//
//  Created by coderl on 22-1-12.
//  Copyright (c) 2022年blueBling. All rights reserved.
//

#include "UtilPdu.h"
#include "Base64.h"
#include "EncDec.h"

#include<stdint.h> //uint16_t
#include<stdio.h>
#include<string.h>

#include<vector>
#include<string>
#include<iostream>

using std::vector;
using std::string;
using std::cout;
using std::endl;

int test_base64(){
	string strInData("123456");
    string strResult = base64_encode(strInData);
	string strOutData = base64_decode(strResult);

	cout << "strResult:" << strResult << " strOutData:" << strOutData << endl;
	
	return 0;
}

int test_encdec(){
	string strAesKey("12345678901234567890123456789012", 32);
    CAes cAes = CAes(strAesKey);

	string strAudioEnc;
    string strAudio = "[语音]";
    char* pAudioEnc;
    uint32_t nOutLen;
    if(cAes.Encrypt(strAudio.c_str(), strAudio.length(), &pAudioEnc, nOutLen) == 0)
    {
        strAudioEnc.clear();
        strAudioEnc.append(pAudioEnc, nOutLen);
		cout << "strAudioEnc:" << strAudioEnc << " nOutLen:" << nOutLen << endl;
        cAes.Free(pAudioEnc);
    }

	char* pAudioDec;
	if(cAes.Decrypt(strAudioEnc.c_str(), nOutLen, &pAudioDec, nOutLen) == 0)
	{
		cout << "pAudioDec:" << pAudioDec << " nOutLen:" << nOutLen << endl;
		cAes.Free(pAudioDec);
	}
	
	return 0;
}


int test_md5calculate(){
    char szMd5[33] = "";
	string strInPass("hello");
    CMd5::MD5_Calculate(strInPass.c_str(), strInPass.length(), szMd5);
	cout << "szMd5:" << szMd5 << endl;
	return 0;
}

int main(){
	test_base64();
	test_encdec();
	test_md5calculate();
	return 0;
}