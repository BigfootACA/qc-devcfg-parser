//
// Created by bigfoot on 2023/7/25.
//

#include<stdarg.h>
#include<string.h>
#include"compatible.h"

void lib_printf(lib_log_level level,int pad,const char*str,...){
	int i;
	va_list va;
	char buff[8192];
	if(!str)return;
	memset(buff,0,sizeof(buff));
	for(i=0;i<pad;i++)
		strlcat(buff,"  ",sizeof(buff)-1);
	va_start(va,str);
	vsnlprintf(buff,sizeof(buff)-1,str,va);
	va_end(va);
	if(level>LEVEL_WARN){
		if(errno!=0)snlprintf(
			buff,sizeof(buff)-1,
			": %s",strerror(errno)
		);
		#ifdef __WIN32__
		DWORD err=GetLastError();
		if(err!=0)snlprintf(
			buff,sizeof(buff)-1,
			": 0x%x",err
		);
		#endif
		clear_error();
	}
	lib_print(level,buff);
}
