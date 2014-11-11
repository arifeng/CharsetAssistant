/* charset: GBK, Tab: 4
*
* @file: config.h
* @desc: utils配置文件
* @auth: liwei (www.leewei.org)
* @mail: ari.feng@qq.com
* @date: 2012/11/29
* @mdfy: 2012/11/29
* @Vern: 1.0
*/

#ifndef CONFIG_UTIL_H
#define CONFIG_UTIL_H

#include "deps/platform.h"                /* 系统平台和处理器架构宏定义 */

/************************************************************************/
/*                    Settings & Options 设置选项                            */
/************************************************************************/

/* 
 * 运行时内存调试
 * 即时的内存分析，在程序结束后打印统计信息
 */
#ifdef _DEBUG
#define DBG_MEM_RT            1
#endif

/*
 * 内存调试日志 
 * 将分配/释放内存的操作记录在系统日志，以便分析优化
 * 注：若要使用此功能，下面的 USE_DEBUG_LOG 选项也必须开启
 */
//#define DBG_MEM_LOG        1

/*
 * 开启默认调试日志
 * 程序在启动时会在启动目录下自动打开ID为0的日志文件，
 * 在程序运行中随时可用log_dprintf(fmt,...)向其写入记录信息
 * 程序退出时会自动关闭此日志文件，当再次启动打开时为追加模式。
 */
#define USE_DEBUG_LOG        1

/*
 * 字符串参数编码（仅针对Windows）
 * 是否使用UTF-8作为默认的多字节字符串编码
 */
#define USE_UTF8_STR        1

/*
 * 崩溃转储文件
 * 是否在程序异常崩溃时产生转储文件，以便调试分析
 * 转储文件会被保存于临时目录下
 */
//#define USE_CRASH_HANDLER    1

/*
 * 是否启用读写锁 
 */
//#define USE_READ_WRITE_LOCK	 1

/*
 * 是否启用条件变量
 */
//#define USE_CONDITION_VARIABLE	1

/************************************************************************/
/*                    3rd party libraries 第三方依赖库                     */
/************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

/* libiconv */
#ifdef OS_WIN
#define USING_STATIC_LIBICONV
#include "third_party/iconv/iconv.h"
#ifdef _DEBUG
#pragma comment(lib, "utils/third_party/iconv/Win32/iconvd.lib")
#else
#pragma comment(lib, "utils/third_party/iconv/Win32/iconv.lib")
#endif
#else /* OS_WIN */
#include <iconv.h>
#ifndef _LIBICONV_H
#define _LIBICONV_H 1
#endif
#endif /* OS_WIN */

#ifdef __cplusplus
}
#endif

/************************************************************************/
/*                                Macros 宏定义                            */
/************************************************************************/

/* 内存调试 */
#if (defined DBG_MEM_LOG || defined DBG_MEM_RT)
#define DBG_MEM
#else
#undef DBG_MEM
#endif

#if !defined(_DEBUG) && !defined(NDEBUG)
#error "No build mode specified"
#elif defined(_DEBUG) && defined(NDEBUG)
#error "Only one build mode can be specified"
#endif

#endif /* #define CONFIG_UTIL_H */
