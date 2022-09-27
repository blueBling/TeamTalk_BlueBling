//
//  test_relationmodel.cpp
//  test_relationmodel
//
//  Created by blueBling on 22-04-06.
//  Copyright (c) 2022年blueBling. All rights reserved.
//

#include "RelationModel.h"


#include<iostream>


using std::cout;
using std::endl;

string strAudioEnc;

//从mysql IMRelationShip表中获取用户id为1和17关系记录所在id
int test_getrelationid() {

	uint32_t id = CRelationModel::getInstance()->getRelationId(1, 17, false);

	cout << id << endl;

	return 0;
}


int main(){

	test_getrelationid();


	//这里mysql和redis连接池未释放存在内存泄漏问题，解决方法参考test_dbpool
	
	return 0;
}
