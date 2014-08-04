/**
 *
 * @file: WubiCode.h
 * @desc: 五笔字型编码查询
 * @auth: liwei (www.leewei.org)
 * @mail: ari.feng@qq.com
 * @date: 2012/06/10
 * @mdfy: 2012/06/10
 * @Ver:  1.0
 *
 */

#ifndef _WUBI_CODE_H
#define _WUBI_CODE_H

#include <string>

class CWubiCode
{
public:
	CWubiCode(void);
	~CWubiCode(void);

	static std::string			Find(std::wstring ch);			//查找一个汉字的五笔编码
};

#endif