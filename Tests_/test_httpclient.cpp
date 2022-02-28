//
//  test_httpclient.cpp
//  test_httpclient
//
//  Created by blueBling on 22-2-15.
//  Copyright (c) 2022年blueBling. All rights reserved.
//

#include "HttpClient.h"

int test_httpclient(){
	CHttpClient httpClient;
    string strUrl = "http://192.168.49.128:8080/msg_server";   // 连接login服务器
    string strResp;
    CURLcode nRet = httpClient.Get(strUrl, strResp);
    if(nRet != CURLE_OK)
    {
        printf("login falied. access url:%s error\n", strUrl.c_str());
        return -1;
    }
    printf("CClient::connect %s\n", strResp.c_str());
    Json::Reader reader;
    Json::Value value;
    if(!reader.parse(strResp, value))
    {
        printf("login falied. parse response error:%s\n", strResp.c_str());
        return -1;
    }
    string strPriorIp, strBackupIp;
    uint16_t nPort;
    try {
        uint32_t nRet = value["code"].asUInt();
        if(nRet != 0)
        {
            string strMsg = value["msg"].asString();
            printf("login falied. errorMsg:%s\n", strMsg.c_str());
            return -1;
        }
        strPriorIp = value["priorIP"].asString();
        strBackupIp = value["backupIp"].asString();
        nPort = (uint16_t)atoi(value["port"].asString().c_str());
    } 
    catch (std::runtime_error msg) {
        printf("login falied. get json error:%s\n", strResp.c_str());
        return -1;
    }
    
	return 0;
}


int main(){
	test_httpclient();
	return 0;
}
