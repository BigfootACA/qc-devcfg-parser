//
// Created by bigfoot on 2023/7/24.
//

#include"internal.h"

int devcfg_prop_dump(devcfg_prop*prop,int prefix){
	if(!devcfg_prop_check(prop))return -1;
	dbg_prefix(
		prefix,_("Prop %s type %s"),
		devcfg_prop_get_name(prop),
		devcfg_prop_get_type_readable(prop,_("(Unknown)"))
	);
	return 0;
}

int devcfg_device_dump(devcfg_device*device,int prefix){
	size_t i;
	devcfg_prop**props;
	DEBUG_REFCNT_START(devcfg_device,device);
	if(!devcfg_device_check(device))return -1;
	dbg_prefix(prefix,_("Device %s"),devcfg_device_get_name(device));
	if(!(props=devcfg_device_get_props(device)))return -1;
	for(i=0;props[i];i++){
		devcfg_prop_dump(props[i],prefix+1);
		devcfg_prop_unref(props[i]);
	}
	free(props);
	DEBUG_REFCNT_END(devcfg_device,device);
	return 0;
}

int devcfg_target_dump(devcfg_target*target,int prefix){
	size_t i;
	uint32_t soc;
	devcfg_device**devices;
	DEBUG_REFCNT_START(devcfg_target,target);
	if(!devcfg_target_check(target))return -1;
	soc=devcfg_target_get_soc(target);
	dbg_prefix(prefix,_("Target SOC %u (0x%x)"),soc,soc);
	if(!(devices=devcfg_target_get_devices(target)))return -1;
	for(i=0;devices[i];i++){
		devcfg_device_dump(devices[i],prefix+1);
		devcfg_device_unref(devices[i]);
	}
	free(devices);
	DEBUG_REFCNT_END(devcfg_target,target);
	return 0;
}

int devcfg_section_dump(devcfg_section*section,int prefix){
	size_t i;
	devcfg_target**targets;
	DEBUG_REFCNT_START(devcfg_section,section);
	if(!devcfg_section_check(section))return -1;
	dbg_prefix(prefix,_("Section %u"),devcfg_section_get_id(section));
	if(!(targets=devcfg_section_get_targets(section)))return -1;
	for(i=0;targets[i];i++){
		devcfg_target_dump(targets[i],prefix+1);
		devcfg_target_unref(targets[i]);
	}
	free(targets);
	DEBUG_REFCNT_END(devcfg_section,section);
	return 0;
}

int devcfg_dump(devcfg*cfg,int prefix){
	size_t i;
	int32_t*array;
	devcfg_section*section;
	devcfg_check_loaded_int(cfg,-1);
	DEBUG_REFCNT_START(devcfg,cfg);
	if(!(array=devcfg_list_section_by_id(cfg,NULL)))return -1;
	dbg_prefix(prefix,_("Dump of devcfg size %zu bytes"),cfg->len);
	for(i=0;array[i]>=0;i++){
		section=devcfg_get_section_by_id(cfg,array[i]);
		if(!section)continue;
		devcfg_section_dump(section,prefix+1);
		devcfg_section_unref(section);
	}
	free(array);
	DEBUG_REFCNT_END(devcfg,cfg);
	return 0;
}
