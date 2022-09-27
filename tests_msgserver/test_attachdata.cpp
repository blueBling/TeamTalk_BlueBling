//
//  test_attachdata.cpp
//  test_attachdata
//
//  Created by blueBling on 22-07-25.
//  Copyright (c) 2022年blueBling. All rights reserved.
//

#include "AttachData.h"
#include "EncDec.h"

#include <iostream>


using std::cout;
using std::endl;

CAes *pAes;


int test_attachdata(){

	pAes = new CAes("12345678901234567890123456789012");

	CDbAttachData attach_data(ATTACH_TYPE_HANDLE, 1, 0);

	delete pAes;
	
	return 0;
}


int main(){
	test_attachdata();
	return 0;
}