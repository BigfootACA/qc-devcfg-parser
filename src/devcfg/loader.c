//
// Created by bigfoot on 2023/7/22.
//

#include"internal.h"

static int devcfg_load_elf64(devcfg*cfg){
	if(cfg->len<=sizeof(Elf64_Ehdr))r_err(
		-1,_("ELF64 too short")
	);
	if(cfg->hdr64->e_version!=EV_CURRENT)r_err(
		-1,_("Unknown ELF64 version: 0x%02x"),
		cfg->hdr64->e_version
	);
	if(cfg->hdr64->e_phnum<=0)r_err(
		-1,_("No any ELF64 program header entry found")
	);
	if(cfg->hdr64->e_phentsize!=sizeof(Elf64_Phdr))r_err(
		-1,_("Invalid ELF64 program header entry size: %u bytes (need %zu bytes)"),
		cfg->hdr64->e_phentsize,sizeof(Elf64_Phdr)
	);
	size_t ph_start=cfg->hdr64->e_phoff;
	size_t ph_size=cfg->hdr64->e_phentsize*cfg->hdr64->e_phnum;
	size_t ph_end=ph_start+ph_size;
	if(ph_end>cfg->len)r_err(
		-1,_("ELF64 program header out of file (end %zu bytes, file %zu bytes)"),
		ph_end,cfg->len
	);
	return 0;
}

static int devcfg_load_elf32(devcfg*cfg){
	if(cfg->len<=sizeof(Elf32_Ehdr))r_err(
		-1,_("ELF32 too short")
	);
	if(cfg->hdr32->e_version!=EV_CURRENT)r_err(
		-1,_("Unknown ELF32 version: 0x%02x"),
		cfg->hdr32->e_version
	);
	if(cfg->hdr32->e_phnum<=0)r_err(
		-1,_("No any ELF32 program header entry found")
	);
	if(cfg->hdr32->e_phentsize!=sizeof(Elf32_Phdr))r_err(
		-1,_("Invalid ELF32 program header entry size: %u bytes (need %zu bytes)"),
		cfg->hdr32->e_phentsize,sizeof(Elf32_Phdr)
	);
	size_t ph_start=cfg->hdr32->e_phoff;
	size_t ph_size=cfg->hdr32->e_phentsize*cfg->hdr32->e_phnum;
	size_t ph_end=ph_start+ph_size;
	if(ph_end>cfg->len)r_err(
		-1,_("ELF32 program header out of file (end %zu bytes, file %zu bytes)"),
		ph_end,cfg->len
	);
	return 0;
}

static inline bool machine_check(uint8_t cls){
	#define CHECK_CLASS(_cls)\
		case ELFCLASS##_cls:\
			if(sizeof(void*)==sizeof(uint##_cls##_t))\
				return true;\
		break;
	switch(cls){
		CHECK_CLASS(32)
		CHECK_CLASS(64)
		default:;
	}
	#undef CHECK_CLASS
	return false;
}

int devcfg_load(devcfg*cfg){
	if(!devcfg_check(cfg))return -1;
	if(!cfg->elf||cfg->len<=0)return -1;
	if(cfg->len<=EI_NIDENT)
		r_err(-1,_("ELF too short"));
	if(memcmp(cfg->elf,ELFMAG,SELFMAG)!=0)
		r_err(-1,_("Bad ELF magic"));
	if(cfg->data[EI_VERSION]!=EV_CURRENT)r_err(
		-1,_("Unknown ELF version: 0x%02x"),
		cfg->data[EI_VERSION]
	);
	cfg->elf_class=cfg->data[EI_CLASS];
	if(!machine_check(cfg->elf_class))r_err(
		-1,_("Unsupported ELF class %u in %zu-bit cpu"),
		cfg->elf_class,sizeof(void*)*8
	);
	switch(cfg->data[EI_CLASS]){
		case ELFCLASS32:if(devcfg_load_elf32(cfg)<0)return -1;break;
		case ELFCLASS64:if(devcfg_load_elf64(cfg)<0)return -1;break;
		default:r_err(-1,_("Unknown ELF class: 0x%02x"),cfg->data[EI_CLASS]);
	}
	cfg->loaded=true;
	return 0;
}

int devcfg_load_mem(devcfg*cfg,void*elf,size_t len,bool alloc){
	if(!devcfg_check(cfg))return -1;
	if(!elf||len<=0)return -1;
	if(devcfg_is_loaded(cfg))
		r_err(-1,_("devcfg already loaded"));
	cfg->elf=elf,cfg->len=len,cfg->alloc=alloc;
	return devcfg_load(cfg);
}

int devcfg_load_file(devcfg*cfg,FILE*f){
	if(!devcfg_check(cfg))return -1;
	if(devcfg_is_loaded(cfg))
		r_err(-1,_("devcfg already loaded"));
	cfg->elf=NULL,cfg->len=0;
	if(io_read_file(
		f,DEVCFG_MAX,
		&cfg->elf,&cfg->len
	)<0)return -1;
	cfg->alloc=true;
	return devcfg_load(cfg);
}

int devcfg_load_path(devcfg*cfg,const char*path){
	if(!devcfg_check(cfg))return -1;
	if(devcfg_is_loaded(cfg))
		r_err(-1,_("devcfg already loaded"));
	cfg->elf=NULL,cfg->len=0;
	if(io_read_path(
		path,DEVCFG_MAX,
		&cfg->elf,&cfg->len
	)<0)return -1;
	cfg->alloc=true;
	return devcfg_load(cfg);
}
