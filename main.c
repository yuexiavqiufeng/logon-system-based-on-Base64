#include <stdio.h>
#include <windows.h>
#include <string.h>
#include <math.h>
#include "base64.h"
#define filename_size 20
#define Account_size 20
#define Password_size 20

//文件名字符串定义函数
int filename_strcat(char *account, char *file_name)
{

	strcat(file_name, "file_");
	if (strlen(account) > filename_size - 9)
	{
		return -1;
	}
	strcat(file_name, account);
	strcat(file_name, ".dat");

	return 0;
}

// struct account_information
// {
// 	char id[20];
// 	char Password[20];
// };

int main()
{
	int i = 0;
	int j = 0;
	int operation;

	while (1)
	{

		printf("hello!\nregister:1\nSign in:2\n");
		operation = getchar() - 48;
		/*****************************************************************************************/
		/**************************************注册程序*******************************************/
		/*****************************************************************************************/

		switch (operation)
		{
		register_again:
		case 1: //执行注册操作
		{
			system("cls");
			char Filename[filename_size] = {""};
			char Account[Account_size] = {""};
			char Password[Password_size] = {""};
			char *Password_base64_encode = NULL;

			memset(Filename, 0, filename_size);

			//输入注册用账号密码
			printf("请输入账号(20位以内):\n");
			scanf("%s", Account);

			//检测输入是否合规
			if (filename_strcat(Account, Filename) < 0)
			{
				printf("超出账号字符数最大限");
				break;
			}

			FILE *p1 = fopen(Filename, "r"); //打开对应文件
			//检测是否已经注册
			if (p1 == NULL) //未注册现在进行注册
			{
				printf("账号未被注册\n请输入你的密码(20位以内):\n");

				scanf("%s", Password);
				Password_base64_encode = base64_encode(Password, strlen(Password));
				FILE *p1 = fopen(Filename, "w");
				/*****************注册操作*****************/
				fprintf(p1, "%s\n%s", Account, Password_base64_encode);
				printf("注册成功!!");
				fclose(p1); //释放指针
			}
			else
			{
				printf("账号已被注册!\n");
				fclose(p1); //释放指针
				goto register_again;
			}
			Sleep(3000); //挂起3s
			system("cls");
			break;
		}

			/*****************************************************************************************/
			/**************************************登陆程序*******************************************/
			/*****************************************************************************************/

		sign_in_again:
		case 2:
		{
			char Filename[filename_size] = {""};
			char Account[Account_size] = {""};
			char Password[Password_size] = {""};
			char *Password_base64_encode = NULL;
			char Password_base64_encode_cmp[Password_size * 2] = {""};

			printf("请输入你的账号:");
			scanf("%s", Account);

			//检测输入是否合规
			if (filename_strcat(Account, Filename) < 0)
			{
				printf("超出账号字符数最大限");
				break;
			}

			FILE *p1 = fopen(Filename, "r"); //打开对应文件
			//检测账号是否存在
			if (p1 == NULL) //账号不存在
			{
				printf("账号不存在!\n");
				fclose(p1); //释放指针
				goto sign_in_again;
			}
			else
			{
				FILE *p1 = fopen(Filename, "r");
				fscanf(p1, "%s\n%s", Account, Password_base64_encode_cmp);

				printf("请输入你的密码:");
				scanf("%s", Password);

				Password_base64_encode = base64_encode(Password, strlen(Password));
				

				/*****************验证密码*****************/

				if (strcmp(Password_base64_encode, Password_base64_encode_cmp) == 0)
				{
					//密码正确
					printf("登陆成功！\n");
					Sleep(1000); //挂起1s
					printf("登陆成功要干嘛，我也不知道\n");
					Sleep(1000); //挂起1s
					printf("那就这样吧\n");
				}
				else
				{
					//密码错误
					printf("密码错误！");
					Sleep(3000); //挂起3s
					system("cls");
					goto sign_in_again;
				}
			}
			Sleep(3000); //挂起3s
			system("cls");
			break;
		}
		default:
		{
			printf("Invalid input");
			Sleep(3000); //挂起3s
			system("cls");
			break;
		}
		}

		printf("exit?\n输入Y键退出,按任意键重启\n:");

		if ('Y' == getchar() || 'y' == getchar())
		{
			return 0;
		}
		else
		{
			system("cls");
		}
	}

	return 0;
}
