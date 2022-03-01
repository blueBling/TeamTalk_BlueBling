/*
 * ImPduBase.h
 *
 *  Created on: 2013-8-27
 *      Author: ziteng@mogujie.com
 */

#ifndef IMPDUBASE_H_
#define IMPDUBASE_H_

#include "UtilPdu.h"
#include "pb/google/protobuf/message_lite.h"

#define IM_PDU_HEADER_LEN		16
#define IM_PDU_VERSION			1


#define ALLOC_FAIL_ASSERT(p) if (p == NULL) { \
throw CPduException(m_pdu_header.service_id, m_pdu_header.command_id, ERROR_CODE_ALLOC_FAILED, "allocate failed"); \
}

#define CHECK_PB_PARSE_MSG(ret) { \
    if (ret == false) \
    {\
        log_error("parse pb msg failed.");\
        return;\
    }\
}



#ifdef WIN32
#ifdef BUILD_PDU
#define DLL_MODIFIER __declspec(dllexport)
#else
#define DLL_MODIFIER __declspec(dllimport)
#endif
#else
#define DLL_MODIFIER
#endif

//////////////////////////////
typedef struct {
    uint32_t 	length;		  // the whole pdu length
    uint16_t 	version;	  // pdu version number
    uint16_t	flag;		  // not used
    uint16_t	service_id;	  //
    uint16_t	command_id;	  //
    uint16_t	seq_num;     // 包序号
    uint16_t    reversed;    // 保留
} PduHeader_t;

class DLL_MODIFIER CImPdu
{
public:
    CImPdu();
    virtual ~CImPdu() {}
    
    uchar_t* GetBuffer();
    uint32_t GetLength();
    uchar_t* GetBodyData();
    uint32_t GetBodyLength();
    
    
    uint16_t GetVersion() { return m_pdu_header.version; }
    uint16_t GetFlag() { return m_pdu_header.flag; }
    uint16_t GetServiceId() { return m_pdu_header.service_id; }
    uint16_t GetCommandId() { return m_pdu_header.command_id; }
    uint16_t GetSeqNum() { return m_pdu_header.seq_num; }
    uint32_t GetReversed() { return m_pdu_header.reversed; }
    
    void SetVersion(uint16_t version);
    void SetFlag(uint16_t flag);
    void SetServiceId(uint16_t service_id);
    void SetCommandId(uint16_t command_id);
    void SetError(uint16_t error);
    void SetSeqNum(uint16_t seq_num);
    void SetReversed(uint32_t reversed);
    void WriteHeader(); //通过m_pdu_header结构体信息构造m_buf字节数组头部信息
    
    static bool IsPduAvailable(uchar_t* buf, uint32_t len, uint32_t& pdu_len); //对数据包头部4个字节即长度来校验buf是否完整
    static CImPdu* ReadPdu(uchar_t* buf, uint32_t len);
    void Write(uchar_t* buf, uint32_t len) { m_buf.Write((void*)buf, len); } //通过buf构造CImPdu内部成员CSimpleBuffer	m_buf
    int ReadPduHeader(uchar_t* buf, uint32_t len); //从buf解析出包头数据，将字节数组转化成struct PduHeader_t,实际是构造CImPdu内部成员PduHeader_t	m_pdu_header
    void SetPBMsg(const google::protobuf::MessageLite* msg); //将protobuf msg序列化再加上pdu header,构造成完整pdu即m_buf
    
protected:
    CSimpleBuffer	m_buf;        //pdu原始数据（header+protobuf序列化后的消息体）
    PduHeader_t		m_pdu_header; //pdu消息头
};


#endif /* IMPDUBASE_H_ */