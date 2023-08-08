//
// Created by bigfoot on 2023/7/24.
//

#ifndef DEVCFG_INTERNAL_H
#define DEVCFG_INTERNAL_H

#include<elf.h>
#include<string.h>
#include<stdlib.h>
#include<stddef.h>
#include<stdbool.h>
#include"defines.h"
#include"devcfg.h"
#include"str.h"
#include"io.h"

typedef union elf_phdr elf_phdr;
typedef struct devcfg_string_device devcfg_string_device;
typedef struct devcfg_struct_type devcfg_struct_type;
typedef struct devcfg_props_dir devcfg_props_dir;
typedef struct devcfg_props devcfg_props;
typedef struct devcfg_platform_info devcfg_platform_info;
typedef struct devcfg_platform_seg_info devcfg_platform_seg_info;
typedef struct devcfg_target_info devcfg_target_info;

union elf_phdr{
	void*ptr;
	Elf32_Phdr*p32;
	Elf64_Phdr*p64;
};

struct devcfg_string_device{
	const char*name;
	uint32_t hash;
	uint32_t offset;
	void*func_name;
	uint32_t collision_cnt;
	const uint32_t*collisions;
};

struct devcfg_struct_type{
	uint32_t size;
	const void*ptr;
};

struct devcfg_props_dir{
	uint32_t length;
	uint32_t name_offset;
	uint32_t string_offset;
	uint32_t byte_offset;
	uint32_t uint32_offset;
	uint32_t devices_cnt;
	uint32_t device_offset[1][2];
};

struct devcfg_props{
	const void*bin;
	const devcfg_struct_type*struct_ptr;
	const uint32_t device_cnt;
	const devcfg_string_device*devices;
};

struct devcfg_platform_info{
	uint16_t hw_version;
	uint16_t soc_number;
};

struct devcfg_platform_seg_info{
	uint32_t platform_id;
	devcfg_props*props;
};

struct devcfg_target_info{
	uint32_t soc_id;
	devcfg_props*props;
	devcfg_platform_seg_info*platform;
	uint32_t platform_type_count;
};

struct devcfg{
	int refcnt;
	uint8_t elf_class;
	bool loaded;
	bool alloc;
	union{
		void*elf;
		uint8_t*data;
		Elf32_Ehdr*hdr32;
		Elf64_Ehdr*hdr64;
	};
	size_t len;
};

struct devcfg_prop{
	int refcnt;
	devcfg*devcfg;
	devcfg_section*section;
	devcfg_target*target;
	devcfg_device*device;
	devcfg_prop_type type;
	uintptr_t ptr;
	char name[4096];
};

struct devcfg_device{
	int refcnt;
	devcfg*devcfg;
	devcfg_section*section;
	devcfg_target*target;
	char name[4096];
	devcfg_string_device dev;
};

struct devcfg_target{
	int refcnt;
	devcfg*devcfg;
	devcfg_section*section;
	devcfg_target_info tgt;
	devcfg_props_dir dir;
	devcfg_props props;
};

struct devcfg_section{
	int refcnt;
	devcfg*devcfg;
	uint16_t id;
	elf_phdr phdr;
};

#define DEVCFG_MAX 256*1024
#define DEBUG_REFCNT_START(_type,_obj) \
	int debug_refcnt_##_type##_##_obj##_start=_type##_refcnt(_obj)
#define DEBUG_REFCNT_END(_type,_obj)\
	int debug_refcnt_##_type##_##_obj##_end=_type##_refcnt(_obj);\
	if(debug_refcnt_##_type##_##_obj##_start!=debug_refcnt_##_type##_##_obj##_end)warn(\
		_("refcnt check failed on %p (%s): %d!=%d"),_obj,#_type,\
		debug_refcnt_##_type##_##_obj##_start,debug_refcnt_##_type##_##_obj##_end\
	)
#define DECL_REFCNT(_type,on_free) \
	int _type##_refcnt(_type*var){\
		if(!var)return -1;\
		return var->refcnt;\
	}\
	_type*_type##_ref(_type*var){\
		if(!var)return NULL;\
		var->refcnt++;\
		return var;\
	}\
	void _type##_unref(_type*var){\
		if(!var)return;\
		var->refcnt--;\
		if(var->refcnt>0)return;\
		on_free\
		memset(var,0,sizeof(_type));\
		free(var);\
	}
#define elf_prog_area_in_range(prog,req_base,req_len) \
	area_in_range((prog)->p_paddr,(prog)->p_memsz,req_base,req_len)
#define elf_prog_ptr_in_range(prog,ptr) \
	ptr_in_range((prog)->p_paddr,(prog)->p_memsz,ptr)
#define devcfg_check_loaded_ptr(cfg)do{\
	devcfg*_cfg=(cfg);\
	if(!devcfg_check(_cfg))return NULL;\
	if(!devcfg_is_loaded(_cfg))r_perr(_("No devcfg loaded"));\
}while(0)
#define devcfg_check_loaded_int(cfg,r)do{\
	devcfg*_cfg=(cfg);\
	if(!devcfg_check(_cfg))return r;\
	if(!devcfg_is_loaded(_cfg))r_err(r,_("No devcfg loaded"));\
}while(0)

extern bool devcfg_check_program(devcfg*cfg,size_t i,elf_phdr phdr,bool warn);
header_func bool devcfg_check_program32(devcfg*cfg,size_t i,Elf32_Phdr*phdr,bool warn){
	elf_phdr p={.p32=phdr};
	return devcfg_check_program(cfg,i,p,warn);
}
header_func bool devcfg_check_program64(devcfg*cfg,size_t i,Elf64_Phdr*phdr,bool warn){
	elf_phdr p={.p64=phdr};
	return devcfg_check_program(cfg,i,p,warn);
}
extern int devcfg_target_set_info(devcfg_target*target,devcfg_target_info*info);
extern int devcfg_device_set_dev(devcfg_device*device,devcfg_string_device*dev);
extern uintptr_t devcfg_section_get_base(devcfg_section*section);
extern devcfg_section*devcfg_section_new(devcfg*cfg);
extern devcfg_target*devcfg_target_new(devcfg_section*section);
extern devcfg_device*devcfg_device_new(devcfg_target*target);
extern devcfg_prop*devcfg_prop_new(devcfg_device*device);
#endif
