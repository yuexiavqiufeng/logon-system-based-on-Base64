#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "base64.h"
//base64��ѯ��
char *base64_encodetable = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

//base64����ѯ��
char base64_decodetable[128] = {
	-1, -1, -1, -1, -1, -1, -1, -1, //0-7
	-1, -1, -1, -1, -1, -1, -1, -1, //8-15
	-1, -1, -1, -1, -1, -1, -1, -1, //16-23
	-1, -1, -1, -1, -1, -1, -1, -1, //24-31
	-1, -1, -1, -1, -1, -1, -1, -1, //32-39
	-1, -1, -1, 62, -1, -1, -1, 63, //40-47, + /
	52, 53, 54, 55, 56, 57, 58, 59, //48-55, 0-7
	60, 61, -1, -1, -1,  0, -1, -1, //56-63, 8-9
	-1,  0,  1,  2,  3,  4,  5,  6, //64-71, A-G
	 7,  8,  9, 10, 11, 12, 13, 14, //72-79, H-O
	15, 16, 17, 18, 19, 20, 21, 22, //80-87, P-W
	23, 24, 25, -1, -1, -1, -1, -1, //88-95, X-Z
	-1, 26, 27, 28, 29, 30, 31, 32, //96-103, a-g
	33, 34, 35, 36, 37, 38, 39, 40, //104-111, h-o
	41, 42, 43, 44, 45, 46, 47, 48, //112-119, p-w
	49, 50, 51, -1, -1, -1, -1, -1, //120-127, x-z
};

//�����ַ�����
static char *num_table = "0123456789";

//char������ת��Ϊ2�����ַ�����ʽ, ��2->"10", 6->"110"
static char *char2binstr(char value)
{
	int i = 0;
	char binstr[9] = {};

	//ȡ��ÿһλ֮�󣬲��õ���Ӧ���ַ�ƴ�ӳ�һ���ַ���
	for (i = 0; i < 8; i++)
		binstr[7 - i] = num_table[(value & (0x1 << i)) >> i];

	return strdup(binstr);
}

//2�����ַ�����ʽ����ת��Ϊchar����������"10"->2, "110"->6
static char binstr2char(char *binstr)
{
	int i = 0;
	char value = 0;
	int length = 0;

	if (!binstr)
		return 0;

	length = strlen(binstr);

	//ȡ��2�����ַ�����ÿһ���ֽڣ�����ת����Ӧ�����֣�Ȼ��ԭ���ݡ�
	for (i = 0; i < length; i++)
		value += (binstr[length - 1 - i] - 0x30) << i;

	return value;
}

/*
 * Base64����˵��
 *  Base64����Ҫ���3��8λ�ֽڣ�3*8=24��ת��Ϊ4��6λ���ֽڣ�4*6=24����֮����6λ��ǰ�油����0���γ�8λһ���ֽڵ���ʽ��
 *  ���ʣ�µ��ַ�����3���ֽڣ�����0��䣬����ַ�ʹ��'='����˱����������ı�ĩβ���ܻ����1��2��'='��
 *  Base64�ƶ���һ��������Ա����ͳһת���������Ĵ�СΪ2^6=64����Ҳ��Base64���Ƶ�������
 */
static char *_base64_encode(char *src, int length)
{
	int i = 0;
	char dest[4] = "====";
	char binstr[24] = "000000000000000000000000";
	char tmp[6] = {};
	char *tmp1 = NULL;

	//ÿ�ֽ�8λ��ÿ��ת��3�ֽڣ�����3�ֽ�ʱ����λ��0
	//��ƴ��һ��������24�ֽ�2�����ַ���
	for (i = 0; i < length; i++) {
		tmp1 =  char2binstr(src[i]);
		strncpy(binstr + i * 8, tmp1, 8);
		free(tmp1);
	}

	//24�ֽڣ���4�飬ÿ��6�ֽڣ���ÿ��ת��Ϊ���ָ�ʽ,ÿ�����ֲ��õ���Ӧ�롣
	//3�������
	// 1. ������ʣ��1�����ݣ����������2��=�������2�Σ������������������ֽ��� + 1
	// 2. ������ʣ��2�����ݣ����������1��=�������3��
	// 3. ���û��ʣ���κ�����(ʣ��3������)����ʲô����Ҫ��,�����4��
	for (i = 0; i < length + 1; i++) {
		strncpy(tmp, binstr + 6 * i, 6);
		dest[i] = base64_encodetable[binstr2char(tmp)];
	}

	return strdup(dest);
}

char *base64_encode(char *src, int length)
{
	int i = 0;
	char *base64code = NULL;
	int codelength = 0;
	char *tmp = NULL;

	codelength = length / 3 * 4 + 4 + 1;
	base64code = malloc(codelength);
	memset(base64code, 0, codelength);

	for (i = 0; i < length / 3; i++) {
		tmp = _base64_encode(src + i * 3, 3);
		strcat(base64code, tmp);
		free(tmp);
	}

	if (length % 3) {
		tmp = _base64_encode(src + length - (length % 3), length % 3);
		strcat(base64code, tmp);
		free(tmp);
	}

	return base64code;
}

static char *_base64_decode(char *dest)
{
	int i = 0;
	char src[4] = {};
	char tmp[8] = {};

	for (i = 0; i < 3; i++) {
		strncpy(tmp, dest + 8 * i, 8);
		sprintf(src + i, "%c", binstr2char(tmp));
	}

	return strdup(src);
}

char *base64_decode(char *dest)
{
	int i = 0;
	int j = 0;
	char *src= NULL;
	int srclength = 0;
	char *binstr = NULL;
	int binstr_length = 0;
	char *tmp = NULL;

	binstr_length = strlen(dest) * 8 + 1;
	binstr = malloc(binstr_length);
	memset(binstr, 0, binstr_length);

	srclength = strlen(dest) / 4 * 3 + 1;
	src = malloc(srclength);
	memset(src, 0, srclength);

	for(i = 0; i < strlen(dest) / 4; i++)
		for (j = 0; j < 4; j++)
			strcat(binstr, char2binstr(base64_decodetable[dest[i * 4 + j]]) + 2);

	for (i = 0; i < strlen(dest) / 4; i++) {
		tmp = _base64_decode(binstr + i * 24);
		strcat(src, tmp);
		free(tmp);
	}

	return src;
}

