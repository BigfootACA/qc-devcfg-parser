//
// Created by bigfoot on 2023/5/17.
//

#include<stdlib.h>
#include<stdint.h>
#include<stdbool.h>
#include"construct.h"
#include"defines.h"

static bool run_init=false,run_fini=false;

void run_constructor(){
	if(run_init)return;
	FOREACH_CONSTRUCT(CONSTRUCT_FUNC_CONSTRUCTOR,data)
		CONSTRUCT_DATA(data,constructor)();
	if(!run_init){
		err(_("invalid constructor section"));
		abort();
	}
}

void run_destructor(){
	if(run_fini)return;
	FOREACH_CONSTRUCT(CONSTRUCT_FUNC_DESTRUCTOR,data)
		CONSTRUCT_DATA(data,constructor)();
	if(!run_fini){
		err(_("invalid constructor section"));
		abort();
	}
}

static void first_init(){run_init=true;}
static void first_fini(){run_fini=true;}
DECL_CONSTRUCTOR(first_init);
DECL_DESTRUCTOR(first_fini);
