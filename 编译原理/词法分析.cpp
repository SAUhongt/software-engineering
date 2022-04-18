#include<stdio.h>
#include<stdlib.h>
#include<string.h>
typedef struct node {
	char str[100];		//需要处理的原始代码
	int* n;				//种别码
	char (*flag)[15];	//符号
	int num;			//识别的符号个数
	int size;			//空间开辟的大小
	char def[36][15];	//存放定义表
}Code;
void malloc(Code& code) {
	if (code.size - code.num <= 0) {
		code.n = (int*)realloc(code.n, sizeof(int) * (code.size + 10));
		code.flag = (char(*)[15])realloc(code.flag, sizeof(char*) * (code.size + 10) * 10 * 15);
		code.num = code.size;
		code.size += 10;
	}
}
//结构体初始化
void init(Code& code,char*str) {
	code.num = 0;
	code.size = 10;
	code.n = (int*)malloc(sizeof(int) * code.size);
	code.flag = (char(*)[15])malloc(sizeof(char*) * code.size*15);
	strcpy(code.str, str);
	//FILE* fp = fopen("定义表.txt", "r");
	/*FILE* fp = fopen("lab.txt", "r");
	if (!fp)
		exit(0);
	int m;
	char s[15];
	for (int i = 0; i < 36; i++) {
		fscanf(fp, "%d %s", &m, s);
		strcpy(code.def[m], s);
	}
	fclose(fp);*/
	strcpy(code.def[0], "无定义");
	strcpy(code.def[1], "标识符");
	strcpy(code.def[2], "整数");
	strcpy(code.def[3], "void");
	strcpy(code.def[4], "int");
	strcpy(code.def[5], "float");
	strcpy(code.def[6], "char");
	strcpy(code.def[7], "if");
	strcpy(code.def[8], "else");
	strcpy(code.def[9], "while");
	strcpy(code.def[10], "do");
	strcpy(code.def[11], "for");
	strcpy(code.def[12], "return");
	strcpy(code.def[13], "main");
	strcpy(code.def[14], "+");
	strcpy(code.def[15], "-");
	strcpy(code.def[16], "*");
	strcpy(code.def[17], "/");
	strcpy(code.def[18], "%");
	strcpy(code.def[19], "<");
	strcpy(code.def[20], "<=");
	strcpy(code.def[21], ">");
	strcpy(code.def[22], ">=");
	strcpy(code.def[23], "==");
	strcpy(code.def[24], "!=");
	strcpy(code.def[25], "&&");
	strcpy(code.def[26], "||");
	strcpy(code.def[27], "=");
	strcpy(code.def[28], "(");
	strcpy(code.def[29], ")");
	strcpy(code.def[30], "[");
	strcpy(code.def[31], "]");
	strcpy(code.def[32], "{");
	strcpy(code.def[33], "}");
	strcpy(code.def[34], ";");
	strcpy(code.def[35], ",");
}
void add(Code& code, int n, const char* flag) {
	malloc(code);
	code.n[code.num] = n;
	strcpy(code.flag[code.num], flag);
	code.num++;
}
//去除首位空格
void sta(char* str, int& star) {
	for (int i = 0; i < strlen(str); i++) {
		if (str[i] != ' ') {
			return;
		}
		star++;
	}
}
//获取str前面长度为l的字符串
char* copystr(char* str, int l) {
	char* s1 = (char*)malloc(sizeof(char) * (l + 1));
	for (int i = 0; i < l; i++) {
		s1[i] = str[i];
	}
	s1[l] = '\0';
	return s1;
}
//检索定义表并识别字符串
int find(Code code, int& star) {
	if (star <= strlen(code.str) - 2 && code.str[star] == '/' && code.str[star + 1] == '/') {
		return -2;
	}
	for (int i = 0; i < 36; i++) {
		if (code.str[star] == code.def[i][0]) {
			int s=strlen(code.def[i]);
			if (strcmp(code.def[i], copystr(code.str + star, strlen(code.def[i]))) == 0) {
				if (i == 27 && code.str[star + 1] == '=') {
					i++;
				}
				if (i == 19 && code.str[star + 1] == '=') {
					i++;
				}
				if (i == 21 && code.str[star + 1] == '=') {
					i++;
				}
				/*if (i == 23 && code.str[star + 1] == '*') {
					i++;
				}*/
				if (i == 15 && star != 0) {
					int a[3] = { 27, 19, 21 };
					for (int j = 0; j < 3; j++) {
						if (code.str[star - 1] == code.def[a[j]][0]) {
							return -1;
						}
					}
				}
				star += strlen(code.def[i]);
				return i;
			}
		}
	}
	return -1;
}
//运算符、';'
int findca(Code code, int n, int& star) {
	//标识符
	int flag = 0;
	if (n == 1) {
		for (int i = star; i < strlen(code.str); i++) {
			int a[14] = { 14,15,16,17,18,19,21,27,28,29,30,31,34,35 };
			for (int j = 0; j < 14; j++) {
				if (code.str[i] == code.def[a[j]][0] || code.str[i] == ' ') {
					star = i;
					return flag;
				}
			}
		}
	}
	//整数
	else {
		for (int i = star; i < strlen(code.str); i++) {
			//int a[11] = { 15,19,20,21,22,23,25,26,28,29,31 };
			for (int j = 0; j < 11; j++) {
				if (code.str[i] == code.def[34][0] || code.str[i] == ' ') {
					star = i;
					return flag;
				}
			}
			if (code.str[i] < 48 || code.str[i]>57) {
				flag = -1;
			}
		}
	}
	return flag;
}
int advance(Code& code) {
	int i = 0, j = 0, flag = 1;
	while (1) {
		sta(code.str + i, i);
		j = i;
		if (code.str[i] == '\0') {
			return flag;
		}
		int n = find(code, j);
		if (n == -2) {
			strcpy(code.str, "\0");
			return flag;
		}
		if (n != -1) {
			add(code, n, code.def[n]);
		}
		else {
			if ((code.str[i] >= 'a' && code.str[i] <= 'z') || (code.str[i] >= 'A' && code.str[i] <= 'Z') || code.str[i] == '_' || code.str[i] == '$') {
				n = 1;
			}
			else {
				n = 2;
			}
			if (findca(code,n, j) == -1) {
				flag = -1;
				add(code, 1, "LexicalError");
			}
			else {
				add(code, n, copystr(code.str + i, j - i));
			}
			
		}
		i = j;
	}
	return flag;
}
void print(Code code) {
	if (strcmp(code.str, "\0") == 0) {
		return;
	}
	for (int i = 0; i < code.num; i++) {
		//printf("<%d,", code.n[i]);
		if (strcmp(code.flag[i], "LexicalError") == 0) {
			printf("LexicalError,");
		}
		else if (code.n[i] == 1 || code.n[i] == 2)
			printf("<%d,%s>,", code.n[i], code.flag + i);
		else
			printf("<%d,->,", code.n[i]);
	}
	printf("\n");
}
int main() {
	int num = 0, size = 10;
	int flag[2][2];
	char(*str)[100];
	str = (char(*)[100])malloc(sizeof(char*) * (size * 100));
	while (1) {
		if (num >= size) {
			str = (char(*)[100])realloc(str, sizeof(char*) * (size + 10) * 100);
			num = size;
			size += 10;
		}
		gets_s(str[num]);
		int len = strlen(str[num]);
		//多行注释的去除
		for (int i = 0; i < len - 1; i++) {
			if (str[num][i] == '/' && str[num][i + 1] == '*') {
				flag[0][0] = num;
				flag[0][1] = i;
			}
			if (str[num][i] == '*' && str[num][i + 1] == '/') {
				flag[1][0] = num;
				flag[1][1] = i + 1;
				if (flag[0][0] == flag[1][0]) {
					char* s = copystr(str[flag[0][0]], flag[0][1]);
					strcat(s, str[flag[0][0]] + flag[1][1] + 1);
					strcpy(str[flag[0][0]], s);
				}
				else {
					for (int j = flag[0][0]; j <= flag[1][0]; j++) {
						if (j == flag[0][0]) {
							strcpy(str[j], copystr(str[j], flag[0][1]));
						}
						else if (j == flag[1][0]) {
							strcpy(str[j], str[j] + flag[1][1] + 1);
						}
						else {
							strcpy(str[j], "\0");
						}
					}
				}
			}
		}
		if (strcmp(str[num], "#") == 0) {
			break;
		}
		num++;
	}
	int error[100], errn = 0, Null = 0;
	for (int i = 0; i < num; i++) {
		Code code;
		if (strcmp(str[i], "\0") == 0) {
			Null++;
			continue;
		}
		init(code, str[i]);
		if (advance(code) == -1) {
			error[errn++] = i + 1 - Null;
		}
		print(code);
	}
	if (errn != 0) {
		printf("LexicalError(s) on line(s) ");
		for (int i = 0; i < errn; i++) {
			printf("%d,", error[i]);
		}
	}
	//gets_s(str);
	//init(code, str);
	//advance(code);
	//print(code);
	return 0;
}
