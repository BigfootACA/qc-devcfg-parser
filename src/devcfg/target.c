//
// Created by bigfoot on 2023/7/24.
//

#include"internal.h"

DECL_REFCNT(devcfg_target,{
	devcfg_unref(var->devcfg);
	devcfg_section_unref(var->section);
})

devcfg_target*devcfg_target_new(devcfg_section*section){
	if(!devcfg_section_check(section))return NULL;
	devcfg_target*target=malloc(sizeof(devcfg_target));
	if(!target)r_perr(_("alloc devcfg target failed"));
	memset(target,0,sizeof(devcfg_target));
	target->devcfg=devcfg_ref(section->devcfg);
	target->section=devcfg_section_ref(section);
	return devcfg_target_ref(target);
}

bool devcfg_target_check(devcfg_target*target){
	if(!target)return false;
	if(target->refcnt<=0)return false;
	if(!devcfg_check(target->devcfg))return false;
	if(!devcfg_section_check(target->section))return false;
	if(target->section->devcfg!=target->devcfg)return false;
	return true;
}

int64_t devcfg_target_get_soc(devcfg_target*target){
	if(!devcfg_target_check(target))return -1;
	return target->tgt.soc_id;
}

devcfg*devcfg_target_get_devcfg(devcfg_target*target){
	if(!devcfg_target_check(target))return NULL;
	if(!devcfg_check(target->devcfg))return NULL;
	return target->devcfg;
}

devcfg_section*devcfg_target_get_section(devcfg_target*target){
	if(!devcfg_target_check(target))return NULL;
	if(!devcfg_section_check(target->section))return NULL;
	return target->section;
}

int devcfg_target_set_info(devcfg_target*target,devcfg_target_info*info){
	uintptr_t ptr;
	if(!devcfg_target_check(target)||!info)return -1;
	devcfg_check_loaded_int(target->devcfg,-1);
	memcpy(&target->tgt,info,sizeof(devcfg_target_info));
	if(!(ptr=(uintptr_t)target->tgt.props))
		r_err(-1,_("invalid devcfg props"));
	if(!devcfg_read(target->devcfg,ptr,&target->props,sizeof(target->props),false))
		r_err(-1,_("read devcfg props failed"));
	if(!(ptr=(uintptr_t)target->props.bin))
		r_err(-1,_("invalid devcfg props binary"));
	if(!devcfg_read(target->devcfg,ptr,&target->dir,sizeof(target->dir),false))
		r_err(-1,_("read devcfg props dir failed"));
	return 0;
}

devcfg_device**devcfg_target_get_devices(devcfg_target*target){
	uint32_t i;
	size_t size;
	uintptr_t ptr;
	devcfg_device**device;
	devcfg_string_device dev;
	if(!devcfg_target_check(target))return NULL;
	devcfg_check_loaded_ptr(target->devcfg);
	size=sizeof(devcfg_device*)*(target->props.device_cnt+1);
	if(!(device=malloc(size)))r_perr(_("alloc devices failed"));
	memset(device,0,size);
	for(i=0;i<target->props.device_cnt;i++){
		ptr=(uintptr_t)target->props.devices+sizeof(dev)*i;
		if(!devcfg_read(target->devcfg,ptr,&dev,sizeof(dev),false))
			r_perr(_("read devcfg string device failed"));
		if(!(device[i]=devcfg_device_new(target)))continue;
		devcfg_device_set_dev(device[i],&dev);
	}
	return device;
}

devcfg_device*devcfg_target_get_device_by_name(devcfg_target*target,const char*name){
	uint32_t i;
	uintptr_t ptr;
	devcfg_string_device dev;
	char buff[4096];
	devcfg_device*device;
	if(!devcfg_target_check(target)||!name)return NULL;
	devcfg_check_loaded_ptr(target->devcfg);
	for(i=0;i<target->props.device_cnt;i++){
		ptr=(uintptr_t)target->props.devices+sizeof(dev)*i;
		if(!devcfg_read(target->devcfg,ptr,&dev,sizeof(dev),false))
			r_perr(_("read devcfg string device failed"));
		if(!(ptr=(uintptr_t)device->dev.name))continue;
		if(!devcfg_read(device->devcfg,ptr,buff,sizeof(buff),true))
			r_perr(_("read devcfg string device name failed"));
		if(strcmp(buff,name)!=0)continue;
		if(!(device=devcfg_device_new(target)))continue;
		devcfg_device_set_dev(device,&dev);
		return device;
	}
	return NULL;
}
