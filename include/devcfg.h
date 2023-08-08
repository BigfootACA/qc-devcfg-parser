//
// Created by bigfoot on 2023/7/22.
//

#ifndef _LOADER_H
#define _LOADER_H
#ifdef __cplusplus
extern "C"{
#endif
#include<stdio.h>
#include<stdbool.h>
#include<string.h>
#include<elf.h>
#include"defines.h"
typedef struct devcfg devcfg;
typedef struct devcfg_prop devcfg_prop;
typedef struct devcfg_device devcfg_device;
typedef struct devcfg_target devcfg_target;
typedef struct devcfg_section devcfg_section;
typedef union devcfg_num_val devcfg_num_val;
typedef union devcfg_num_array_val devcfg_num_array_val;
typedef struct devcfg_struct_item devcfg_struct_item;
typedef struct devcfg_struct_match devcfg_struct_match;
typedef struct devcfg_struct devcfg_struct;
#ifndef __cplusplus
typedef enum devcfg_struct_val_type devcfg_struct_val_type;
typedef enum devcfg_prop_type devcfg_prop_type;
#endif
enum devcfg_prop_type{
	DEVCFG_PROP_UNKNOWN          = 0x00000000,
	DEVCFG_PROP_STRING_PTR       = 0x00000011,
	DEVCFG_PROP_UINT32           = 0x00000002,
	DEVCFG_PROP_BYTE_ARRAY       = 0x00000008,
	DEVCFG_PROP_BYTE_ARRAY_PTR   = 0x00000018,
	DEVCFG_PROP_UINT32_ARRAY_PTR = 0x00000014,
	DEVCFG_PROP_STRUCT_PTR       = 0x00000012,
	DEVCFG_PROP_END              = 0xFF00FF00,
};
union devcfg_num_val{
	uintmax_t val;
	union{
		bool boolean;
		char c8;
		unsigned char uc8;
		int8_t int8;
		int16_t int16;
		int32_t int32;
		int64_t int64;
		intptr_t intptr;
		intmax_t intmax;
		uint8_t uint8;
		uint16_t uint16;
		uint32_t uint32;
		uint64_t uint64;
		uintptr_t uintptr;
		uintmax_t uintmax;
	}number;
	union{
		bool*boolean;
		char*c8;
		unsigned char*uc8;
		int8_t*int8;
		int16_t*int16;
		int32_t*int32;
		int64_t*int64;
		intptr_t*intptr;
		intmax_t*intmax;
		uint8_t*uint8;
		uint16_t*uint16;
		uint32_t*uint32;
		uint64_t*uint64;
		uintptr_t*uintptr;
		uintmax_t*uintmax;
	}array;
};
enum devcfg_struct_val_type{
	TYPE_NONE,
	TYPE_VOID,
	TYPE_BOOL,
	TYPE_INTN,TYPE_UINTN,
	TYPE_INT8,TYPE_UINT8,
	TYPE_INT16,TYPE_UINT16,
	TYPE_INT32,TYPE_UINT32,
	TYPE_INT64,TYPE_UINT64,
	TYPE_CHAR,
	TYPE_STRUCT,
};
struct devcfg_struct_item{
	char*name;
	devcfg_struct_val_type type;
	bool pointer;
	bool array;
	bool ptr_field;
	size_t offset;
	size_t size;
	size_t field;
	devcfg_struct*use_struct;
};
struct devcfg_struct{
	char*name;
	size_t size;
	devcfg_struct_item*items;
};
#ifdef __cplusplus
#define devcfg_prop_type enum devcfg_prop_type
#define devcfg_struct_val_type enum devcfg_struct_val_type
#endif
extern devcfg*devcfg_new(void);
extern int devcfg_load(devcfg*cfg);
extern int devcfg_dump(devcfg*cfg,int prefix);
extern int devcfg_section_dump(devcfg_section*section,int prefix);
extern bool devcfg_is_loaded(devcfg*devcfg);
extern int devcfg_load_mem(devcfg*cfg,void*elf,size_t len,bool alloc);
extern int devcfg_load_file(devcfg*cfg,FILE*f);
extern int devcfg_load_path(devcfg*cfg,const char*path);
extern devcfg*devcfg_section_get_devcfg(devcfg_section*section);
extern devcfg*devcfg_target_get_devcfg(devcfg_target*target);
extern devcfg*devcfg_device_get_devcfg(devcfg_device*device);
extern devcfg*devcfg_prop_get_devcfg(devcfg_prop*prop);
extern devcfg_section*devcfg_target_get_section(devcfg_target*target);
extern devcfg_section*devcfg_device_get_section(devcfg_device*device);
extern devcfg_section*devcfg_prop_get_section(devcfg_prop*prop);
extern devcfg_target*devcfg_device_get_target(devcfg_device*device);
extern devcfg_target*devcfg_prop_get_target(devcfg_prop*prop);
extern devcfg_device*devcfg_prop_get_device(devcfg_prop*prop);
extern int32_t devcfg_section_get_id(devcfg_section*section);
extern int64_t devcfg_target_get_soc(devcfg_target*target);
extern const char*devcfg_device_get_name(devcfg_device*device);
extern const char*devcfg_prop_get_name(devcfg_prop*prop);
extern devcfg_prop_type devcfg_prop_get_type(devcfg_prop*prop);
extern const char*devcfg_prop_get_type_readable(devcfg_prop*prop,const char*def);
extern const char*devcfg_prop_get_type_symbol(devcfg_prop*prop,const char*def);
extern ssize_t devcfg_prop_get_value_length(devcfg_prop*prop);
extern bool devcfg_prop_get_value_int8(devcfg_prop*prop,int8_t*val);
extern bool devcfg_prop_get_value_int16(devcfg_prop*prop,int16_t*val);
extern bool devcfg_prop_get_value_int32(devcfg_prop*prop,int32_t*val);
extern bool devcfg_prop_get_value_int64(devcfg_prop*prop,int64_t*val);
extern bool devcfg_prop_get_value_uint8(devcfg_prop*prop,uint8_t*val);
extern bool devcfg_prop_get_value_uint16(devcfg_prop*prop,uint16_t*val);
extern bool devcfg_prop_get_value_uint32(devcfg_prop*prop,uint32_t*val);
extern bool devcfg_prop_get_value_uint64(devcfg_prop*prop,uint64_t*val);
extern int8_t*devcfg_prop_get_value_int8_array(devcfg_prop*prop,size_t*count);
extern int16_t*devcfg_prop_get_value_int16_array(devcfg_prop*prop,size_t*count);
extern int32_t*devcfg_prop_get_value_int32_array(devcfg_prop*prop,size_t*count);
extern int64_t*devcfg_prop_get_value_int64_array(devcfg_prop*prop,size_t*count);
extern uint8_t*devcfg_prop_get_value_uint8_array(devcfg_prop*prop,size_t*count);
extern uint16_t*devcfg_prop_get_value_uint16_array(devcfg_prop*prop,size_t*count);
extern uint32_t*devcfg_prop_get_value_uint32_array(devcfg_prop*prop,size_t*count);
extern uint64_t*devcfg_prop_get_value_uint64_array(devcfg_prop*prop,size_t*count);
extern char*devcfg_prop_get_value_string(devcfg_prop*prop);
extern const char*devcfg_prop_get_value_string_buf(devcfg_prop*prop,char*buff,size_t size);
extern void*devcfg_prop_get_value_bytes(devcfg_prop*prop,size_t*len,size_t require);
extern ssize_t devcfg_prop_get_value_bytes_buf(devcfg_prop*prop,void*buffer,size_t len,size_t require);
extern uintptr_t devcfg_prop_get_value_address(devcfg_prop*prop);
extern ssize_t devcfg_prop_get_value_file_offset(devcfg_prop*prop,size_t*avail);
extern devcfg_section*devcfg_get_section_by_id(devcfg*cfg,uint16_t id);
extern int32_t*devcfg_list_section_by_id(devcfg*cfg,size_t*cnt);
extern devcfg_section**devcfg_get_sections(devcfg*cfg);
extern devcfg_target**devcfg_section_get_targets(devcfg_section*section);
extern devcfg_target*devcfg_section_get_target_by_id(devcfg_section*section,uint32_t id);
extern devcfg_target*devcfg_section_get_target_by_soc(devcfg_section*section,uint32_t soc);
extern devcfg_device**devcfg_target_get_devices(devcfg_target*target);
extern devcfg_device*devcfg_target_get_device_by_name(devcfg_target*target,const char*name);
extern devcfg_device*devcfg_section_get_device_by_name(devcfg_section*section,const char*name);
extern devcfg_device*devcfg_get_device_by_name(devcfg*cfg,const char*name);
extern devcfg_prop**devcfg_device_get_props(devcfg_device*device);
extern devcfg_prop*devcfg_device_get_prop_by_name(devcfg_device*dev,const char*name);
extern ssize_t devcfg_get_file_offset(devcfg*cfg,uintptr_t addr,size_t*available);
extern void*devcfg_read(devcfg*cfg,uintptr_t addr,void*buffer,size_t len,bool str);
extern int devcfg_struct_match_dev_name(devcfg_struct_match*match,const char*dev_name);
extern int devcfg_struct_match_prop_name(devcfg_struct_match*match,const char*prop_name);
extern devcfg_struct*devcfg_struct_match_get_type(devcfg_struct_match*match);
extern devcfg_struct*devcfg_prop_find_struct(devcfg_prop*prop);
extern devcfg_struct_match*devcfg_prop_find_struct_match(devcfg_prop*prop);
extern devcfg_struct_match*devcfg_struct_find_match(const char*dev_name,const char*prop_name);
extern devcfg_struct*devcfg_struct_find(const char*dev_name,const char*prop_name);
extern ssize_t devcfg_prop_guess_struct_size(devcfg_prop*prop);
extern ssize_t devcfg_struct_get_size(devcfg_struct_match*st,devcfg_prop*prop,uintptr_t ptr);
#define DECL_FUNC(_type)\
	extern int _type##_refcnt(_type*var);\
	extern _type*_type##_ref(_type*var);\
	extern void _type##_unref(_type*var);\
	extern bool _type##_check(_type*var);
DECL_FUNC(devcfg)
DECL_FUNC(devcfg_prop)
DECL_FUNC(devcfg_device)
DECL_FUNC(devcfg_target)
DECL_FUNC(devcfg_section)
header_func int32_t devcfg_prop_get_section_id(devcfg_prop*prop){
	return devcfg_section_get_id(devcfg_prop_get_section(prop));
}
header_func int64_t devcfg_prop_get_target_soc(devcfg_prop*prop){
	return devcfg_target_get_soc(devcfg_prop_get_target(prop));
}
header_func const char*devcfg_prop_get_device_name(devcfg_prop*prop){
	return devcfg_device_get_name(devcfg_prop_get_device(prop));
}
header_func bool devcfg_prop_get_value_number(
	devcfg_prop*prop,bool sign,uint8_t base,devcfg_num_val*val
){
	if(!val)return false;
	val->val=0;
	if(sign)switch(base){
		case 8:return devcfg_prop_get_value_int8(prop,&val->number.int8);
		case 16:return devcfg_prop_get_value_int16(prop,&val->number.int16);
		case 32:return devcfg_prop_get_value_int32(prop,&val->number.int32);
		case 64:return devcfg_prop_get_value_int64(prop,&val->number.int64);
	}else switch(base){
		case 8:return devcfg_prop_get_value_uint8(prop,&val->number.uint8);
		case 16:return devcfg_prop_get_value_uint16(prop,&val->number.uint16);
		case 32:return devcfg_prop_get_value_uint32(prop,&val->number.uint32);
		case 64:return devcfg_prop_get_value_uint64(prop,&val->number.uint64);
	}
	return false;
}
header_func devcfg_num_val devcfg_prop_get_value_number_array(
	devcfg_prop*prop,bool sign,uint8_t base,size_t*count
){
	devcfg_num_val val;
	val.val=0;
	if(sign)switch(base){
		case 8:val.array.int8=devcfg_prop_get_value_int8_array(prop,count);break;
		case 16:val.array.int16=devcfg_prop_get_value_int16_array(prop,count);break;
		case 32:val.array.int32=devcfg_prop_get_value_int32_array(prop,count);break;
		case 64:val.array.int64=devcfg_prop_get_value_int64_array(prop,count);break;
	}else switch(base){
		case 8:val.array.uint8=devcfg_prop_get_value_uint8_array(prop,count);break;
		case 16:val.array.uint16=devcfg_prop_get_value_uint16_array(prop,count);break;
		case 32:val.array.uint32=devcfg_prop_get_value_uint32_array(prop,count);break;
		case 64:val.array.uint64=devcfg_prop_get_value_uint64_array(prop,count);break;
	}
	return val;
}
header_func int devcfg_device_cmp_name(devcfg_device*device,const char*str){
	const char*name;
	if(!devcfg_device_check(device)||!str)return -1;
	if(!(name=devcfg_device_get_name(device)))return -1;
	return strcmp(name,str);
}

header_func int devcfg_device_icmp_name(devcfg_device*device,const char*str){
	const char*name;
	if(!devcfg_device_check(device)||!str)return -1;
	if(!(name=devcfg_device_get_name(device)))return -1;
	return strcasecmp(name,str);
}
header_func int devcfg_prop_cmp_name(devcfg_prop*prop,const char*str){
	const char*name;
	if(!devcfg_prop_check(prop)||!str)return -1;
	if(!(name=devcfg_prop_get_name(prop)))return -1;
	return strcmp(name,str);
}
header_func int devcfg_prop_icmp_name(devcfg_prop*prop,const char*str){
	const char*name;
	if(!devcfg_prop_check(prop)||!str)return -1;
	if(!(name=devcfg_prop_get_name(prop)))return -1;
	return strcasecmp(name,str);
}
#undef DECL_FUNC
#ifdef __cplusplus
}
#endif
#endif
