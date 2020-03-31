#pragma once
#include <string>
#include <vector>
#include "amqp_tcp_socket.h"
 
using namespace std;

namespace userLoginService{
namespace infrastructure{


 
class RabbitmqClient
{

public:
    RabbitmqClient();
    virtual ~RabbitmqClient();
 
 
    int connect(const string &strHostname, int iPort, const string &strUser, const string &strPasswd);
    int disconnect();
 
    
	//声明exchange
    int exchangeDeclare(const string &strExchange, const string &strType);
 
    //声明消息队列
    int queueDeclare(const string &strQueueName);
 
    //将队列，交换机和绑定规则绑定起来形成一个路由表
    int queueBind(const string &strQueueName, const string &strExchange, const string &strBindKey);
 
    //将队列，交换机和绑定规则绑定解除
    int queueUnbind(const string &strQueueName, const string &strExchange, const string &strBindKey);
 
    //删除消息队列
    int queueDelete(const string &strQueueName, int iIfUnused);
 
    //发布消息
    int publish(const string &strMessage, const string &strExchange, const string &strRoutekey);
 
    //消费消息
    int consumer(const string &strQueueName, vector<string> &message_array, int GetNum = 1, struct timeval *timeout = NULL);
 
 
private:
    RabbitmqClient(const RabbitmqClient& rc);
    void operator=(const RabbitmqClient& rc);
 
    int ErrorMsg(amqp_rpc_reply_t x, char const *context);
 
    string                      m_strHostname;      // amqp主机
    int                         m_iPort;            // amqp端口
    string					    m_strUser;
    string					    m_strPasswd;
    int                         m_iChannel; 
 
    amqp_socket_t               *m_pSock;        
    amqp_connection_state_t     m_pConn;

};




}
}