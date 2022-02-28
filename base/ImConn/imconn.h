/*
 * imconn.h
 *
 *  Created on: 2013-6-5
 *      Author: ziteng
 */

#ifndef IMCONN_H_
#define IMCONN_H_

#include "netlib.h"
#include "util.h"
#include "ImPduBase.h"

#define SERVER_HEARTBEAT_INTERVAL	5000
#define SERVER_TIMEOUT				30000
#define CLIENT_HEARTBEAT_INTERVAL	30000
#define CLIENT_TIMEOUT				120000
#define MOBILE_CLIENT_TIMEOUT       60000 * 5
#define READ_BUF_SIZE	2048

class CImConn : public CRefObject
{
public:
	CImConn();
	virtual ~CImConn();

	bool IsBusy() { return m_busy; } //是否发送数据忙状态
	int SendPdu(CImPdu* pPdu) { return Send(pPdu->GetBuffer(), pPdu->GetLength()); }
	int Send(void* data, int len);

	//下面接口用于实现自己的业务逻辑
	virtual void OnConnect(net_handle_t handle) { m_handle = handle; } //tcp连接处理
	virtual void OnConfirm() {} //连接后处理
	virtual void OnRead();      //收数据
	virtual void OnWrite();     //发数据
	virtual void OnClose() {}   //关闭连接
	virtual void OnTimer(uint64_t curr_tick) {} //定时任务
    virtual void OnWriteCompelete() {}; //发数据完成

	virtual void HandlePdu(CImPdu* pPdu) {} //解析Pdu

protected:
	net_handle_t	m_handle; //socket fd
	bool			m_busy;   //发送数据一次性未发完时，会置该标记，等待下次EPOLLOUT时再发送

	string			m_peer_ip;   //对端服务器ip
	uint16_t		m_peer_port; //对端服务器port
	CSimpleBuffer	m_in_buf;    //收数据缓冲区
	CSimpleBuffer	m_out_buf;   //发数据缓冲区

	bool			m_policy_conn;
	uint32_t		m_recv_bytes;         //记录收数据字节数
	uint64_t		m_last_send_tick;     //记录最后一次发数据时刻
	uint64_t		m_last_recv_tick;     //记录最后一次收数据时刻
    uint64_t        m_last_all_user_tick;
};


typedef hash_map<net_handle_t, CImConn*> ConnMap_t;
typedef hash_map<uint32_t, CImConn*> UserMap_t;

void imconn_callback(void* callback_data, uint8_t msg, uint32_t handle, void* pParam);
void ReadPolicyFile();

#endif /* IMCONN_H_ */
