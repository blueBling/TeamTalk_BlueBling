//
//  test_imconn.cpp
//  test_imconn
//
//  Created by blueBling on 22-2-11.
//  Copyright (c) 2022年blueBling. All rights reserved.
//

#include "imconn.h"

#include<iostream>
#include<string>

#include <unistd.h> //sleep
#include <string.h>

#include "IM.Server.pb.h"
#include "IM.Other.pb.h"
#include "IM.BaseDefine.pb.h"
using namespace IM::BaseDefine;

using std::cout;
using std::endl;
using std::string;

static ConnMap_t g_msg_serv_conn_map;

//处理登陆服务器连接
class CLoginConn : public CImConn
{
public:
		CLoginConn() {}
		~CLoginConn() {};

		void OnConnect2(net_handle_t handle) {
			m_handle = handle;
			//m_conn_type = conn_type;
			ConnMap_t* conn_map = &g_msg_serv_conn_map;
			conn_map->insert(make_pair(handle, this));

			netlib_option(handle, NETLIB_OPT_SET_CALLBACK, (void*)imconn_callback);
			netlib_option(handle, NETLIB_OPT_SET_CALLBACK_DATA, (void*)conn_map);
		}

		void HandlePdu(CImPdu* pPdu) {
			cout << "HandlePdu" << endl;
			//g_msg_serv_conn_map.erase(m_handle);
		}

		void OnTimer(uint64_t curr_tick)
		{
			{
				if (curr_tick > m_last_send_tick + SERVER_HEARTBEAT_INTERVAL) {
		            IM::Other::IMHeartBeat msg;
		            CImPdu pdu;
		            pdu.SetPBMsg(&msg);
		            pdu.SetServiceId(SID_OTHER);
		            pdu.SetCommandId(CID_OTHER_HEARTBEAT);
					SendPdu(&pdu);
					cout << "send CID_OTHER_HEARTBEAT" << endl;
				}

				if (curr_tick > m_last_recv_tick + SERVER_TIMEOUT) {
					cout << "connection to MsgServer timeout " << endl;
					//Close();
				}
			}
		}
};


// msg_server请求连接事件
void msg_serv_callback(void* callback_data, uint8_t msg, uint32_t handle, void* pParam)
{
    cout << "msg_server come in" << endl;

	if (msg == NETLIB_MSG_CONNECT)
	{
		CLoginConn* pConn = new CLoginConn();
		pConn->OnConnect2(handle);
	}
	else
	{
		cout << "!!!error msg: %d " << msg << endl;
	}
}

//模拟登陆服务器定时任务(如心跳)
void login_conn_timer_callback(void* callback_data, uint8_t msg, uint32_t handle, void* pParam)
{
	uint64_t cur_time = get_tick_count();

	for (ConnMap_t::iterator it = g_msg_serv_conn_map.begin(); it != g_msg_serv_conn_map.end(); ) {
		ConnMap_t::iterator it_old = it;
		it++;

		CLoginConn* pConn = (CLoginConn*)it_old->second;
		pConn->OnTimer(cur_time);
	}
}


//模拟login_server和msg_server的连接
int test_imconn() {
	signal(SIGPIPE, SIG_IGN);

	char ip[] = "0.0.0.0";
	uint16_t listen_port = 8100;

	int ret = netlib_listen(ip, listen_port, msg_serv_callback, NULL); //监听msg_server

	if (ret == NETLIB_ERROR)
		return ret;

	netlib_register_timer(login_conn_timer_callback, NULL, 1000);     //注册定时任务

	netlib_eventloop();

	return 0;
}

int main(){
	test_imconn();
	return 0;
}
