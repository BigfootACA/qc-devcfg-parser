//
// Created by bigfoot on 2023/7/24.
//

#include"internal.h"

DECL_REFCNT(devcfg_prop,{
	devcfg_unref(var->devcfg);
	devcfg_section_unref(var->section);
	devcfg_target_unref(var->target);
	devcfg_device_unref(var->device);
})

devcfg_prop*devcfg_prop_new(devcfg_device*device){
	if(!devcfg_device_check(device))return NULL;
	devcfg_prop*prop=malloc(sizeof(devcfg_prop));
	if(!prop)r_perr(_("alloc devcfg prop failed"));
	memset(prop,0,sizeof(devcfg_prop));
	prop->devcfg=devcfg_ref(device->devcfg);
	prop->section=devcfg_section_ref(device->section);
	prop->target=devcfg_target_ref(device->target);
	prop->device=devcfg_device_ref(device);
	return devcfg_prop_ref(prop);
}

bool devcfg_prop_check(devcfg_prop*prop){
	if(!prop)return false;
	if(prop->refcnt<=0)return false;
	if(!devcfg_check(prop->devcfg))return false;
	if(!devcfg_section_check(prop->section))return false;
	if(prop->section->devcfg!=prop->devcfg)return false;
	if(!devcfg_target_check(prop->target))return false;
	if(prop->target->devcfg!=prop->devcfg)return false;
	if(prop->target->section!=prop->section)return false;
	if(!devcfg_device_check(prop->device))return false;
	if(prop->device->devcfg!=prop->devcfg)return false;
	if(prop->device->section!=prop->section)return false;
	if(prop->device->target!=prop->target)return false;
	return true;
}

const char*devcfg_prop_get_name(devcfg_prop*prop){
	if(!devcfg_prop_check(prop))return NULL;
	return prop->name;
}

devcfg_prop_type devcfg_prop_get_type(devcfg_prop*prop){
	if(!devcfg_prop_check(prop))return DEVCFG_PROP_UNKNOWN;
	return prop->type;
}

const char*devcfg_prop_get_type_readable(devcfg_prop*prop,const char*def){
	if(!devcfg_prop_check(prop))return NULL;
	return devcfg_prop_type_to_readable(prop->type,def);
}

const char*devcfg_prop_get_type_symbol(devcfg_prop*prop,const char*def){
	if(!devcfg_prop_check(prop))return NULL;
	return devcfg_prop_type_to_symbol(prop->type,def);
}

devcfg*devcfg_prop_get_devcfg(devcfg_prop*prop){
	if(!devcfg_prop_check(prop))return NULL;
	if(!devcfg_check(prop->devcfg))return NULL;
	return prop->devcfg;
}

devcfg_section*devcfg_prop_get_section(devcfg_prop*prop){
	if(!devcfg_prop_check(prop))return NULL;
	if(!devcfg_section_check(prop->section))return NULL;
	return prop->section;
}

devcfg_target*devcfg_prop_get_target(devcfg_prop*prop){
	if(!devcfg_prop_check(prop))return NULL;
	if(!devcfg_target_check(prop->target))return NULL;
	return prop->target;
}

devcfg_device*devcfg_prop_get_device(devcfg_prop*prop){
	if(!devcfg_prop_check(prop))return NULL;
	if(!devcfg_device_check(prop->device))return NULL;
	return prop->device;
}
