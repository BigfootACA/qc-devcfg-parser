#include<errno.h>
#include<string.h>
#include<stddef.h>
#include<stdlib.h>
#include<stdint.h>
#include"defines.h"

weak_func void*memdup(const void*mem,size_t len){
	if(!mem||len<=0)EPRET(EINVAL);
	void*dup=malloc(len);
	if(!dup)EPRET(ENOMEM);
	memcpy(dup,mem,len);
	return dup;
}
