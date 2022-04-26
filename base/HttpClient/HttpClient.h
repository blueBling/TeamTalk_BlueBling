/*================================================================
 *   Copyright (C) 2014 All rights reserved.
 *
 *   文件名称：HttpClient.h
 *   创 建 者：Zhang Yuanhao
 *   邮    箱：bluefoxah@gmail.com
 *   创建日期：2014年08月14日
 *   描    述：
 *
 #pragma once
 ================================================================*/

#ifndef __HTTP_CURL_H__
#define __HTTP_CURL_H__

#include <string>
#include <curl/curl.h>
#include "public_define.h"

class CHttpClient
{
public:
    CHttpClient(void);
    ~CHttpClient(void);
    
public: 
	//http post请求 strUrl为请求地址 strPost为post内容 strResponse保存响应结果
    CURLcode Post(const string & strUrl, const string & strPost, string & strResponse);
    //http get请求
    CURLcode Get(const string & strUrl, string & strResponse);
    //http 上传语音数据到url指定上传位置 
    string UploadByteFile(const string &url, void* data, int data_len);
    //http 从指定url下载语音数据
    bool DownloadByteFile(const string &url, AudioMsgInfo* pAudioMsg);
};

#endif