#include "receiveLogoutNotice.h"

using namespace client::service;

extern int isNoticeLogout;



Status receiveLogoutNotice::noticeClientLogout(ServerContext* context, const NoticeClientLogoutRequest* request, NoticeClientLogoutResponse* response)
{
	//打印消息
	string message = request->message();
	printf("%s\n", message.c_str());

	//设置isNoticeLogout为已收到退出通知
	isNoticeLogout = 1;

	//返回
	context->set_compression_algorithm(GRPC_COMPRESS_DEFLATE);
    response->set_message("client got server message.");
    return Status::OK;
}