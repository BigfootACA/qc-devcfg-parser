#ifndef _STR_H
#define _STR_H
#ifdef __cplusplus
extern "C"{
#endif
#include<inttypes.h>
#include"devcfg.h"
extern const char*elf_pt_to_symbol(uint32_t p_type,const char*def);
extern const char*elf_pt_to_readable(uint32_t p_type,const char*def);
extern const char*devcfg_prop_type_to_symbol(devcfg_prop_type type,const char*def);
extern const char*devcfg_prop_type_to_readable(devcfg_prop_type type,const char*def);
#ifdef __cplusplus
}
#endif
#endif
