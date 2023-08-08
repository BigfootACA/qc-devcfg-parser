//
// Created by bigfoot on 2023/8/1.
//

// TODO: implement struct viewer
#ifndef INTERNAL_H
#define INTERNAL_H
#include"devcfg.h"
#include"construct.h"
typedef struct devcfg_struct_match devcfg_struct_match;
typedef ssize_t(*devcfg_struct_guess_size)(
	devcfg_struct_match*st,
	devcfg_prop*prop,
	uintptr_t ptr
);
struct devcfg_struct_match{
	char*device_regex;
	char*prop_regex;
	devcfg_struct_item type;
	devcfg_struct_guess_size guess_size;
};
#define STRUCT_ITEM_EMPTY {NULL,TYPE_NONE,false,false,false,0,0,0,NULL}
#define STRUCT_ITEM_SINGLE(_struct,_type,_field,...) {\
	.name    = #_field,\
	.type    = _type,\
	.offset  = offsetof(_struct,_field),\
	.size    = sizeof(((_struct*)0)->_field),\
	.field   = sizeof(((_struct*)0)->_field),\
	__VA_ARGS__\
}
#define DECL_STRUCT_MATCH(_match) DECL_CONSTRUCT_DATA(_match,STRUCT_MATCH,&_match)
#endif
