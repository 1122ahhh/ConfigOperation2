#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Properties.h"
#include "Regrex.h"

#define KEY_SIZE        128 // key缓冲区大小
#define VALUE_SIZE      1024 // value缓冲区大小
#define REGREX_MODEL_NUM 6 //正则匹配符号数

char RegrexModel[REGREX_MODEL_NUM][5] = { "<","<=","==",">=",">",":" };

void RegrexSearch(void *handle) {

	printf("--- 匹配模式：{ %%str1%%,str1} {<, <=, ==, >=, >, ：} {%%str2%%, str2}  --\n");
	printf("--- 详细的正则查找模式详见：RegrexRule.docx \n");
	printf("--- 退出查找，请输入：back\n\n");
	printf("--- 请输入查找模式:");

	int keyFlag, valueFlag, keyscount, valuescount, model;
	char key[KEY_SIZE], value[VALUE_SIZE];
	char strBuffer[VALUE_SIZE + KEY_SIZE];
	char **keys = NULL, **values = NULL;

	getKeys(handle, &keys, &keyscount);//获取配置文件key列表
	getValues(handle, &values, &valuescount);//获取配置文件value列表

	trimeSpaceGet(strBuffer);//获取输入

	while (1) {
		if (strcmp(strBuffer, "back") == 0) break;
		keyFlag = 0, valueFlag = 0, model = -1;
		ParseStr(strBuffer, key, &keyFlag, value, &valueFlag, &model);
		Match(keys, values, keyscount, key, keyFlag, value, valueFlag, model);
		printf("--- 请输入查找模式:");
		trimeSpaceGet(strBuffer);//获取输入
	}

    // 释放内存
	free_keys(&keys, &keyscount);
    free_values(&values, &valuescount);

	printf("你已退出配置文件正则查找，返回主菜单！\n");

	return;
}

// 正则匹配
void Match(char** keys, char** values, int count, char* f_key, int keyFlag, char* f_value, int valueFlag, int model) {
	int k = 0, ret=0, type;

	
	for (int i = 0; i < count; i++) {
		ret = 0;
		if (keyFlag) {
			if (strstr(keys[i], f_key)) ret = 1;
		}
		else if (strcmp(keys[i], f_key)==0) ret = 1;
		if (ret) {
			if (valueFlag) {
				if (strstr(values[i], f_value) == NULL) ret = 0;
			}
			else 
			{ 
				switch (model) {
				case 0:if (!(strcmp(values[i], f_value) < 0)) { ret = 0; }
					   break;
				case 1:if (!(strcmp(values[i], f_value) <= 0)) { ret = 0; }
					   break;
				case 2:if (!(strcmp(values[i], f_value) == 0)) { ret = 0; }
					   break;
				case 3:if (!(strcmp(values[i], f_value) >= 0)) { ret = 0; }
					   break;
				case 4:if (!(strcmp(values[i], f_value) > 0)) { ret = 0; }
					   break;
				default: break;
				}
			}
		}
		if (ret) {
			printf("%s = %s\n", keys[i], values[i]); k++;
		}
  }
	if (k == 0) printf("没有符合条件的查询结果！\n\n");
	else printf("一共有 %d 个符合条件的查询结果\n\n", k);

	return;

}

void ParseStr(char *str, char* key, int *keyFlag, char* value, int* valueFlag, int* model) {
	int i = 0, j = 0, k = 0, m = 0;
	char Model[5];

	if (str[i] == ':') *keyFlag = 1;
	else if (str[i] == '%') {
		*keyFlag = 1; i++;
		while (str[i] != '%') key[j++] = str[i++];
		while (str[++i] == ' ');
	}
	else {
		while (str[i] != ' ') key[j++] = str[i++];
		while (str[i] == ' ') i++;
	}
	while (str[i]!='\0' && str[i] != ' ') Model[m++] = str[i++];
	while (str[i] == ' ') i++;
	if (str[i] == '%') { *valueFlag = 1; i++; }
	while (str[i] != '%' && str[i] != '\0') value[k++] = str[i++];
    key[j] = '\0'; value[k] = '\0'; Model[m] = '\0';
	if (strcmp(Model, ":") == 0 && k == 0) *valueFlag = 1;

	for (i = 0; i<REGREX_MODEL_NUM; i++)
		if (strcmp(Model, RegrexModel[i]) == 0) { *model = i; break; }

	return;

}

void trimeSpaceGet(char *s) {
	char c;
	int i = -1;
	s[0] = '\0';
	c = getchar();
	while (c == ' ') c = getchar();
	while (c != '\n') { s[++i] = c; c = getchar(); }
	while (s[i] == ' ') i--;
	s[i + 1] = '\0';
	
	return;
}
