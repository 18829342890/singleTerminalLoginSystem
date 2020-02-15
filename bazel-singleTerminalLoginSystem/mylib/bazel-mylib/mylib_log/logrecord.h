
#ifndef _LOG_RECORD_H_
#define _LOG_RECORD_H_



/************************************************************************/
/* 
const char *file：文件名称
int line：文件行号
int level：错误级别
		0 -- 没有日志
		1 -- debug级别
		2 -- info级别
		3 -- warning级别
		4 -- err级别
int status：错误码
const char *fmt：可变参数
*/
/************************************************************************/


/*Level类别*/
#define LR_NO_LOG_LEVEL         0
#define LR_DEBUG_LEVEL          1
#define LR_INFO_LEVEL           2
#define LR_WARNING_LEVEL        3
#define LR_ERROR_LEVEL          4

/*实际使用的Level*/
extern int  LogLevel[5];
void LR_LOG(const char *file, int line, int level, const char *fmt, ...);

#define PROCLOG(level, format, arg...)                                         \
	{                                                                          \
		LR_LOG(__FILE__, __LINE__, level, format, ##arg);                      \
	}

#define LOG_ERROR(format, arg...)                                              \
    {                                                                          \
        PROCLOG(LR_ERROR_LEVEL, format, ##arg);                                \
    }

#define LOG_WARNNING(format, arg...)                                           \
    {                                                                          \
        PROCLOG(LR_WARNING_LEVEL, format, ##arg);                              \
    }

#define LOG_INFO(format, arg...)                                               \
    {                                                                          \
        PROCLOG(LR_INFO_LEVEL, format, ##arg);                                 \
    }

#define LOG_DEBUG(format, arg...)                                              \
    {                                                                          \
        PROCLOG(LR_DEBUG_LEVEL, format, ##arg);                                \
    }





#endif
