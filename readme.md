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
option java_package = "singleTerminaLoginSystem.protos.messageReceiver";
option java_outer_classname = "messageReceiverProto";
option objc_class_prefix = "TXH";

package proto.messageReceiver;

service messageReceiver {
  // regist
  rpc regist (RegistRequest) returns (RegistResponse) {}

  // login
  rpc login (LoginRequest) returns (LoginResponse) {}

  // logout
  rpc logout (LogoutRequest) returns (LogoutResponse) {}

	// noticeClientLogout
  rpc noticeClientLogout (NoticeClientLogoutRequest) returns (NoticeClientLogoutResponse) {}

  // syncClientInfo
  rpc syncClientInfo (SyncClientInfoRequest) returns (SyncClientInfoResponse) {}
}


//***
//@request RegistRequest
//@response RegistResponse
//@remark  用户注册
//***
message RegistRequest
{
	string user_name = 1;
	string pass_word = 2;
}

message RegistResponse
{
	int32  code    = 1;
	string message = 2;
}

//***
//@request LoginRequest
//@response LoginResponse
//@remark  用户登录
//***
message LoginRequest
{
	string user_name = 1;
	string pass_word = 2;
}

message LoginResponse
{
	int32  code    = 1;
	string message = 2;
}

//***
//@request LogoutRequest
//@response LogoutResponse
//@remark  退出登录
//***
message LogoutRequest
{
	string user_name   = 1;
	int32  logout_type = 2; //1:用户自己退出登录 2:管理员踢出用户退出登录
}

message LogoutResponse
{
	int32  code    = 1;
	string message = 2;
}

//***
//@request NoticeClientLogoutRequest
//@response NoticeClientLogoutResponse
//@remark  通知客户端退出登录
//***
message NoticeClientLogoutRequest
{
	string message = 1;
}

message NoticeClientLogoutResponse
{
	int32  code    = 1;
	string message = 2;
}

//***
//@request  SyncClientInfoRequest
//@response SyncClientInfoResponse
//@remark  同步客户端信息
//***
message SyncClientInfoRequest
{
	string user = 1;
	string ip   = 2;
	int32  port = 3; 
}

message SyncClientInfoResponse
{
	int32  code    = 1;
	string message = 2;
}
```





## 交互图

请查看链接：https://www.processon.com/view/link/5e4ffe99e4b0362764fbccf2



## 时序图
请查看链接：https://www.processon.com/view/link/5e53f53fe4b0c037b5fd4c9f


## 安全性
- 敏感数据传输： 密码使用base64_code进行编码，并用grpc自带的ssl加密进行传输。
- 敏感数据存储：使用加盐之后的bcrypt进行加密，设置加密一个密码的所需要的时间大概为1秒。








