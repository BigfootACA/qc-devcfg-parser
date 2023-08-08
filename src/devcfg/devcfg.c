//
// Created by bigfoot on 2023/7/24.
//

#include"internal.h"

DECL_REFCNT(devcfg,{
	if(var->alloc&&var->elf)free(var->elf);
})

devcfg*devcfg_new(void){
	devcfg*cfg=malloc(sizeof(devcfg));
	if(!cfg)r_perr(_("alloc devcfg failed"));
	memset(cfg,0,sizeof(devcfg));
	return devcfg_ref(cfg);
}

bool devcfg_check(devcfg*devcfg){
	if(!devcfg)return false;
	if(devcfg->refcnt<=0)return false;
	return true;
}

bool devcfg_is_loaded(devcfg*devcfg){
	if(!devcfg_check(devcfg))return false;
	if(!devcfg->elf||devcfg->len<=0)return false;
	if(!devcfg->loaded)return false;
	return true;
}

int32_t*devcfg_list_section_by_id(devcfg*cfg,size_t*cnt){
	size_t size=sizeof(int32_t),i,o=0;
	if(cnt)*cnt=0;
	devcfg_check_loaded_ptr(cfg);
	switch(cfg->elf_class){
		case ELFCLASS32:size*=cfg->hdr32->e_phnum;break;
		case ELFCLASS64:size*=cfg->hdr64->e_phnum;break;
		default:return NULL;
	}
	int32_t*array=malloc(size);
	if(!array)return NULL;
	switch(cfg->elf_class){
		case ELFCLASS32:{
			Elf32_Phdr*phb=cfg->elf+cfg->hdr32->e_phoff;
			for(i=0;i<cfg->hdr32->e_phnum;i++)
				if(devcfg_check_program32(cfg,i,&phb[i],false))
					array[o++]=(int32_t)i;
		}break;
		case ELFCLASS64:{
			Elf64_Phdr*phb=cfg->elf+cfg->hdr64->e_phoff;
			for(i=0;i<cfg->hdr64->e_phnum;i++)
				if(devcfg_check_program64(cfg,i,&phb[i],false))
					array[o++]=(int32_t)i;
		}break;
	}
	array[o]=-1;
	if(cnt)*cnt=o;
	return array;
}

devcfg_section*devcfg_get_section_by_id(devcfg*cfg,uint16_t id){
	devcfg_section*section;
	elf_phdr phdr;
	devcfg_check_loaded_ptr(cfg);
	switch(cfg->elf_class){
		case ELFCLASS32:
			phdr.p32=cfg->elf+cfg->hdr32->e_phoff;
			if(id>=cfg->hdr32->e_phnum)r_perr(_("section %u out of file"),id);
			phdr.p32=&phdr.p32[id];
		break;
		case ELFCLASS64:
			phdr.p64=cfg->elf+cfg->hdr64->e_phoff;
			if(id>=cfg->hdr64->e_phnum)r_perr(_("section %u out of file"),id);
			phdr.p64=&phdr.p64[id];
		break;
		default:return NULL;
	}
	if(!devcfg_check_program(cfg,id,phdr,true))return NULL;
	if((section=devcfg_section_new(cfg)))section->id=id,section->phdr=phdr;
	return section;
}

devcfg_section**devcfg_get_sections(devcfg*cfg){
	size_t cnt=0,size,i;
	devcfg_section**sections;
	int32_t*ids=devcfg_list_section_by_id(cfg,&cnt);
	if(!ids)return NULL;
	size=sizeof(devcfg_section)*(cnt+1);
	if(!(sections=malloc(size)))goto fail;
	memset(sections,0,size);
	for(i=0;i<cnt;i++)sections[i]=devcfg_get_section_by_id(cfg,ids[i]);
	fail:free(ids);
	return sections;
}

devcfg_device*devcfg_get_device_by_name(devcfg*cfg,const char*name){
	size_t i;
	devcfg_device*dev,*ret=NULL;
	devcfg_section**sections=devcfg_get_sections(cfg);
	if(!sections)return NULL;
	for(i=0;sections[i];i++){
		dev=devcfg_section_get_device_by_name(sections[i],name);
		if(dev&&!ret)ret=dev;
		devcfg_section_unref(sections[i]);
	}
	free(sections);
	return ret;
}
