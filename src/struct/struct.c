//
// Created by bigfoot on 2023/8/1.
//

// TODO: implement struct viewer
#include"internal.h"
#include"regexp.h"

int devcfg_struct_match_dev_name(devcfg_struct_match*match,const char*dev_name){
	Reprog*prog=NULL;
	const char*err=NULL;
	if(!match||!match->device_regex)return -1;
	if(!dev_name||!dev_name[0])return -1;
	if(!(prog=regexp_comp(match->device_regex,0,&err)))r_warn(
		-1,_("error while compile regex \"%s\": %s"),
		match->device_regex,err
	);
	int ret=regexp_exec(prog,dev_name,NULL,0);
	regexp_free(prog);
	return ret;
}

int devcfg_struct_match_prop_name(devcfg_struct_match*match,const char*prop_name){
	Reprog*prog=NULL;
	const char*err=NULL;
	if(!match||!match->prop_regex)return -1;
	if(!prop_name||!prop_name[0])return -1;
	if(!(prog=regexp_comp(match->prop_regex,0,&err)))r_warn(
		-1,_("error while compile regex \"%s\": %s"),
		match->prop_regex,err
	);
	int ret=regexp_exec(prog,prop_name,NULL,0);
	regexp_free(prog);
	return ret;
}

devcfg_struct*devcfg_struct_match_get_type(devcfg_struct_match*match){
	return match?match->type.use_struct:NULL;
}

devcfg_struct_match*devcfg_struct_find_match(
	const char*dev_name,
	const char*prop_name
){
	if(!dev_name||!dev_name[0])return NULL;
	if(!prop_name||!prop_name[0])return NULL;
	FOREACH_CONSTRUCT(CONSTRUCT_STRUCT_MATCH,i){
		devcfg_struct_match*match=i.data->value;
		if(!match||!match->prop_regex||!match->device_regex)continue;
		if(devcfg_struct_match_dev_name(match,dev_name)!=0)continue;
		if(devcfg_struct_match_prop_name(match,prop_name)!=0)continue;
		return match;
	}
	return NULL;
}

devcfg_struct*devcfg_struct_find(
	const char*dev_name,
	const char*prop_name
){
	return devcfg_struct_match_get_type(
		devcfg_struct_find_match(dev_name,prop_name)
	);
}

ssize_t devcfg_struct_get_size(devcfg_struct_match*st,devcfg_prop*prop,uintptr_t ptr){
	if(!devcfg_prop_check(prop)||!st)return -1;
	if(!st->type.name||!st->type.pointer)return -1;
	if(st->type.type!=TYPE_STRUCT)return -1;
	if(st->type.size!=sizeof(void*))return -1;
	if(ptr>0&&st->guess_size)return st->guess_size(st,prop,ptr);
	if(st->type.field>0)return (ssize_t)st->type.field;
	return -1;
}
