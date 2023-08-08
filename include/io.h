#ifndef _IO_H
#define _IO_H
#ifdef __cplusplus
extern "C"{
#endif
#include<stdio.h>
extern int io_read_file(FILE*in,size_t max,void**data,size_t*size);
extern int io_read_path(const char*path,size_t max,void**data,size_t*size);
#ifdef __cplusplus
}
#endif
#endif
