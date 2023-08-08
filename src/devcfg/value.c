//
// Created by bigfoot on 2023/7/24.
//

#include"internal.h"

devcfg_struct_match*devcfg_prop_find_struct_match(devcfg_prop*prop){
	const char*dev_name,*prop_name;
	if(!devcfg_prop_check(prop))return NULL;
	if(devcfg_prop_get_type(prop)!=DEVCFG_PROP_STRUCT_PTR)return NULL;
	if(!(dev_name=devcfg_prop_get_device_name(prop)))return NULL;
	if(!(prop_name=devcfg_prop_get_name(prop)))return NULL;
	return devcfg_struct_find_match(dev_name,prop_name);
}

devcfg_struct*devcfg_prop_find_struct(devcfg_prop*prop){
	return devcfg_struct_match_get_type(
		devcfg_prop_find_struct_match(prop)
	);
}

ssize_t devcfg_prop_guess_struct_size(devcfg_prop*prop){
	uintptr_t p;
	devcfg_struct_match*match;
	if(!devcfg_prop_check(prop))return -1;
	if(!(match=devcfg_prop_find_struct_match(prop)))return -1;
	if((p=devcfg_prop_get_value_address(prop))<=0)return -1;
	return devcfg_struct_get_size(match,prop,p);
}

static ssize_t devcfg_prop_get_value_length_off(devcfg_prop*prop){
	uint32_t off;
	uintptr_t ptr,bin;
	if(!(bin=(uintptr_t)prop->target->props.bin))
		r_err(-1,_("invalid devcfg props binary"));
	if(!devcfg_read(prop->devcfg,prop->ptr,&off,sizeof(off),false))
		r_err(-1,_("read devcfg prop offset failed"));
	switch(prop->type){
		case DEVCFG_PROP_UINT32_ARRAY_PTR:{
			uint32_t cnt;
			ptr=bin+prop->target->dir.uint32_offset;
			if(!devcfg_read(prop->devcfg,ptr+off,&cnt,sizeof(cnt),false))
				r_err(-1,_("read devcfg uint32 array ptr length failed"));
			return (ssize_t)((cnt+1)*sizeof(uint32_t));
		}
		case DEVCFG_PROP_BYTE_ARRAY_PTR:{
			uint8_t cnt;
			ptr=bin+prop->target->dir.byte_offset;
			if(!devcfg_read(prop->devcfg,ptr+off,&cnt,sizeof(cnt),false))
				r_err(-1,_("read devcfg byte array ptr length failed"));
			return (ssize_t)(cnt+1);
		}
		case DEVCFG_PROP_STRUCT_PTR:{
			devcfg_struct_type st;
			ptr=(uintptr_t)prop->target->props.struct_ptr;
			ptr+=sizeof(devcfg_struct_type)*off;
			if(!devcfg_read(prop->devcfg,ptr,&st,sizeof(st),false))
				r_err(-1,_("read devcfg struct tbl failed"));
			if(st.size!=sizeof(void*))
				r_err(-1,_("struct tbl pointer size mismatch"));
			return max_ssize_t(devcfg_prop_guess_struct_size(prop),0);
		}
		case DEVCFG_PROP_STRING_PTR:{
			char*str;
			size_t avail=0;
			ssize_t offset;
			ptr=bin+prop->target->dir.string_offset;
			offset=devcfg_get_file_offset(prop->devcfg,ptr+off,&avail);
			if(offset<=0||avail<=0)return -1;
			str=prop->devcfg->elf+offset;
			return (ssize_t)strnlen(str,avail);
		}
		default:return -1;
	}
}

ssize_t devcfg_prop_get_value_length(devcfg_prop*prop){
	if(!devcfg_prop_check(prop))return -1;
	switch(prop->type){
		case DEVCFG_PROP_BYTE_ARRAY:{
			uint8_t len;
			if(!devcfg_read(
				prop->devcfg,prop->ptr,
				&len,sizeof(len),false
			))r_err(-1,_("read devcfg byte array len failed"));
			return (ssize_t)len;
		}
		case DEVCFG_PROP_UINT32:
			return sizeof(uint32_t);
		case DEVCFG_PROP_UINT32_ARRAY_PTR:
		case DEVCFG_PROP_BYTE_ARRAY_PTR:
		case DEVCFG_PROP_STRING_PTR:
		case DEVCFG_PROP_STRUCT_PTR:
			return devcfg_prop_get_value_length_off(prop);
		default:return -1;
	}
}

char*devcfg_prop_get_value_string(devcfg_prop*prop){
	ssize_t len;
	if(!devcfg_prop_check(prop))return NULL;
	if((len=devcfg_prop_get_value_length(prop))<=0)return NULL;
	return devcfg_prop_get_value_bytes(prop,NULL,len);
}

const char*devcfg_prop_get_value_string_buf(
	devcfg_prop*prop,
	char*buff,
	size_t size
){
	uintptr_t ptr;
	if(!devcfg_prop_check(prop)||!buff||size<=0)return NULL;
	if((ptr=devcfg_prop_get_value_address(prop))<=0)return NULL;
	return devcfg_read(prop->devcfg,ptr,buff,size,true);
}

void*devcfg_prop_get_value_bytes(
	devcfg_prop*prop,
	size_t*len,
	size_t require
){
	size_t max;
	uintptr_t ptr;
	void*buffer=NULL;
	if(len)*len=0;
	if(!devcfg_prop_check(prop))goto fail;
	if((max=devcfg_prop_get_value_length(prop))<=0)goto fail;
	if(require<=0||require>max)require=max;
	if(!(buffer=malloc(require)))goto fail;
	memset(buffer,0,require);
	if((ptr=devcfg_prop_get_value_address(prop))<=0)goto fail;
	if(!devcfg_read(prop->devcfg,ptr,buffer,require,false))goto fail;
	if(len)*len=require;
	return buffer;
	fail:
	if(len)*len=0;
	if(buffer)free(buffer);
	return NULL;
}

ssize_t devcfg_prop_get_value_bytes_buf(
	devcfg_prop*prop,
	void*buffer,
	size_t len,
	size_t require
){
	size_t max;
	uintptr_t ptr;
	if(!devcfg_prop_check(prop)||!buffer||len<=0)return -1;
	if((max=devcfg_prop_get_value_length(prop))<=0)return -1;
	if(require<=0||require>max)require=max;
	if(require>len)require=len;
	memset(buffer,0,len);
	if((ptr=devcfg_prop_get_value_address(prop))<=0)return -1;
	if(!devcfg_read(prop->devcfg,ptr,buffer,require,false))return -1;
	return (ssize_t)require;
}

uintptr_t devcfg_prop_get_value_address(devcfg_prop*prop){
	uint32_t off;
	devcfg_struct_type st;
	uintptr_t ptr,data,bin;
	if(!devcfg_prop_check(prop))return -1;
	if(!(bin=(uintptr_t)prop->target->props.bin))
		r_err(0,_("invalid devcfg props binary"));
	data=bin+prop->target->dir.uint32_offset;
	if(!devcfg_read(prop->devcfg,prop->ptr,&off,sizeof(off),false))
		r_err(0,_("read devcfg value offset failed"));
	switch(prop->type){
		case DEVCFG_PROP_UINT32_ARRAY_PTR:
			return (uintptr_t)data+off+sizeof(uint32_t);
		case DEVCFG_PROP_UINT32:
			return (uintptr_t)prop->ptr;
		case DEVCFG_PROP_STRING_PTR:
			return (uintptr_t)data+off;
		case DEVCFG_PROP_BYTE_ARRAY:
			return (uintptr_t)data+off+1;
		case DEVCFG_PROP_BYTE_ARRAY_PTR:
			ptr=(uintptr_t)prop->target->props.struct_ptr;
			return (uintptr_t)ptr+off;
		case DEVCFG_PROP_STRUCT_PTR:
			ptr=(uintptr_t)prop->target->props.struct_ptr;
			ptr+=sizeof(devcfg_struct_type)*off;
			if(!devcfg_read(prop->devcfg,ptr,&st,sizeof(st),false))
				r_err(0,_("read devcfg struct tbl failed"));
			return (uintptr_t)st.ptr;
		default:return 0;
	}
}

ssize_t devcfg_prop_get_value_file_offset(devcfg_prop*prop,size_t*avail){
	uintptr_t ptr=devcfg_prop_get_value_address(prop);
	return ptr>0?devcfg_get_file_offset(prop->devcfg,ptr,avail):-1;
}

#define DECL_NUMBER_GET(type)\
	bool devcfg_prop_get_value_##type(devcfg_prop*prop,type##_t*val){\
		ssize_t len,bytes;\
		size_t get=sizeof(type##_t);\
		if(!devcfg_prop_check(prop)||!val)return false;\
		len=devcfg_prop_get_value_length(prop);\
		if((size_t)len<get)return false;\
		bytes=devcfg_prop_get_value_bytes_buf(prop,val,get,len);\
		return bytes>0&&(size_t)bytes==get;\
	}\
	type##_t*devcfg_prop_get_value_##type##_array(devcfg_prop*prop,size_t*count){\
		void*buf;\
		ssize_t len,bytes;\
		if(count)*count=0;\
		if(!devcfg_prop_check(prop))return NULL;\
		len=devcfg_prop_get_value_length(prop);\
		if(len<=0||(size_t)len%sizeof(type##_t))return NULL;\
		if(!(buf=malloc(len)))r_perr(_("alloc number array failed"));\
		bytes=devcfg_prop_get_value_bytes_buf(prop,buf,len,len);\
		if(bytes!=len)goto fail;\
		if(count)*count=len/sizeof(type##_t);\
		return buf;\
		fail:free(buf);\
		return NULL;\
	}

DECL_NUMBER_GET(int8)
DECL_NUMBER_GET(int16)
DECL_NUMBER_GET(int32)
DECL_NUMBER_GET(int64)
DECL_NUMBER_GET(uint8)
DECL_NUMBER_GET(uint16)
DECL_NUMBER_GET(uint32)
DECL_NUMBER_GET(uint64)
