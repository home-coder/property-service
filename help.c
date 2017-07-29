#include <stdio.h>
#include <string.h>

/*
 * char *strtok(char *str, const char *delim);
 * strsep
 strtok 与 strsep 函数说明

 这几天处理字符串，突然遇到字符串分割问题，上网查了一些资料后，找到这两个函数，strtok与strsep函数。网上举的例子千篇一律，下面我根据函数的实现源码，记录一下使用说明，供大家讨论，欢迎大牛拍砖！PS:找个库函数源码的在线查询网站真不容易，先找到了这个http://sources.redhat.com/cgi-bin/cvsweb.cgi/libc/?cvsroot=glibc

 之后，发现了经常去找软件的oschina有源码库,真是踏破铁鞋无觅处，得来全不费工夫！

http://www.oschina.net/code/explore/glibc-2.9/string/strtok.c
复制代码

1 #include <stdio.h>
2 #include <string.h>
3 int main()
4 {
5     char token[] ="abdczxbcdefgh";
6     printf("%s\n",token);
7     char *tokenremain = token;
8     char *tok1 = strtok(tokenremain,"cde");
9     printf("tok1:%s\n",tok1);
10     tok1 = strtok(NULL,"cde");                                                                          
11     printf("tok1:%s\n",tok1);
12     return 0;
13 }

复制代码

[root@ test]# ./strtok 
abdczxbcdefgh
tok1:ab
tok1:zxb

总结:strtok内部记录上次调用字符串的位置，所以不支持多线程，可重入版本为strtok_r,有兴趣的可以研究一下。它适用于分割关键字在字符串之间是“单独”或是 “连续“在一起的情况。



http://www.oschina.net/code/explore/glibc-2.9/string/strsep.c
复制代码

1 #include <stdio.h>
2 #include <string.h>
3 
4 int main()
5 {
6     char token[] ="abdzxbcdefgh";
7     printf("%s\n",token);
8     char *tokenremain = token;
9     char *tok1 = strsep(&tokenremain,"cde");
10     printf("tok1:%s,token:%s\n",tok1,tokenremain);
11     tok1 = strsep(&tokenremain,"cde");                                                                  
12     printf("tok1:%s,token:%s\n",tok1,tokenremain);
13     return 0;
14 }

复制代码

[root@ test]# ./strsep 
abdzxbcdefgh
tok1:ab,token:zxbcdefgh
tok1:zxb,token:defgh

总结:strsep返回值为分割后的开始字符串，并将函数的第一个参数指针指向分割后的剩余字符串。它适用于分割关键字在两个字符串之间只严格出现一次的情况。

所以通过阅读函数实现源码，可以灵活运用这两个函数，为自己所用！

PS:因为函数内部会修改原字符串变量，所以传入的参数不能是不可变字符串（即文字常量区）。

如 char *tokenremain ="abcdefghij"//编译时为文字常量，不可修改。

strtok(tokenremain,"cde");

strsep(&tokenremain,"cde");

编译通过,运行时会报段错误。
	*/
int main()
{
	char mstr[]="Hello,I am sky2098,I liking programing!\n";
	char character='k' ;  //指定一个字符
	char *strtemp;
	char *str = mstr;

	while ((strtemp=strchr(str,character))) {
		if(strtemp!=NULL)
		{
			printf("%s\n",strtemp);
		}
		else
		{
			printf("can not find %s !\n",strtemp);
		}
		*strtemp++ = 0;
		str = strtemp;
	}


	char *p ="\nfdsl\n";
	if (p[0] == '\n') {
		printf("wow\n");
	}

	return 0;
}
#include <stdio.h>
#include <string.h>
int main()
{
	char token[] ="abdczxbcdefgh";
	printf("%s\n",token);
	char *tokenremain = token;
	char *tok1 = strtok(tokenremain,"cde");
	printf("tok1:%s\n",tok1);
	tok1 = strtok(NULL,"cde");                                                                          
	printf("tok1:%s\n",tok1);
	return 0;
}
