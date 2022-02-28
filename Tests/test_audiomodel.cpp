//
//  test_audiomodel.cpp
//  test_audiomodel
//
//  Created by blueBling on 22-2-24.
//  Copyright (c) 2022年blueBling. All rights reserved.
//

#include "AudioModel.h"

#include <stdint.h>

#include <iostream>


using std::cout;
using std::endl;


int test_audiomodel(){

	uint32_t nFromId = 17;
	uint32_t nToId = 1;
	uint32_t nCreateTime = 1645663528;
	char pMsgContent[1024] = {0x00, 0x00, 0x00, 0x04, 0x01, 0x02, 0x03, 0x04};
	uint32_t nMsgLen = 8;

	CAudioModel* pAudioModel = CAudioModel::getInstance();

	string strFileSite("127.0.0.1");
	pAudioModel->setUrl(strFileSite);
	
	int nAudioId = pAudioModel->saveAudioInfo(nFromId, nToId, nCreateTime, pMsgContent, nMsgLen);

	bool bRet = true;
	if (nAudioId != -1) {
		cout << "nAudioId:" << nAudioId << endl; 
	}

	delete pAudioModel;
	pAudioModel = NULL;
	
	return 0;
}

int main(){
	test_audiomodel();
	return 0;
}