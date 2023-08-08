//
// Created by bigfoot on 2023/7/31.
//

#ifndef COMPATIBLE_H
#define COMPATIBLE_H
#include<inttypes.h>
#include"defines.h"
extern void*memdup(void*mem,size_t len);
extern char*strndup(const char*str,size_t max);
extern size_t strlcat(char*d,const char*s,size_t n);
extern size_t strlcpy(char*d,const char*s,size_t n);
extern ssize_t snlprintf(char*buf,size_t count,const char*fmt,...);
extern ssize_t vsnlprintf(char*buf,size_t count,const char*fmt,va_list va);
#endif
