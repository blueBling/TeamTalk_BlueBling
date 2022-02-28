//
//  test_util.cpp
//  test_util
//
//  Created by blueBling on 22-2-18.
//  Copyright (c) 2022年blueBling. All rights reserved.
//

#include "util.h"

#include<iostream>


using std::cout;
using std::endl;


int test_util(){

	uint64_t tick = get_tick_count();
	cout << "tick:" << tick << endl;
	
	return 0;
}

int main(){
	test_util();
	return 0;
}