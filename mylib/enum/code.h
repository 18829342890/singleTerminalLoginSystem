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
	PASSWORD_FORMAT_ERROR     = -7,
	GEN_SALT_FAILED           = -8,
	ENCRYPT_FAILED            = -9,
	PUBLISH_MSG_FAILED        = -10,

	
};

//登录的状态
enum LOGIN_STATUS
{
	LOGINED                   = 1, //已登录
	LOGOUT                    = 2, //退出登录 
};

enum CLIENT_OPERATION
{
	NOTHING                   = 0, //无须做任何操作
	CURRENT_USER_LOGOUT       = 1, //当前用户已退出登录,客户端可自定义操作
	OTHER_DEVICE_LOGINED_JUST_LOGOUT = 2, //已在其它设备上登录，当前设备应退出登录
	KICKOUT_BY_MANAGER_JUST_LOGOUT   = 3, //被管理员踢出，应退出登录
};


}
}





#endif