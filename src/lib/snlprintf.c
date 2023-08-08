#include"compatible.h"
#include<stdarg.h>
#include<stdio.h>

weak_func ssize_t snlprintf(char*buf,size_t count,const char*fmt,...){
	ssize_t r;
	va_list va;
	if(!buf||!fmt)return -1;
	va_start(va,fmt);
	r=vsnlprintf(buf,count,fmt,va);
	va_end(va);
	return r;
}
