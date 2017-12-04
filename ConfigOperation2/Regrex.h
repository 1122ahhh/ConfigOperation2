#pragma once

//
//  Regrex.h
//  读写配置文件
//
//

#ifndef _______Regrex_h
#define _______Regrex_h

#ifdef _cplusplus
extern "C" {
#endif
    // 打印符合查询条件的键值对<key, value>
	void RegrexSearch(void *handle);

	// 正则匹配
	void Match(char** keys, char** values, int count, char* f_key, int keyFlag, char* f_value, int valueFlag, int model);

	// 模式解析
	void ParseStr(char *str, char* key, int *keyFlag, char* value, int* valueFlag, int* model);

	// 获取输入
	void trimeSpaceGet(char *s);

#ifdef _cplusplus
}
#endif

#endif
