// ConfigOperation.cpp: 定义控制台应用程序的入口点。
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Properties.h"
#include "Regrex.h"

#define KEY_SIZE        128 // key缓冲区大小
#define VALUE_SIZE      1024 // value缓冲区大小


void printMenu();
void getKey(char* key);
void fWrite(void *handle);
void fRead(void *handle);
void SaveToXml(void *handle);

int main(int argc, const char * argv[])
{
	int choice;
	
	printMenu();
    printf("请输入选择：");
	scanf_s("%d", &choice);
	getchar();
	
	while (choice) {

		int ret;
		void *handle;
	    char filepath[128] ;

		printf("请输入配置文件名：");//获取文件路径
		gets_s(filepath, 128);

		// 初始化
		ret = init(filepath, &handle);
		if (ret != 0) {
			printf("env init error:%d\n", ret);
			return 0;
		}
		
		switch (choice) {
		case 1:fWrite(handle); break;
		case 2:fRead(handle); break;
		case 3:RegrexSearch(handle); break;
		case 4:SaveToXml(handle); break;
		default:break;
		}

		release(&handle);//释放资源

		printf("请输入选择：");
		scanf_s("%d", &choice);
		getchar();

		//char valuebuf[VALUE_SIZE];
		//// 测试获取配置项
		//ret = getValue(handle, "host", valuebuf);
		//if (ret == 0) {
		//	printf("value=%s\n", valuebuf);
		//}
		//else {
		//	printf("获取值host的值失败\n");
		//}

		//// 测试修改配置项
		//ret = setValue(handle, "version", "1.2.3");
		//if (ret == 0) {
		//	printf("修改成功!\n");
		//}
		//else {
		//	printf("修改失败\n");
		//}

		//// 测试添加配置项
		//ret = add(handle, "pool_connection_countxx", "2000");
		//if (ret == 0) {
		//	printf("添加成功!\n");
		//}
		//else {
		//	printf("添加失败\n");
		//}

		//// 测试删除配置项
		//ret = del(handle, "connectionMax");
		//if (ret == 0) {
		//	printf("删除成功!\n");
		//}
		//else {
		//	printf("删除失败\n");
		//}

		//// 测试获取所有配置项的key
		//char **keys = NULL;
		//int keyscount;
		//ret = getKeys(handle, &keys, &keyscount);
		//if (ret == 0) {
		//	printf("一共有%d个Key\n", keyscount);
		//	for (int i = 0; i<keyscount; i++) {
		//		printf("%s\n", keys[i]);
		//	}
		//}
		//// 释放内存
		//ret = free_keys(&keys, &keyscount);
		//if (ret == 0) {
		//	printf("keys释放内存成功!\n");
		//}

		//// 测试获取所有配置项的value
		//char **values = NULL;
		//int valuescount;
		//ret = getValues(handle, &values, &valuescount);
		//if (ret == 0) {
		//	printf("一共有%d个Value\n", valuescount);
		//	for (int i = 0; i < valuescount; i++) {
		//		printf("%s", values[i]);
		//	}
		//}
		//// 释放内存
		//ret = free_values(&values, &valuescount);
		//if (ret == 0) {
		//	printf("values释放内存成功!\n");
		//}

		//// 释放资源
		//ret = release(&handle);
		//if (ret != 0) {
		//	printf("env release error:%d\n", ret);
		//}
	}
	
	system("pause");
	return 0;
}

void SaveToXml(void *handle) {
	char xmlPath[100];
	printf("请输入要保存<key, value> xml格式的文件名：");
	gets_s(xmlPath, 100);

	if (Save2Xml(xmlPath, handle) == 0)
		printf("保存成功！！！\n\n");
	else
		printf("保存失败！！！\n\n");
	return;
}

void fRead(void *handle) {
	int ret1, ret2;
	// 测试获取所有配置项的key
	char **keys = NULL, **values = NULL;
	int keyscount, valuescount;

	ret1 = getKeys(handle, &keys, &keyscount);//获取配置文件key列表
	ret2 = getValues(handle, &values, &valuescount);//获取配置文件value列表

	if (ret1 == 0 && ret2==0 && keyscount==valuescount) {
		printf("一共有%d对<Key,Value>\n", keyscount);
		for (int i = 0; i<keyscount; i++) {
			printf("%s = %s\n", keys[i],values[i]);
		}
	}

	// 释放内存
	ret1 = free_keys(&keys, &keyscount);
    ret2 = free_values(&values, &valuescount);

	printf("你已退出配置文件读取，返回主菜单\n\n");
	return;
}

void fWrite(void *handle) {

	printf("/*****请输入要写入的数据,若key值存在，则value值会被覆盖，若不存在，则在文末追加，返回主菜单，请输入：back\n");
	printf(" *****如输入的value值为数组，则请以{}标识,，例如输入：{{,,,},{,,,}}\n");
	printf(" *****如输入的value值为数组，则请以[]标识,，例如输入：[[,,,],[,,,]]\n\n");

	int ret;
	char key[KEY_SIZE], value[VALUE_SIZE];
	while (1) {
		key[0] = '\0', value[0] = '\0';
		
		printf("请输入key=");//获取key值
		getKey(key);
		if (!strlen(key)) { printf("Error!! Key值不能为空\n"); continue; }
		if (strcmp(key, "back") == 0) break;
		
		printf("请输入value=");//获取value值
		gets_s(value, 1024);

		ret = add(handle, key, value);//若key存在文件中，则直接修改，否则添加到文件中
		if (ret == 0) printf("添加成功!\n");
		else printf("添加失败\n");
	}
	printf("你已退出配置文件写入，返回主菜单\n\n");
	return;
}

void getKey(char* key) {
	int i = 0;
	char c;
	c = getchar();
	while (c != '\n' && c == ' ') c = getchar();
	while (c != '\n' && c != ' ') {
		key[i] = c; i++; c = getchar();
	}
	key[i] = '\0';
	while (c != '\n') c = getchar();
	return;
}

void printMenu() {
	printf("================欢迎使用Meihong Zeng的配置文件操作系统===================\n");
	printf(" 1   Write          ----写入配置文件\n");
	printf(" 2   Read           ----读取配置文件，并以列表展示\n");
	printf(" 3   RegrexSearch   ----正则查找\n");
	printf(" 4   Modify ----查找key值，查找替换value\n");
	printf(" 0   Exit  ----退出程序\n");
	printf("=========================================================================\n\n");
}
