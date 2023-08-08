#include"compatible.h"
#include<string.h>

weak_func size_t strlcat(char*d,const char*s,size_t n){
	if(!d||!s)return 0;
	size_t l=strnlen(d,n);
	if(l==n)return l+strlen(s);
	return l+strlcpy(d+l,s,n-l);
}
