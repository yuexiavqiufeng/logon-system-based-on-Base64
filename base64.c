#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "base64.h"
//base64查询表
char *base64_encodetable = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

//base64反查询表
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

//数字字符串表
static char *num_table = "0123456789";

//char类型数转换为2进制字符串格式, 如2->"10", 6->"110"
static char *char2binstr(char value)
{
	int i = 0;
	char binstr[9] = {};

	//取到每一位之后，查表得到对应的字符拼接成一个字符串
	for (i = 0; i < 8; i++)
		binstr[7 - i] = num_table[(value & (0x1 << i)) >> i];

	return strdup(binstr);
}

//2进制字符串格式数据转换为char类型数，如"10"->2, "110"->6
static char binstr2char(char *binstr)
{
	int i = 0;
	char value = 0;
	int length = 0;

	if (!binstr)
		return 0;

	length = strlen(binstr);

	//取得2进制字符串的每一个字节，将其转化对应的数字，然后还原数据。
	for (i = 0; i < length; i++)
		value += (binstr[length - 1 - i] - 0x30) << i;

	return value;
}

/*
 * Base64编码说明
 *  Base64编码要求把3个8位字节（3*8=24）转化为4个6位的字节（4*6=24），之后在6位的前面补两个0，形成8位一个字节的形式。
 *  如果剩下的字符不足3个字节，则用0填充，输出字符使用'='，因此编码后输出的文本末尾可能会出现1或2个'='。
 *  Base64制定了一个编码表，以便进行统一转换。编码表的大小为2^6=64，这也是Base64名称的由来。
 */
static char *_base64_encode(char *src, int length)
{
	int i = 0;
	char dest[4] = "====";
	char binstr[24] = "000000000000000000000000";
	char tmp[6] = {};
	char *tmp1 = NULL;

	//每字节8位，每次转换3字节，不足3字节时，高位补0
	//先拼接一个完整的24字节2进制字符串
	for (i = 0; i < length; i++) {
		tmp1 =  char2binstr(src[i]);
		strncpy(binstr + i * 8, tmp1, 8);
		free(tmp1);
	}

	//24字节，分4组，每组6字节，将每组转换为数字格式,每个数字查表得到对应码。
	//3种情况：
	// 1. 如果最后剩下1个数据，编码结果后加2个=，即查表2次，查表次数正好是数据字节数 + 1
	// 2. 如果最后剩下2个数据，编码结果后加1个=，即查表3次
	// 3. 如果没有剩下任何数据(剩下3个数据)，就什么都不要加,即查表4次
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

