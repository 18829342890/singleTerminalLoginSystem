#include "rabbitmqClient.h"
#include "mylib/mylibLog/logrecord.h"
#include <unistd.h>
 
using namespace userLoginService::infrastructure;
 
RabbitmqClient::RabbitmqClient()
    : m_strHostname(""),
      m_iPort(0),
      m_strUser(""),
      m_strPasswd(""),
      m_iChannel(1),  //默认用1号通道，通道无所谓 
      m_pSock(NULL),
      m_pConn(NULL) 
{}
 
RabbitmqClient::~RabbitmqClient() 
{
}
 
int RabbitmqClient::connect(const string &strHostname, int iPort, const string &strUser, const string &strPasswd) 
{
    m_strHostname = strHostname;
    m_iPort = iPort;
    m_strUser = strUser;
    m_strPasswd = strPasswd;
 
    m_pConn = amqp_new_connection();
    if (NULL == m_pConn) {
        LOG_ERROR("amqp new connection failed");
        return -1;
    }
 
    m_pSock =  amqp_tcp_socket_new(m_pConn);
    if (NULL == m_pSock) {
        LOG_ERROR("amqp tcp new socket failed");
        return -2;
    }
 
    int status = amqp_socket_open(m_pSock, m_strHostname.c_str(), m_iPort);
    if (status<0) {
        LOG_ERROR("amqp socket open failed");
        return -3;
    }
 
    if (0 != ErrorMsg(amqp_login(m_pConn, "/", 0, 131072, 0, AMQP_SASL_METHOD_PLAIN, m_strUser.c_str(), m_strPasswd.c_str()), "Logging in")) {
        return -4;
    }
    
    amqp_channel_open(m_pConn, m_iChannel);
    if(0 != ErrorMsg(amqp_get_rpc_reply(m_pConn), "open channel")) 
    {
        amqp_channel_close(m_pConn, m_iChannel, AMQP_REPLY_SUCCESS);
        return -5;
    }

    return 0;
}
 
int RabbitmqClient::disconnect() 
{
    if (NULL != m_pConn) 
    {
        amqp_channel_close(m_pConn, m_iChannel, AMQP_REPLY_SUCCESS);

        if (0 != ErrorMsg(amqp_connection_close(m_pConn, AMQP_REPLY_SUCCESS), "Closing connection"))
            return -1;
 
        if (amqp_destroy_connection(m_pConn) < 0)
            return -2;
 
        m_pConn = NULL;
    }
 
    return 0;
}
 
int RabbitmqClient::exchangeDeclare(const string &strExchange, const string &strType) 
{
    amqp_bytes_t _exchange = amqp_cstring_bytes(strExchange.c_str());
    amqp_bytes_t _type = amqp_cstring_bytes(strType.c_str());
    int _passive= 0;
    int _durable= 0;
    amqp_exchange_declare(m_pConn, m_iChannel, _exchange, _type, _passive, _durable, 0, 0, amqp_empty_table);
    if (0 != ErrorMsg(amqp_get_rpc_reply(m_pConn), "exchange_declare")) 
    {
        return -1;
    }

    return 0;
}
 
int RabbitmqClient::queueDeclare(const string &strQueueName) 
{
    if(NULL == m_pConn) 
    {
        LOG_ERROR("QueueDeclare m_pConn is null");
        return -1;
    }
 
    amqp_bytes_t _queue = amqp_cstring_bytes(strQueueName.c_str());
    int32_t _passive = 0;
    int32_t _durable = 0; 
    int32_t _exclusive = 0;
    int32_t _auto_delete = 0;
    amqp_queue_declare(m_pConn, m_iChannel, _queue, _passive, _durable, _exclusive, _auto_delete, amqp_empty_table);
    if (0 != ErrorMsg(amqp_get_rpc_reply(m_pConn), "queue_declare")) 
    {
        return -2;
    }

    return 0;
}
 
int RabbitmqClient::queueBind(const string &strQueueName, const string &strExchange, const string &strBindKey) 
{
    if(NULL == m_pConn) 
    {
        LOG_ERROR("QueueBind m_pConn is null");
        return -1;
    }
 
    amqp_bytes_t _queue = amqp_cstring_bytes(strQueueName.c_str());
    amqp_bytes_t _exchange = amqp_cstring_bytes(strExchange.c_str());
    amqp_bytes_t _routkey  = amqp_cstring_bytes(strBindKey.c_str());
    amqp_queue_bind(m_pConn, m_iChannel, _queue, _exchange, _routkey, amqp_empty_table);
    if(0 != ErrorMsg(amqp_get_rpc_reply(m_pConn), "queue_bind")) 
    {
        return -2;
    }
 
    return 0;
}
 
int RabbitmqClient::queueUnbind(const string &strQueueName, const string &strExchange, const string &strBindKey) 
{
    if(NULL == m_pConn) 
    {
        LOG_ERROR("QueueUnbind m_pConn is null");
        return -1;
    }
 
    amqp_bytes_t _queue = amqp_cstring_bytes(strQueueName.c_str());
    amqp_bytes_t _exchange = amqp_cstring_bytes(strExchange.c_str());
    amqp_bytes_t _routkey  = amqp_cstring_bytes(strBindKey.c_str());
    amqp_queue_unbind(m_pConn, m_iChannel, _queue, _exchange, _routkey, amqp_empty_table);
    if(0 != ErrorMsg(amqp_get_rpc_reply(m_pConn), "queue_unbind")) 
    {
        return -2;
    }

    return 0;
}
 
int RabbitmqClient::queueDelete(const string &strQueueName, int iIfUnused) 
{
    if(NULL == m_pConn) 
    {
        LOG_ERROR("QueueDelete m_pConn is null");
        return -1;
    }

    amqp_queue_delete(m_pConn, m_iChannel, amqp_cstring_bytes(strQueueName.c_str()), iIfUnused, 0);
    if(0 != ErrorMsg(amqp_get_rpc_reply(m_pConn), "delete queue")) 
    {
        return -2;
    }

    return 0;
}
 
int RabbitmqClient::publish(const string &strMessage, const string &strExchange, const string &strRoutekey) 
{
    if (NULL == m_pConn) {
        LOG_ERROR("publish m_pConn is null, publish failed");
        return -1;
    }

    amqp_bytes_t message_bytes;
    message_bytes.len = strMessage.length();
    message_bytes.bytes = (void *)(strMessage.c_str());
 
    amqp_bytes_t exchange = amqp_cstring_bytes(strExchange.c_str());
    amqp_bytes_t routekey = amqp_cstring_bytes(strRoutekey.c_str());
 
    if (0 != amqp_basic_publish(m_pConn, m_iChannel, exchange, routekey, 0, 0, NULL, message_bytes)) 
    {
        LOG_ERROR("publish amqp_basic_publish failed");
        if (0 != ErrorMsg(amqp_get_rpc_reply(m_pConn), "amqp_basic_publish")) 
        {
            return -2;
        }
    }

    return 0;
}
 
int RabbitmqClient::consumer(const string &strQueueName, vector<string> &messageArray, int getNum, struct timeval *timeout) 
{
    if (NULL == m_pConn) 
    {
        LOG_ERROR("Consumer m_pConn is null, Consumer failed");
        return -1;
    }

    int ack = 0; // no_ack    是否需要确认消息后再从队列中删除消息
    amqp_bytes_t queuename= amqp_cstring_bytes(strQueueName.c_str());
    amqp_basic_consume(m_pConn, m_iChannel, queuename, amqp_empty_bytes, 0, ack, 0, amqp_empty_table);
    if (0 != ErrorMsg(amqp_get_rpc_reply(m_pConn), "Consuming")) 
    {
        return -2;
    }
 
    int hasget = 0;
    amqp_rpc_reply_t res;
    amqp_envelope_t envelope;
    while (getNum > 0) {
        LOG_INFO("amqp_consume_message begin...");
        amqp_maybe_release_buffers(m_pConn);
        res = amqp_consume_message(m_pConn, &envelope, timeout, 0);
        if (AMQP_RESPONSE_NORMAL != res.reply_type) 
        {
            ErrorMsg(res, "amqp_consume_message");
            if (0 == hasget)
                return -res.reply_type;
            else
                return 0;
        }
 
        string str((char *)envelope.message.body.bytes, (char *)envelope.message.body.bytes + envelope.message.body.len);
        messageArray.push_back(str);
        int rtn = amqp_basic_ack(m_pConn, m_iChannel, envelope.delivery_tag, 1);
        amqp_destroy_envelope(&envelope);
        if (rtn != 0) 
        {
            LOG_ERROR("Consumer amqp_basic_ack failed");
            return -4;
        }
 
        getNum--;
        hasget++;
        usleep(1);
    }

    LOG_INFO("consume end!");
    return 0;
}
 
int RabbitmqClient::ErrorMsg(amqp_rpc_reply_t x, char const *context) 
{
    switch (x.reply_type) 
    {
        case AMQP_RESPONSE_NORMAL:
            return 0;
 
        case AMQP_RESPONSE_NONE:
            LOG_ERROR("%s: missing RPC reply type!", context);
            break;
 
        case AMQP_RESPONSE_LIBRARY_EXCEPTION:
            LOG_ERROR("%s: %s", context, amqp_error_string2(x.library_error));
            break;
 
        case AMQP_RESPONSE_SERVER_EXCEPTION:
            switch (x.reply.id) 
            {
                case AMQP_CONNECTION_CLOSE_METHOD: {
                    amqp_connection_close_t *m = (amqp_connection_close_t *)x.reply.decoded;
                    LOG_ERROR("%s: server connection error %uh, message: %.*s",context, m->reply_code, (int)m->reply_text.len,(char *)m->reply_text.bytes);
                    break;
                }
                case AMQP_CHANNEL_CLOSE_METHOD:    {
                    amqp_channel_close_t *m = (amqp_channel_close_t *)x.reply.decoded;
                    LOG_ERROR("%s: server channel error %uh, message: %.*s",context, m->reply_code, (int)m->reply_text.len,(char *)m->reply_text.bytes);
                    break;
                }
                default:
                    LOG_ERROR("%s: unknown server error, method id 0x%08X",context, x.reply.id);
                    break;
            }
            break;
        default:
            LOG_ERROR("unknown x.reply_type! x.reply_type:%d", x.reply_type);
            break;
    }
 
    return -1;
}