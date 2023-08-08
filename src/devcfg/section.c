//
// Created by bigfoot on 2023/7/24.
//

#include"internal.h"

DECL_REFCNT(devcfg_section,{
	devcfg_unref(var->devcfg);
})

devcfg_section*devcfg_section_new(devcfg*cfg){
	if(!devcfg_check(cfg))return NULL;
	devcfg_section*section=malloc(sizeof(devcfg_section));
	if(!section)r_perr(_("alloc devcfg section failed"));
	memset(section,0,sizeof(devcfg_section));
	section->devcfg=devcfg_ref(cfg);
	return devcfg_section_ref(section);
}

bool devcfg_section_check(devcfg_section*section){
	if(!section)return false;
	if(section->refcnt<=0)return false;
	if(!devcfg_check(section->devcfg))return false;
	return true;
}

int32_t devcfg_section_get_id(devcfg_section*section){
	if(!devcfg_section_check(section))return -1;
	return section->id;
}

devcfg*devcfg_section_get_devcfg(devcfg_section*section){
	if(!devcfg_section_check(section))return NULL;
	if(!devcfg_check(section->devcfg))return NULL;
	return section->devcfg;
}

uintptr_t devcfg_section_get_base(devcfg_section*section){
	if(!devcfg_section_check(section))return 0;
	devcfg_check_loaded_int(section->devcfg,0);
	switch(section->devcfg->elf_class){
		case ELFCLASS32:return section->phdr.p32->p_paddr;
		case ELFCLASS64:return section->phdr.p64->p_paddr;
		default:return 0;
	}
}

devcfg_target**devcfg_section_get_targets(devcfg_section*section){
	size_t size;
	uintptr_t ptr;
	uint32_t cnt=0,i=0;
	devcfg_target**target;
	devcfg_target_info tgt;
	if(!devcfg_section_check(section))return NULL;
	devcfg_check_loaded_ptr(section->devcfg);
	if((ptr=devcfg_section_get_base(section)))do{
		if(!devcfg_read(section->devcfg,ptr,&tgt,sizeof(tgt),false))
			r_perr(_("read devcfg target info failed"));
		ptr+=sizeof(tgt);
		if(tgt.soc_id!=0&&tgt.props)cnt++;
	}while(tgt.props);
	size=sizeof(devcfg_target*)*(cnt+1);
	if(!(target=malloc(size)))r_perr(_("alloc targets failed"));
	memset(target,0,size);
	if((ptr=devcfg_section_get_base(section)))do{
		if(!devcfg_read(section->devcfg,ptr,&tgt,sizeof(tgt),false))
			r_perr(_("read devcfg target info failed"));
		ptr+=sizeof(tgt);
		if(tgt.soc_id==0&&!tgt.props)continue;
		if(!(target[i]=devcfg_target_new(section)))continue;
		devcfg_target_set_info(target[i],&tgt);
		i++;
	}while(tgt.props);
	return target;
}

devcfg_target*devcfg_section_get_target_by_id(devcfg_section*section,uint32_t id){
	uint32_t i=0;
	uintptr_t ptr;
	devcfg_target*target;
	devcfg_target_info tgt;
	if(!devcfg_section_check(section))return NULL;
	devcfg_check_loaded_ptr(section->devcfg);
	if((ptr=devcfg_section_get_base(section)))do{
		if(!devcfg_read(section->devcfg,ptr,&tgt,sizeof(tgt),false))
			r_perr(_("read devcfg target info failed"));
		ptr+=sizeof(tgt);
		if(tgt.soc_id==0||!tgt.props||id++!=i)continue;
		if(!(target=devcfg_target_new(section)))break;
		devcfg_target_set_info(target,&tgt);
		return target;
	}while(tgt.props);
	return NULL;
}

devcfg_target*devcfg_section_get_target_by_soc(devcfg_section*section,uint32_t soc){
	uintptr_t ptr;
	devcfg_target*target;
	devcfg_target_info tgt;
	if(!devcfg_section_check(section))return NULL;
	devcfg_check_loaded_ptr(section->devcfg);
	if((ptr=devcfg_section_get_base(section)))do{
		if(!devcfg_read(section->devcfg,ptr,&tgt,sizeof(tgt),false))
			r_perr(_("read devcfg target info failed"));
		ptr+=sizeof(tgt);
		if(tgt.soc_id==0||!tgt.props)continue;
		if(tgt.soc_id!=soc)continue;
		if(!(target=devcfg_target_new(section)))break;
		devcfg_target_set_info(target,&tgt);
		return target;
	}while(tgt.props);
	return NULL;
}

devcfg_device*devcfg_section_get_device_by_name(devcfg_section*section,const char*name){
	size_t i;
	devcfg_device*dev,*ret=NULL;
	devcfg_target**targets=devcfg_section_get_targets(section);
	if(!targets)return NULL;
	for(i=0;targets[i];i++){
		dev=devcfg_target_get_device_by_name(targets[i],name);
		if(dev&&!ret)ret=dev;
		devcfg_target_unref(targets[i]);
	}
	free(targets);
	return ret;
}
