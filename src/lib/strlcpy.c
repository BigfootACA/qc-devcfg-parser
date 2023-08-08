#include"compatible.h"
#include<string.h>

weak_func size_t strlcpy(char*d,const char*s,size_t n){
	if(!d||!s)return 0;
	char*d0=d;
	if(n--)strncpy(d,s,n);
	return d-d0+strlen(s);
}
