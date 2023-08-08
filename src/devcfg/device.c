//
// Created by bigfoot on 2023/7/24.
//

#include"internal.h"

DECL_REFCNT(devcfg_device,{
	devcfg_unref(var->devcfg);
	devcfg_section_unref(var->section);
	devcfg_target_unref(var->target);
})

devcfg_device*devcfg_device_new(devcfg_target*target){
	if(!devcfg_target_check(target))return NULL;
	devcfg_device*device=malloc(sizeof(devcfg_device));
	if(!device)r_perr(_("alloc devcfg device failed"));
	memset(device,0,sizeof(devcfg_device));
	device->devcfg=devcfg_ref(target->devcfg);
	device->section=devcfg_section_ref(target->section);
	device->target=devcfg_target_ref(target);
	return devcfg_device_ref(device);
}

bool devcfg_device_check(devcfg_device*device){
	if(!device)return false;
	if(device->refcnt<=0)return false;
	if(!devcfg_check(device->devcfg))return false;
	if(!devcfg_section_check(device->section))return false;
	if(device->section->devcfg!=device->devcfg)return false;
	if(!devcfg_target_check(device->target))return false;
	if(device->target->devcfg!=device->devcfg)return false;
	if(device->target->section!=device->section)return false;
	return true;
}

devcfg*devcfg_device_get_devcfg(devcfg_device*device){
	if(!devcfg_device_check(device))return NULL;
	if(!devcfg_check(device->devcfg))return NULL;
	return device->devcfg;
}

const char*devcfg_device_get_name(devcfg_device*device){
	if(!devcfg_device_check(device))return NULL;
	return device->name;
}

devcfg_section*devcfg_device_get_section(devcfg_device*device){
	if(!devcfg_device_check(device))return NULL;
	if(!devcfg_section_check(device->section))return NULL;
	return device->section;
}

devcfg_target*devcfg_device_get_target(devcfg_device*device){
	if(!devcfg_device_check(device))return NULL;
	if(!devcfg_target_check(device->target))return NULL;
	return device->target;
}

int devcfg_device_set_dev(devcfg_device*device,devcfg_string_device*dev){
	uintptr_t ptr;
	if(!devcfg_device_check(device)||!dev)return -1;
	devcfg_check_loaded_int(device->devcfg,-1);
	memcpy(&device->dev,dev,sizeof(devcfg_string_device));
	if(!(ptr=(uintptr_t)device->dev.name))
		r_err(-1,_("invalid devcfg string device name"));
	if(!devcfg_read(device->devcfg,ptr,device->name,sizeof(device->name),true))
		r_err(-1,_("read devcfg string device name failed"));
	return 0;
}

static int calc_prop_skip(devcfg_device*device,devcfg_prop_type type,uintptr_t*ptr){
	uint8_t buff,len;
	switch(type){
		case DEVCFG_PROP_STRING_PTR:
		case DEVCFG_PROP_UINT32:
		case DEVCFG_PROP_BYTE_ARRAY_PTR:
		case DEVCFG_PROP_UINT32_ARRAY_PTR:
		case DEVCFG_PROP_STRUCT_PTR:
			*ptr+=sizeof(uint32_t);
		break;
		case DEVCFG_PROP_BYTE_ARRAY:
			if(!devcfg_read(device->devcfg,*ptr,&buff,sizeof(buff),false))
				r_err(-1,_("read devcfg prop byte array len failed"));
			len=buff+1+1,*ptr+=len;
			if(len%4)*ptr+=4-(len%4);
		break;
		default:r_err(-1,_("unknown devcfg prop type %d"),type);
	}
	return 0;
}

devcfg_prop**devcfg_device_get_props(devcfg_device*device){
	size_t size,cnt=0;
	devcfg_prop**props;
	uintptr_t ptr,data,bin,str;
	uint32_t i=0,first,type,str_off;
	if(!devcfg_device_check(device))return NULL;
	devcfg_check_loaded_ptr(device->devcfg);
	if(!(bin=(uintptr_t)device->target->props.bin))
		r_perr(_("invalid devcfg props binary"));
	data=bin+device->dev.offset;
	str=bin+device->target->dir.name_offset;
	for(ptr=data;;){
		if(!devcfg_read(device->devcfg,ptr,&first,sizeof(first),false))
			r_perr(_("read devcfg prop first number failed"));
		ptr+=sizeof(first),type=(first&0xFF000000)>>24;
		if(first==DEVCFG_PROP_END)break;
		calc_prop_skip(device,type,&ptr);
		cnt++;
	}
	size=sizeof(devcfg_prop*)*(cnt+1);
	if(!(props=malloc(size)))r_perr(_("alloc props failed"));
	memset(props,0,size);
	for(ptr=data;;){
		if(!devcfg_read(device->devcfg,ptr,&first,sizeof(first),false))
			r_perr(_("read devcfg prop first number failed"));
		ptr+=sizeof(first),type=(first&0xFF000000)>>24;
		if(first==DEVCFG_PROP_END)break;
		if((props[i]=devcfg_prop_new(device))){
			str_off=(first&0x00FFFFFF);
			if(str_off&0x00800000){
				str_off=str_off&0x007FFFFF;
				if(!devcfg_read(
					device->devcfg,str+str_off,
					props[i]->name,sizeof(props[i]->name),true
				))r_perr(_("read devcfg prop name failed"));
			}else snprintf(props[i]->name,sizeof(props[i]->name)-1,"%u",str_off);
			props[i]->type=type,props[i]->ptr=ptr;
			i++;
		}
		calc_prop_skip(device,type,&ptr);
	}
	return props;
}

devcfg_prop*devcfg_device_get_prop_by_name(devcfg_device*dev,const char*name){
	devcfg_prop*ret=NULL,**props;
	if(!devcfg_device_check(dev)||!name||!*name)return NULL;
	if(!(props=devcfg_device_get_props(dev)))return NULL;
	for(size_t i=0;props[i];i++){
		if(devcfg_prop_cmp_name(props[i],name)==0){
			if(ret)devcfg_prop_unref(ret);
			ret=devcfg_prop_ref(props[i]);
		}
		devcfg_prop_unref(props[i]);
	}
	free(props);
	return ret;
}
