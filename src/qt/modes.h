//
// Created by bigfoot on 2023/7/30.
//

#ifndef MODES_H
#define MODES_H

enum view_mode{
	VIEW_NONE,
	VIEW_NUM             = 0x00100000,

	VIEW_NUM_BASE_BOOL   = 0x00000001,
	VIEW_NUM_BASE_BIN    = 0x00000002,
	VIEW_NUM_BASE_OCT    = 0x00000004,
	VIEW_NUM_BASE_DEC    = 0x00000008,
	VIEW_NUM_BASE_HEX    = 0x00000010,
	VIEW_NUM_BASE_MASK   = 0x000000FF,

	VIEW_NUM_SIZE_8BIT   = 0x00000100,
	VIEW_NUM_SIZE_16BIT  = 0x00000200,
	VIEW_NUM_SIZE_32BIT  = 0x00000400,
	VIEW_NUM_SIZE_64BIT  = 0x00000800,
	VIEW_NUM_SIZE_MASK   = 0x0000FF00,

	VIEW_NUM_SIGNED      = 0x00010000,
	VIEW_NUM_UNSIGNED    = 0x00020000,
	VIEW_NUM_SINGLE      = 0x00040000,
	VIEW_NUM_ARRAY       = 0x00080000,

	#define DECL_NUM(_base,_size) VIEW_NUM | VIEW_NUM_BASE_##_base | VIEW_NUM_SIZE_##_size##BIT
	#define DECL_NUM_TYPE(_base,_size)\
		VIEW_NUM_UINT##_size##_##_base = DECL_NUM(_base,_size) | VIEW_NUM_SINGLE | VIEW_NUM_UNSIGNED, \
		VIEW_NUM_ARRAY_UINT##_size##_##_base = DECL_NUM(_base,_size) | VIEW_NUM_ARRAY | VIEW_NUM_UNSIGNED,
	#define DECL_NUM_BASE(_size)\
		VIEW_NUM_INT##_size##_DEC = DECL_NUM(DEC,_size) | VIEW_NUM_SINGLE | VIEW_NUM_SIGNED, \
		VIEW_NUM_ARRAY_INT##_size##_DEC = DECL_NUM(DEC,_size) | VIEW_NUM_ARRAY | VIEW_NUM_SIGNED, \
		DECL_NUM_TYPE(BOOL,_size)\
		DECL_NUM_TYPE(BIN,_size)\
		DECL_NUM_TYPE(OCT,_size)\
		DECL_NUM_TYPE(DEC,_size)\
		DECL_NUM_TYPE(HEX,_size)
	DECL_NUM_BASE(8)
	DECL_NUM_BASE(16)
	DECL_NUM_BASE(32)
	DECL_NUM_BASE(64)
	#undef DECL_NUM
	#undef DECL_NUM_TYPE
	#undef DECL_NUM_BASE
	VIEW_NUM_END         = 0x001FFFFF,
	VIEW_BYTES           = 0x00200000,
	VIEW_STRING_UTF8     = 0x00400000,
	VIEW_FILE_OFFSET     = 0x00800000,
	VIEW_LOAD_ADDRESS    = 0x01000000,
};

struct view_mode_list{
	devcfg_prop_type type;
	view_mode def;
	view_mode modes[126];
};

struct view_mode_item{
	view_mode mode;
	const char*name;
};

extern struct view_mode_list view_modes[];
extern struct view_mode_item view_items[];

#endif
