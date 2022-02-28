//
//  test_config.cpp
//  test_config
//
//  Created by coderl on 22-1-12.
//  Copyright (c) 2022年blueBling. All rights reserved.
//

#include "ConfigFileReader.h"

int test_config(){
	CConfigFileReader config_file("dbproxyserver.conf");
	char* listen_ip = config_file.GetConfigName("ListenIP");
	char* str_listen_port = config_file.GetConfigName("ListenPort");
	char* str_thread_num = config_file.GetConfigName("ThreadNum");
    char* str_file_site = config_file.GetConfigName("MsfsSite");
    char* str_aes_key = config_file.GetConfigName("aesKey");

	printf("listen_ip:%s str_listen_port:%s str_thread_num:%s\r\n", listen_ip, str_listen_port, str_thread_num);
	printf("str_file_site:%s str_aes_key:%s\r\n", str_file_site, str_aes_key);
	
	return 0;
}

int main(){
	test_config();
	return 0;
}