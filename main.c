#include <stdio.h>
#include <windows.h>
#include <string.h>
#include <math.h>
#include "base64.h"
#define filename_size 20
#define Account_size 20
#define Password_size 20

//�ļ����ַ������庯��
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
		/**************************************ע�����*******************************************/
		/*****************************************************************************************/

		switch (operation)
		{
		register_again:
		case 1: //ִ��ע�����
		{
			system("cls");
			char Filename[filename_size] = {""};
			char Account[Account_size] = {""};
			char Password[Password_size] = {""};
			char *Password_base64_encode = NULL;

			memset(Filename, 0, filename_size);

			//����ע�����˺�����
			printf("�������˺�(20λ����):\n");
			scanf("%s", Account);

			//��������Ƿ�Ϲ�
			if (filename_strcat(Account, Filename) < 0)
			{
				printf("�����˺��ַ��������");
				break;
			}

			FILE *p1 = fopen(Filename, "r"); //�򿪶�Ӧ�ļ�
			//����Ƿ��Ѿ�ע��
			if (p1 == NULL) //δע�����ڽ���ע��
			{
				printf("�˺�δ��ע��\n�������������(20λ����):\n");

				scanf("%s", Password);
				Password_base64_encode = base64_encode(Password, strlen(Password));
				FILE *p1 = fopen(Filename, "w");
				/*****************ע�����*****************/
				fprintf(p1, "%s\n%s", Account, Password_base64_encode);
				printf("ע��ɹ�!!");
				fclose(p1); //�ͷ�ָ��
			}
			else
			{
				printf("�˺��ѱ�ע��!\n");
				fclose(p1); //�ͷ�ָ��
				goto register_again;
			}
			Sleep(3000); //����3s
			system("cls");
			break;
		}

			/*****************************************************************************************/
			/**************************************��½����*******************************************/
			/*****************************************************************************************/

		sign_in_again:
		case 2:
		{
			char Filename[filename_size] = {""};
			char Account[Account_size] = {""};
			char Password[Password_size] = {""};
			char *Password_base64_encode = NULL;
			char Password_base64_encode_cmp[Password_size * 2] = {""};

			printf("����������˺�:");
			scanf("%s", Account);

			//��������Ƿ�Ϲ�
			if (filename_strcat(Account, Filename) < 0)
			{
				printf("�����˺��ַ��������");
				break;
			}

			FILE *p1 = fopen(Filename, "r"); //�򿪶�Ӧ�ļ�
			//����˺��Ƿ����
			if (p1 == NULL) //�˺Ų�����
			{
				printf("�˺Ų�����!\n");
				fclose(p1); //�ͷ�ָ��
				goto sign_in_again;
			}
			else
			{
				FILE *p1 = fopen(Filename, "r");
				fscanf(p1, "%s\n%s", Account, Password_base64_encode_cmp);

				printf("�������������:");
				scanf("%s", Password);

				Password_base64_encode = base64_encode(Password, strlen(Password));
				

				/*****************��֤����*****************/

				if (strcmp(Password_base64_encode, Password_base64_encode_cmp) == 0)
				{
					//������ȷ
					printf("��½�ɹ���\n");
					Sleep(1000); //����1s
					printf("��½�ɹ�Ҫ�����Ҳ��֪��\n");
					Sleep(1000); //����1s
					printf("�Ǿ�������\n");
				}
				else
				{
					//�������
					printf("�������");
					Sleep(3000); //����3s
					system("cls");
					goto sign_in_again;
				}
			}
			Sleep(3000); //����3s
			system("cls");
			break;
		}
		default:
		{
			printf("Invalid input");
			Sleep(3000); //����3s
			system("cls");
			break;
		}
		}

		printf("exit?\n����Y���˳�,�����������\n:");

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
