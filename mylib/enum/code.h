#ifndef __CODE_H__
#define __CODE_H__

namespace userLoginSystem{
namespace myEnum{

//服务端错误码
enum REGIST_LOGIN_CODE
{
	SUCCESS                   = 0,
	SYSTEM_ERROR              = -1,
	DB_OP_FAILED              = -2,
	ALREADY_REGIST            = -3,
	ALREADY_LOGINED           = -4,
	USERNAME_OR_PASSWD_ERROR  = -5,
	USER_NOT_LOGINED          = -6,

	
};


//退出登录请求的类型
enum LOGOUT_REQ_TYPE
{
	USER_LOGOUT               = 1, //用户自己退出登录
	ADMINISTRATOR_KICK_OUT    = 2, //管理员踢出用户退出登录
};


//登录的状态
enum LOGIN_STATUS
{
	LOGINED                   = 1, //已登录
	LOGOUT                    = 2, //退出登录 
};


}
}





#endif