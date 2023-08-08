//
// Created by bigfoot on 2023/7/26.
//

#include<inttypes.h>
#include<string.h>
#include<stdbool.h>
#include<stdlib.h>
#include"defines.h"
#include"language.h"
#include"list.h"

static language*current_language=NULL;

struct lang_cb{
	language_callback cb;
	void*data;
};

static list*callbacks=NULL;

// swapc and mo_lookup from musl libc
static inline uint32_t swapc(uint32_t x,int c){return c?x>>24|(x>>8&0xff00)|(x<<8&0xff0000)|x<<24:x;}

static char*mo_lookup(const void*p,size_t size,const char*s){
	const uint32_t*mo=p;
	int sw=*mo-0x950412de;
	uint32_t b=0,n=swapc(mo[2],sw),o=swapc(mo[3],sw),t=swapc(mo[4],sw);
	if(n>=size/4||o>=size-4*n||t>=size-4*n||((o|t)%4))return NULL;
	o/=4,t/=4;
	for(;;){
		uint32_t ol=swapc(mo[o+2*(b+n/2)],sw),os=swapc(mo[o+2*(b+n/2)+1],sw);
		if(os>=size||ol>=size-os||((char*)p)[os+ol])return NULL;
		int sign=strcmp(s,(char*)p+os);
		if(!sign){
			uint32_t tl=swapc(mo[t+2*(b+n/2)],sw),ts=swapc(mo[t+2*(b+n/2)+1],sw);
			if(ts>=size||tl>=size-ts||((char*)p)[ts+tl])return NULL;
			return (char*)p+ts;
		}else if(n==1)return NULL;
		else if(sign<0)n/=2;
		else b+=n/2,n-=n/2;
	}
}

static maybe_unused language*language_select_str(const char*lang){
	bool cont;
	size_t i,j;
	if(!lang)return NULL;
	for(i=0;languages[i].id;i++)
		if(strcasecmp(languages[i].id,lang)==0)
			return &languages[i];
	for(j=0;;j++,cont=false){
		for(i=0;languages[i].id;i++){
			if(!languages[i].match)continue;
			if(!languages[i].match[j])continue;
			if(strcasecmp(languages[i].match[j],lang)==0)
				return &languages[i];
			cont=true;
		}
		if(!cont)break;
	}
	return NULL;
}

static maybe_unused language*language_select_id(uint16_t id){
	size_t i;
	if(id<=0)return NULL;
	for(i=0;languages[i].id;i++)
		if(languages[i].lang_id==id)
			return &languages[i];
	return NULL;
}

static maybe_unused language*language_select_def(){
	size_t i;
	for(i=0;languages[i].id;i++)
		if(languages[i].def)
			return &languages[i];
	return NULL;
}

void language_init(){
	static bool warned=false;
	language*lang=NULL;
	#if __WIN32__
	if(!lang)lang=language_select_id(GetUserDefaultUILanguage());
	#endif
	if(!lang)lang=language_select_str(getenv("LC_ALL"));
	if(!lang)lang=language_select_str(getenv("LANG"));
	if(!lang)lang=language_select_str(getenv("LANGUAGE"));
	if(!lang)lang=language_select_def();
	if(!lang){
		if(!warned)warn("no any match language found");
		warned=true;
	}else language_set(lang);
}

static void language_internal_init(){
	static bool init=false;
	if(current_language||init)return;
	language_init();
	init=true;
}

const char*lib_gettext(const char*str){
	language_internal_init();
	if(!current_language)return str;
	if(!current_language->data)return str;
	char*ret=mo_lookup(
		current_language->data->data,
		current_language->data->size,
		str
	);
	return ret&&*ret?ret:str;
}

language*language_get_current(void){
	language_internal_init();
	return current_language;
}

int language_set(language*lang){
	list*l;
	if(!lang)return -1;
	if(lang==current_language)return 0;
	language*old_lang=current_language;
	language*new_lang=lang;
	current_language=lang;
	if((l=list_first(callbacks)))do{
		LIST_DATA_DECLARE(cb,l,struct lang_cb*);
		if(!cb||!cb->cb)continue;
		cb->cb(cb->data,old_lang,new_lang);
	}while((l=l->next));
	return 0;
}

static bool lang_cb_id_cmp(list*l,void*d){
	struct lang_cb*cb1=d;
	LIST_DATA_DECLARE(cb2,l,struct lang_cb*);
	if(!cb1||!cb2)return false;
	if(cb1==cb2)return true;
	if(cb1->cb!=cb2->cb)return false;
	if(cb1->data!=cb2->data)return false;
	return true;
}

int language_add_callback(language_callback cb,void*data){
	struct lang_cb c={.cb=cb,.data=data};
	if(!cb||list_search_one(callbacks,lang_cb_id_cmp,&c))return -1;
	return list_obj_add_new_dup(&callbacks,&c,sizeof(c));
}

int language_del_callback(language_callback cb,void*data){
	list*l;
	struct lang_cb c={.cb=cb,.data=data};
	if((l=list_search_one(callbacks,lang_cb_id_cmp,&c)))return -1;
	return list_obj_del(&callbacks,l,list_default_free);
}
