#include<stdio.h>
#include<stdlib.h>
#include"devcfg.h"
#include"defines.h"
#include"construct.h"

void lib_print(lib_log_level level,const char*str){
	(void)level;
	fprintf(stdout,"%s\r\n",str);
}

int main(int argc,char**argv){
	int r;
	devcfg*cfg;
	run_constructor();
	atexit(run_destructor);
	if(argc!=2)r_err(-1,_("Usage: qc-devcfg-parser <DEVCFG>"));
	if(!(cfg=devcfg_new()))return -1;
	if((r=devcfg_load_path(cfg,argv[1]))<0)goto fail;
	if((r=devcfg_dump(cfg,0))<0)goto fail;
	r=0;
	fail:
	devcfg_unref(cfg);
	return r;
}
