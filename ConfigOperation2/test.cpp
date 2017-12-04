#include<stdio.h>
#include<stdlib.h>
#include<string.h>

int main()
{
	char str[20];
	int i = 0, ret1, ret2;
	float fp = 0.0;

	gets_s(str);
	while (1) {
		if (strcmp(str, "exit") == 0) break;
		ret1 = sscanf_s(str, "%d", &i); // 将字符串转换成整数 i = 15
		ret2 = sscanf_s(str, "%f", &fp); // 将字符串转换成浮点数 fp = 15.455000
		printf("i = %d,  fp = %f\n", i, fp);
		printf("retInt = %d,  retFloat = %d\n", ret1, ret2);

		gets_s(str);
	}
	system("pause");
	return 0;
}