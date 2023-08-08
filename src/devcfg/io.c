//
// Created by bigfoot on 2023/7/24.
//

#include"internal.h"

bool devcfg_check_program(devcfg*cfg,size_t i,elf_phdr phdr,bool warn){
	if(!cfg||!phdr.ptr)return false;
	#define x_err(str...) do{if(warn)err(str);return false;}while(0)
	switch(cfg->elf_class){
		case ELFCLASS32:
			if(phdr.p32->p_type!=PT_LOAD)
				x_err(_("section %zu not loadable"),i);
			if((phdr.p32->p_flags&PF_R)==PF_R)
				x_err(_("section %zu not readable"),i);
			if(phdr.p32->p_paddr<=0x1000)
				x_err(_("section %zu address too small"),i);
		break;
		case ELFCLASS64:
			if(phdr.p64->p_type!=PT_LOAD)
				x_err(_("section %zu not loadable"),i);
			if((phdr.p64->p_flags&PF_R)!=PF_R)
				x_err(_("section %zu not readable"),i);
			if(phdr.p64->p_paddr<=0x1000)
				x_err(_("section %zu address too small"),i);
		break;
		default:return false;
	}
	#undef x_err
	return true;
}

ssize_t devcfg_get_file_offset(devcfg*cfg,uintptr_t addr,size_t*avail){
	size_t i,offset;
	if(avail)*avail=0;
	devcfg_check_loaded_int(cfg,-1);
	if(addr<=0)return -1;
	switch(cfg->elf_class){
		case ELFCLASS32:{
			Elf32_Phdr*phb=cfg->elf+cfg->hdr32->e_phoff;
			for(i=0;i<cfg->hdr32->e_phnum;i++)if(
				devcfg_check_program32(cfg,i,&phb[i],false)&&
				elf_prog_ptr_in_range(&phb[i],addr)
			){
				offset=addr-phb[i].p_paddr;
				if(avail)*avail=phb[i].p_memsz-offset;
				return (ssize_t)phb[i].p_offset+offset;
			}
		}break;
		case ELFCLASS64:{
			Elf64_Phdr*phb=cfg->elf+cfg->hdr64->e_phoff;
			for(i=0;i<cfg->hdr64->e_phnum;i++)if(
				devcfg_check_program64(cfg,i,&phb[i],false)&&
				elf_prog_ptr_in_range(&phb[i],addr)
			){
				offset=addr-phb[i].p_paddr;
				if(avail)*avail=phb[i].p_memsz-offset;
				return (ssize_t)phb[i].p_offset+offset;
			}
		}break;
	}
	r_err(-1,_("address 0x%zx not found in devcfg elf"),addr);
}

void*devcfg_read(devcfg*cfg,uintptr_t addr,void*buffer,size_t len,bool str){
	ssize_t r;
	void*base;
	size_t avail=0;
	devcfg_check_loaded_ptr(cfg);
	if(!buffer||len<=0)return NULL;
	memset(buffer,0,len);
	if((r=devcfg_get_file_offset(cfg,addr,&avail))<=0)
		r_perr(_("get file offset of pointer 0x%zx failed"),addr);
	if((str?1:len)>avail)r_perr(
		_("request read 0x%zx out of devcfg (require %zu bytes > available %zu bytes)"),
		addr,len,avail
	);
	base=cfg->elf+(size_t)r;
	if(!str)memcpy(buffer,base,MIN(len,avail));
	else strncpy(buffer,base,MIN(len-1,avail));
	return buffer;
}
