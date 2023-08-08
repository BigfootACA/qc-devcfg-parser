//
// Created by bigfoot on 2023/7/27.
//

#include<stdint.h>
#include"language.h"
#include"defines.h"

#ifndef MO_PATH
#define MO_PATH "."
#endif

extern binary_data _mo_file_en_US;
extern binary_data _mo_file_zh_CN;

language languages[]={
	{
		.id    = "en_US.UTF-8",
		.name  = "English",
		.def   = true,
		.match = (const char*[]){
			"en_US.UTF-8",
			"en_US",
			"en-US",
			"en.UTF-8",
			"en",
			NULL
		},
		.lang_id = 0x409,
		.data=&_mo_file_en_US,
	},{
		.id    = "zh_CN.UTF-8",
		.name  = "简体中文",
		.def   = false,
		.match = (const char*[]){
			"zh_CN.UTF-8",
			"zh_CN",
			"zh-CN",
			"zh.UTF-8",
			"zh",
			NULL
		},
		.lang_id = 0x0804,
		.data = &_mo_file_zh_CN,
	},
	{NULL,NULL,NULL,0,false,NULL}
};
