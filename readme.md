# 单终端登录系统设计文档

## 需求

- 具备注册登录功能
- 一个用户只能在一个设备上登录，切换终端登录时，其它已登录的终端需被踢出
- 后台可以将已登录的设备踢出





## 存储设计

- 用户名密码表

```
create table user.t_user_password(
	`FuiId` bigint(20) unsigned NOT NULL AUTO_INCREMENT COMMENT '自增主键',
	`FstrUserName` varchar(256) NOT NULL DEFAULT '' COMMENT '用户名',
	`FstrPassWord` varchar(64) NOT NULL DEFAULT '' COMMENT '加密之后的密码',
	`FstrSalt` varchar(64) NOT NULL DEFAULT '' COMMENT '加密密码所用的盐',
	`FuiCreateTime` bigint(20) unsigned NOT NULL DEFAULT '0' COMMENT '创建时间',
  `FuiUpdateTime` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP COMMENT '更新时间',
	PRIMARY KEY (`FuiId`),
	UNIQUE KEY `u_idx_username` (`FstrUserName`),
	KEY `idx_update_time` (`FuiUpdateTime`)
)ENGINE=InnoDB AUTO_INCREMENT=1 DEFAULT CHARSET=utf8 COMMENT='用户密码表';
```





- 用户登录管理表
```
create table user.t_user_login_manage(
	`FuiId` bigint(20) unsigned NOT NULL AUTO_INCREMENT COMMENT '自增主键',
	`FstrUserName` varchar(256) NOT NULL DEFAULT '' COMMENT '用户名',
	`FstrClientUid` varchar(64) NOT NULL DEFAULT '' COMMENT '客户端uid',
	`FuiStatus` tinyint(1) unsigned NOT NULL DEFAULT '0' COMMENT '登录状态 1:已登录 2:退出登录',
	`FuiCreateTime` bigint(20) unsigned NOT NULL DEFAULT '0' COMMENT '创建时间',
  `FuiUpdateTime` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP COMMENT '更新时间',
	PRIMARY KEY (`FuiId`),
	UNIQUE KEY `u_idx_username` (`FstrUserName`),
	KEY `idx_username_status_clientuid` (`FstrUserName`, `FuiStatus`, `FstrClientUid`),
	KEY `idx_update_time` (`FuiUpdateTime`)
)ENGINE=InnoDB AUTO_INCREMENT=1 DEFAULT CHARSET=utf8 COMMENT='用户登录记录表';
```








## 协议

```
syntax = "proto3";

option java_multiple_files = true;
option java_package = "singleTerminaLoginSystem.protos.loginManageService";
option java_outer_classname = "loginManageProto";
option objc_class_prefix = "TXH";

package proto.userLoginManage;


service userLoginManageService {
  // regist
  rpc regist (RegistRequest) returns (RegistResponse) {}

  // login
  rpc login (LoginRequest) returns (LoginResponse) {}

  // logout
  rpc logout (LogoutRequest) returns (LogoutResponse) {}

  // kick out user
  rpc kickOutUser (KickOutUserRequest) returns (KickOutUserResponse) {}
  
  // query login status
  rpc queryLoginStatus (stream QueryLoginStatusRequest) returns (stream QueryLoginStatusResponse) {}
}


//***
//@request  BasicRequest
//@response BasicResponse
//@remark   请求的基本信息
//***
message BasicRequest
{
	int64   timestamp     = 1; //发送消息的当前时间戳
}

message BasicResponse
{
	int64   timestamp     = 1; //时间戳
	int32   code          = 2; //状态码
	string  msg           = 3; //描述信息
}


//***
//@request  RegistRequest
//@response RegistResponse
//@remark   用户注册
//***
message RegistRequest
{
	BasicRequest             basic     = 1;
	string                   user_name = 2;
	string                   pass_word = 3;
}

message RegistResponse
{
	BasicResponse            basic     = 1;
}


//***
//@request  LoginRequest
//@response LoginResponse
//@remark   用户登录
//***
message LoginRequest
{
	BasicRequest             basic     = 1;
	string                   user_name = 2;
	string                   pass_word = 3;
}

message LoginResponse
{
	BasicResponse            basic     = 1;
}


//***
//@request  LogoutRequest
//@response LogoutResponse
//@remark   退出登录
//***
message LogoutRequest
{
	BasicRequest             basic     = 1;
	string                   user_name = 2;
}

message LogoutResponse
{
	BasicResponse            basic     = 1;
}

//***
//@request  KickOutUserRequest
//@response KickOutUserResponse
//@remark   踢出用户
//***
message KickOutUserRequest
{
	BasicRequest             basic     = 1;
	string                   user_name = 2;
}

message KickOutUserResponse
{
	BasicResponse basic                = 1;
}


//***
//@request  QueryLoginStatusRequest
//@response QueryLoginStatusResponse
//@remark   询问登录状态
//***
message QueryLoginStatusRequest
{
	BasicRequest             basic     = 1;
}

message QueryLoginStatusResponse
{
	BasicResponse            basic     = 1;
	int32                    operation = 2; //应处理的动作 1.无须处理  2. 有新设备登录，此设备需退出登录
}

```









## 服务端框架图
请查看链接:  https://www.processon.com/view/link/5e6fba8fe4b06b852fe028d9






## 时序图、异常场景分析
请查看链接：https://www.processon.com/view/link/5e7b5241e4b08b615739828f



## 安全性
- 敏感数据传输： 账户、密码使用base64_code进行编码，并用grpc自带的ssl加密进行传输。
- 敏感数据存储：使用加盐之后的bcrypt进行加密，设置加密一个密码的所需要的时间大概为1秒。
- mysql安全: 使用实现了JDBC规范的 mysql Connector C++，使用prepareStatement执行sql语句。








