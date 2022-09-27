//
//  test_synccenter.cpp
//  test_synccenter
//
//  Created by blueBling on 22-04-28.
//  Copyright (c) 2022年blueBling. All rights reserved.
//

#include "SyncCenter.h"
#include "util.h"

#include <iostream>


using std::cout;
using std::endl;

string strAudioEnc;

int test_synccenter() {

	log("startSync");
	CSyncCenter::getInstance()->startSync();
	sleep(10);
	CSyncCenter::getInstance()->stopSync();
	log("stopSync");

	return 0;
}


int main(){

	test_synccenter();

	//这里mysql和redis连接池未释放存在内存泄漏问题，解决方法参考test_dbpool
	
	return 0;
}
