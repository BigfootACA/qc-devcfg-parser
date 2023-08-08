//
// Created by bigfoot on 2023/7/22.
//

#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<stdint.h>
#include"defines.h"
#include"io.h"

static int _io_read_file(FILE*in,size_t max,void**data,size_t*size){
	clear_error();
	if(!in||!data||!size||max<=0)return -1;
	*size=0,*data=NULL;
	fseek(in,0,SEEK_END);
	*size=ftell(in);
	fseek(in,0,SEEK_SET);
	clear_error();
	if(*size<=0)go_err(_("empty file"));
	if(*size>=max)go_err(_("file too large (%zu bytes)"),*size);
	*data=malloc(*size);
	if(!*data)go_err(_("allocate %zu bytes failed"),*size);
	memset(*data,0,*size);
	if(fread(*data,*size,1,in)!=1)
		go_err(_("read %zu bytes failed"),*size);
	fseek(in,0,SEEK_SET);
	clear_error();
	return 0;
	fail:
	if(*data)free(*data);
	*data=0,*size=0;
	return -1;
}

int io_read_file(FILE*in,size_t max,void**data,size_t*size){
	int r=_io_read_file(in,max,data,size);
	if(r==0)warn(_("read file with %p failed"),in);
	else dbg(_("read file with %p size %zu bytes to %p"),in,*size,*data);
	return r;
}

int io_read_path(const char*path,size_t max,void**data,size_t*size){
	int r;
	FILE*f;
	clear_error();
	if(!path||max<=0)return -1;
	if(!(f=fopen(path,"rb")))r_err(-1,_("open %s failed"),path);
	r=_io_read_file(f,max,data,size);
	if(r!=0)warn(_("read file %s failed"),path);
	else dbg(_("read file %s size %zu bytes to %p"),path,*size,*data);
	fclose(f);
	return r;
}
