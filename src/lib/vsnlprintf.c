#include"compatible.h"
#include<stdio.h>
#include<string.h>

weak_func ssize_t vsnlprintf(char*buf,size_t count,const char*fmt,va_list va){
	if(!buf||!fmt)return -1;
	size_t len=strlen(buf);
	if(len>=count)return 0;
	return vsnprintf(buf+len,count-len,fmt,va);
}
