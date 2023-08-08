#include<errno.h>
#include<stdint.h>
#include<string.h>
#include<stddef.h>
#include<stdlib.h>
#include"compatible.h"

weak_func char*strndup(const char*str,size_t max){
	if(!str)return NULL;
	size_t len=strnlen(str,max);
	void*dup=malloc(len+1);
	if(!dup)EPRET(ENOMEM);
	memcpy(dup,str,len+1);
	return dup;
}
