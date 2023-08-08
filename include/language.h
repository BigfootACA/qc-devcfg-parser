//
// Created by bigfoot on 2023/7/26.
//

#ifndef _LANGUAGE_H
#define _LANGUAGE_H
#ifdef __cplusplus
extern "C"{
#endif
#include"defines.h"
#include<stdbool.h>
typedef struct language language;
typedef void(*language_callback)(
	void*data,
	language*old_lang,
	language*new_lang
);
struct language{
	const char*id;
	const char*name;
	const char**match;
	uint16_t lang_id;
	bool def;
	const binary_data*data;
};

extern language languages[];
extern void language_init(void);
extern language*language_get_current(void);
extern int language_set(language*lang);
extern int language_add_callback(language_callback cb,void*data);
extern int language_del_callback(language_callback cb,void*data);

#ifdef __cplusplus
}
#endif
#endif
