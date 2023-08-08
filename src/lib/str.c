//
// Created by bigfoot on 2023/7/22.
//

#include<elf.h>
#include"str.h"
#include"defines.h"

const char*elf_pt_to_symbol(uint32_t p_type,const char*def){
	switch(p_type){
		case PT_NULL:return "NULL";
		case PT_LOAD:return "LOAD";
		case PT_DYNAMIC:return "DYNAMIC";
		case PT_INTERP:return "INTERP";
		case PT_NOTE:return "NOTE";
		case PT_SHLIB:return "SHLIB";
		case PT_PHDR:return "PHDR";
		case PT_TLS:return "TLS";
		#ifdef PT_GNU_EH_FRAME
		case PT_GNU_EH_FRAME:return "GNU_EH_FRAME";
		#endif
		#ifdef PT_GNU_STACK
		case PT_GNU_STACK:return "GNU_STACK";
		#endif
		#ifdef PT_GNU_RELRO
		case PT_GNU_RELRO:return "GNU_RELRO";
		#endif
		#ifdef PT_GNU_PROPERTY
		case PT_GNU_PROPERTY:return "GNU_PROPERTY";
		#endif
		#ifdef PT_SUNWBSS
		case PT_SUNWBSS:return "SUNWBSS";
		#endif
		#ifdef PT_SUNWSTACK
		case PT_SUNWSTACK:return "SUNWSTACK";
		#endif
		default:return def;
	}
}

const char*elf_pt_to_readable(uint32_t p_type,const char*def){
	switch(p_type){
		case PT_NULL:return _("Null");
		case PT_LOAD:return _("Loadable");
		case PT_DYNAMIC:return _("Dynamic");
		case PT_INTERP:return _("Interpreter");
		case PT_NOTE:return _("Note");
		case PT_SHLIB:return _("Shlib");
		case PT_PHDR:return _("Program header");
		case PT_TLS:return _("Thread-local storage");
		#ifdef PT_GNU_EH_FRAME
		case PT_GNU_EH_FRAME:return _("GNU EH_FRAME");
		#endif
		#ifdef PT_GNU_STACK
		case PT_GNU_STACK:return _("GNU Stack");
		#endif
		#ifdef PT_GNU_RELRO
		case PT_GNU_RELRO:return _("GNU RELRO");
		#endif
		#ifdef PT_GNU_PROPERTY
		case PT_GNU_PROPERTY:return _("GNU Property");
		#endif
		#ifdef PT_SUNWBSS
		case PT_SUNWBSS:return _("SUN BSS");
		#endif
		#ifdef PT_SUNWSTACK
		case PT_SUNWSTACK:return _("SUN Stack");
		#endif
		default:return def;
	}
}

const char*devcfg_prop_type_to_symbol(devcfg_prop_type type,const char*def){
	switch(type){
		case DEVCFG_PROP_STRING_PTR:return "STRING_PTR";
		case DEVCFG_PROP_UINT32:return "UINT32";
		case DEVCFG_PROP_BYTE_ARRAY:return "BYTE_SEQ";
		case DEVCFG_PROP_BYTE_ARRAY_PTR:return "BYTE_SEQ_PTR";
		case DEVCFG_PROP_UINT32_ARRAY_PTR:return "UINT32_SEQ_PTR";
		case DEVCFG_PROP_STRUCT_PTR:return "STRUCT_PTR";
		case DEVCFG_PROP_END:return "END";
		default:return def;
	}
}

const char*devcfg_prop_type_to_readable(devcfg_prop_type type,const char*def){
	switch(type){
		case DEVCFG_PROP_STRING_PTR:return _("String Pointer");
		case DEVCFG_PROP_UINT32:return _("UINT32");
		case DEVCFG_PROP_BYTE_ARRAY:return _("Byte Array");
		case DEVCFG_PROP_BYTE_ARRAY_PTR:return _("Byte Array Pointer");
		case DEVCFG_PROP_UINT32_ARRAY_PTR:return _("UINT32 Array Pointer");
		case DEVCFG_PROP_STRUCT_PTR:return _("Struct Pointer");
		case DEVCFG_PROP_END:return _("End");
		default:return def;
	}
}
