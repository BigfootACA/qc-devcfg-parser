//
// Created by bigfoot on 2023/7/26.
//

#include"parser.h"
#include"defines.h"
#include"modes.h"

#define DECL_NUM(_prefix,_base) \
	VIEW_NUM_##_prefix##UINT##_base##_BOOL,\
	VIEW_NUM_##_prefix##UINT##_base##_BIN,\
	VIEW_NUM_##_prefix##UINT##_base##_OCT,\
	VIEW_NUM_##_prefix##INT##_base##_DEC,\
	VIEW_NUM_##_prefix##UINT##_base##_DEC,\
	VIEW_NUM_##_prefix##UINT##_base##_HEX

#define DECL_NUM_TYPE_SINGLE(_base) DECL_NUM(,_base)
#define DECL_NUM_TYPE_ARRAY(_base)  DECL_NUM(ARRAY_,_base)

#define DECL_NUM_SINGLE \
	DECL_NUM_TYPE_SINGLE(8),\
	DECL_NUM_TYPE_SINGLE(16),\
	DECL_NUM_TYPE_SINGLE(32),\
	DECL_NUM_TYPE_SINGLE(64)

#define DECL_NUM_ARRAY \
	DECL_NUM_TYPE_ARRAY(8),\
	DECL_NUM_TYPE_ARRAY(16),\
	DECL_NUM_TYPE_ARRAY(32),\
	DECL_NUM_TYPE_ARRAY(64)

struct view_mode_list view_modes[]={
	{
		.type=DEVCFG_PROP_UINT32,
		.def=VIEW_NUM_UINT32_DEC,
		.modes={
			VIEW_BYTES,
			DECL_NUM_SINGLE,
			VIEW_FILE_OFFSET,
			VIEW_LOAD_ADDRESS,
			VIEW_NONE
		}
	},{
		.type=DEVCFG_PROP_UINT32_ARRAY_PTR,
		.def=VIEW_NUM_ARRAY_UINT32_DEC,
		.modes={
			VIEW_BYTES,
			DECL_NUM_ARRAY,
			VIEW_FILE_OFFSET,
			VIEW_LOAD_ADDRESS,
			VIEW_NONE
		}
	},{
		.type=DEVCFG_PROP_STRING_PTR,
		.def=VIEW_STRING_UTF8,
		.modes={
			VIEW_BYTES,
			VIEW_STRING_UTF8,
			VIEW_FILE_OFFSET,
			VIEW_LOAD_ADDRESS,
			VIEW_NONE
		}
	},{
		.type=DEVCFG_PROP_BYTE_ARRAY,
		.def=VIEW_BYTES,
		.modes={
			VIEW_BYTES,
			VIEW_STRING_UTF8,
			DECL_NUM_SINGLE,
			DECL_NUM_ARRAY,
			VIEW_FILE_OFFSET,
			VIEW_LOAD_ADDRESS,
			VIEW_NONE
		}
	},{
		.type=DEVCFG_PROP_BYTE_ARRAY_PTR,
		.def=VIEW_BYTES,
		.modes={
			VIEW_BYTES,
			VIEW_STRING_UTF8,
			DECL_NUM_SINGLE,
			DECL_NUM_ARRAY,
			VIEW_FILE_OFFSET,
			VIEW_LOAD_ADDRESS,
			VIEW_NONE
		}
	},{
		.type=DEVCFG_PROP_STRUCT_PTR,
		.def=VIEW_BYTES,
		.modes={
			VIEW_BYTES,
			VIEW_STRING_UTF8,
			DECL_NUM_SINGLE,
			DECL_NUM_ARRAY,
			VIEW_FILE_OFFSET,
			VIEW_LOAD_ADDRESS,
			VIEW_NONE
		}
	},
	{DEVCFG_PROP_UNKNOWN,VIEW_NONE,{VIEW_NONE}},
};

#undef _
#define _(text) (const char*)(text)
// tell gettext string are translatable

struct view_mode_item view_items[]={
	{VIEW_STRING_UTF8,           _("UTF8 String")},
	{VIEW_BYTES,                 _("Binary Data")},
	{VIEW_FILE_OFFSET,           _("File Offset")},
	{VIEW_LOAD_ADDRESS,          _("Load Address")},
	{VIEW_NUM_INT8_DEC,          _("8-bit Decimal number")},
	{VIEW_NUM_INT16_DEC,         _("16-bit Decimal number")},
	{VIEW_NUM_INT32_DEC,         _("32-bit Decimal number")},
	{VIEW_NUM_INT64_DEC,         _("64-bit Decimal number")},
	{VIEW_NUM_UINT8_BIN,         _("Unsigned 8-bit Binary number")},
	{VIEW_NUM_UINT8_OCT,         _("Unsigned 8-bit Octal number")},
	{VIEW_NUM_UINT8_HEX,         _("Unsigned 8-bit Hexadecimal number")},
	{VIEW_NUM_UINT8_DEC,         _("Unsigned 8-bit Decimal number")},
	{VIEW_NUM_UINT16_BOOL,       _("Unsigned 16-bit Boolean")},
	{VIEW_NUM_UINT16_BIN,        _("Unsigned 16-bit Binary number")},
	{VIEW_NUM_UINT16_OCT,        _("Unsigned 16-bit Octal number")},
	{VIEW_NUM_UINT16_HEX,        _("Unsigned 16-bit Hexadecimal number")},
	{VIEW_NUM_UINT16_DEC,        _("Unsigned 16-bit Decimal number")},
	{VIEW_NUM_UINT32_BOOL,       _("Unsigned 32-bit Boolean")},
	{VIEW_NUM_UINT32_BIN,        _("Unsigned 32-bit Binary number")},
	{VIEW_NUM_UINT32_OCT,        _("Unsigned 32-bit Octal number")},
	{VIEW_NUM_UINT32_HEX,        _("Unsigned 32-bit Hexadecimal number")},
	{VIEW_NUM_UINT32_DEC,        _("Unsigned 32-bit Decimal number")},
	{VIEW_NUM_UINT64_BOOL,       _("Unsigned 64-bit Boolean")},
	{VIEW_NUM_UINT64_BIN,        _("Unsigned 64-bit Binary number")},
	{VIEW_NUM_UINT64_OCT,        _("Unsigned 64-bit Octal number")},
	{VIEW_NUM_UINT64_HEX,        _("Unsigned 64-bit Hexadecimal number")},
	{VIEW_NUM_UINT64_DEC,        _("Unsigned 64-bit Decimal number")},
	{VIEW_NUM_ARRAY_INT8_DEC,    _("8-bit Decimal number array")},
	{VIEW_NUM_ARRAY_INT16_DEC,   _("16-bit Decimal number array")},
	{VIEW_NUM_ARRAY_INT32_DEC,   _("32-bit Decimal number array")},
	{VIEW_NUM_ARRAY_INT64_DEC,   _("64-bit Decimal number array")},
	{VIEW_NUM_ARRAY_UINT8_BIN,   _("Unsigned 8-bit Binary number array")},
	{VIEW_NUM_ARRAY_UINT8_OCT,   _("Unsigned 8-bit Octal number array")},
	{VIEW_NUM_ARRAY_UINT8_HEX,   _("Unsigned 8-bit Hexadecimal number array")},
	{VIEW_NUM_ARRAY_UINT8_DEC,   _("Unsigned 8-bit Decimal number array")},
	{VIEW_NUM_ARRAY_UINT16_BOOL, _("Unsigned 16-bit Boolean array")},
	{VIEW_NUM_ARRAY_UINT16_BIN,  _("Unsigned 16-bit Binary number array")},
	{VIEW_NUM_ARRAY_UINT16_OCT,  _("Unsigned 16-bit Octal number array")},
	{VIEW_NUM_ARRAY_UINT16_HEX,  _("Unsigned 16-bit Hexadecimal number array")},
	{VIEW_NUM_ARRAY_UINT16_DEC,  _("Unsigned 16-bit Decimal number array")},
	{VIEW_NUM_ARRAY_UINT32_BOOL, _("Unsigned 32-bit Boolean array")},
	{VIEW_NUM_ARRAY_UINT32_BIN,  _("Unsigned 32-bit Binary number array")},
	{VIEW_NUM_ARRAY_UINT32_OCT,  _("Unsigned 32-bit Octal number array")},
	{VIEW_NUM_ARRAY_UINT32_HEX,  _("Unsigned 32-bit Hexadecimal number array")},
	{VIEW_NUM_ARRAY_UINT32_DEC,  _("Unsigned 32-bit Decimal number array")},
	{VIEW_NUM_ARRAY_UINT64_BOOL, _("Unsigned 64-bit Boolean array")},
	{VIEW_NUM_ARRAY_UINT64_BIN,  _("Unsigned 64-bit Binary number array")},
	{VIEW_NUM_ARRAY_UINT64_OCT,  _("Unsigned 64-bit Octal number array")},
	{VIEW_NUM_ARRAY_UINT64_HEX,  _("Unsigned 64-bit Hexadecimal number array")},
	{VIEW_NUM_ARRAY_UINT64_DEC,  _("Unsigned 64-bit Decimal number array")},
	{VIEW_NONE,                  nullptr},
};
