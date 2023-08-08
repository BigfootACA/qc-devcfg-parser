#ifndef _DEFINES_H
#define _DEFINES_H
#ifdef __WIN32__
#include<windows.h>
#endif
#ifdef __cplusplus
extern "C"{
#endif
#include<errno.h>
#include<stddef.h>
#include<stdio.h>
#define MIN(a,b)((b)>(a)?(a):(b))
#define MAX(a,b)((b)<(a)?(a):(b))
#ifdef __WIN32__
#define weak_func
#else
#define weak_func __attribute__((__weak__))
#endif
#define maybe_unused __attribute__((unused))
#define header_func static inline maybe_unused

header_func void clear_error(){
	errno=0;
	#ifdef __WIN32__
	SetLastError(0);
	#endif
}
typedef enum lib_log_level{
	LEVEL_MIN,
	LEVEL_DEBUG,
	LEVEL_INFO,
	LEVEL_WARN,
	LEVEL_ERROR,
	LEVEL_MAX,
}lib_log_level;
extern void lib_print(lib_log_level level,const char*str);
extern void lib_printf(lib_log_level level,int prefix,const char*str,...);
extern __attribute__((format_arg(1))) const char*lib_gettext(const char*str);
#define dbg(str...)lib_printf(LEVEL_DEBUG,0,str)
#define info(str...)lib_printf(LEVEL_INFO,0,str)
#define warn(str...)lib_printf(LEVEL_WARN,0,str)
#define err(str...)lib_printf(LEVEL_ERROR,0,str)
#define dbg_prefix(prefix,str...)lib_printf(LEVEL_DEBUG,prefix,str)
#define info_prefix(prefix,str...)lib_printf(LEVEL_INFO,prefix,str)
#define warn_prefix(prefix,str...)lib_printf(LEVEL_WARN,prefix,str)
#define err_prefix(prefix,str...)lib_printf(LEVEL_ERROR,prefix,str)
#define go_err(str...) do{err(str);goto fail;}while(0)
#define r_err(r,str...) do{err(str);return r;}while(0)
#define r_perr(str...) do{err(str);return NULL;}while(0)
#define go_warn(str...) do{warn(str);goto fail;}while(0)
#define r_warn(r,str...) do{warn(str);return r;}while(0)
#define r_pwarn(str...) do{warn(str);return NULL;}while(0)
#define area_in_range(zone_base,zone_len,req_base,req_len) (\
	req_len<=zone_len&&req_base>=zone_base&&\
	req_base+req_len<=zone_base+zone_len\
)
#define ptr_in_range(zone_base,zone_len,ptr) (ptr>=zone_base&&ptr<=zone_base+zone_len)
#define _ lib_gettext
#define DECL_MIN(type) header_func type min_##type(type a,type b){return b>a?a:b;}
#define DECL_MAX(type) header_func type max_##type(type a,type b){return b<a?a:b;}
#define DECL_MINMAX(type) DECL_MIN(type) DECL_MAX(type)
#define EPRET(err) do{errno=(err);return NULL;}while(0)
#define ENUM(err) -(errno=(err))
#define ERET(err) do{return ENUM(err);}while(0)
DECL_MINMAX(int8_t)
DECL_MINMAX(int16_t)
DECL_MINMAX(int32_t)
DECL_MINMAX(int64_t)
DECL_MINMAX(uint8_t)
DECL_MINMAX(uint16_t)
DECL_MINMAX(uint32_t)
DECL_MINMAX(uint64_t)
DECL_MINMAX(size_t)
DECL_MINMAX(ssize_t)
DECL_MINMAX(int)
DECL_MINMAX(long)
typedef int runnable_t(void*);
typedef struct binary_data{
	size_t size;
	char data[];
}binary_data;

#define IMPORT_BIN(file,sym) asm(\
        ".section \".rodata\"\n"\
        ".balign 4\n"\
        ".global "#sym"\n"\
        #sym":\n"\
        ".quad "#sym"_data_end-"#sym"_data\n"\
        #sym"_data:\n"\
        ".incbin \"" file "\"\n"\
        #sym"_data_end:\n"\
        ".balign 4\n"\
);\
extern maybe_unused const binary_data sym
#ifdef __cplusplus
}
#endif
#endif
