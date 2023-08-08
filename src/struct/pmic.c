//
// Created by bigfoot on 2023/8/3.
//

// TODO: implement struct viewer
#include"internal.h"

typedef enum pm_periph_type{
	PM_PERIPH_TYPE_LDO,
	PM_PERIPH_TYPE_SMPS,
	PM_PERIPH_TYPE_FULL_PERI_REG,
	PM_PERIPH_TYPE_SINGLE_REG,
	PM_PERIPH_TYPE_INVALID,
}pm_periph_type;

typedef struct pm_reg_dump_list_type{
	uint32_t slave_id;
	uint32_t base_addr;
	pm_periph_type periph;
}pm_reg_dump_list_type;


static devcfg_struct struct_pm_reg_dump_list_type={
	.name  = "pm_reg_dump_list_type",
	.size  = sizeof(pm_reg_dump_list_type),
	.items = (devcfg_struct_item[]){
		STRUCT_ITEM_SINGLE(pm_reg_dump_list_type,TYPE_UINT32,slave_id,),
		STRUCT_ITEM_SINGLE(pm_reg_dump_list_type,TYPE_UINT32,base_addr,),
		STRUCT_ITEM_SINGLE(pm_reg_dump_list_type,TYPE_UINT32,periph,),
		STRUCT_ITEM_EMPTY
	},
};

static ssize_t pm_reg_dump_guess_size(
	devcfg_struct_match*st,
	devcfg_prop*prop,
	uintptr_t ptr
){
	size_t count=0;
	pm_reg_dump_list_type one;
	devcfg*cfg=devcfg_prop_get_devcfg(prop);
	while(true){
		if(!devcfg_read(cfg,ptr,&one,sizeof(one),false))break;
		if(one.periph==PM_PERIPH_TYPE_INVALID)break;
		count++,ptr+=sizeof(one);
	}
	return (ssize_t)st->type.field*count;
}

static devcfg_struct_match pm_reg_dump_match={
	.device_regex="^/tz/pmic$",
	.prop_regex="^reg_dump_list$",
	.type={
		.name="pm_reg_dump_list_type",
		.type=TYPE_STRUCT,
		.pointer=true,
		.array=true,
		.size=sizeof(void*),
		.field=sizeof(pm_reg_dump_list_type),
		.use_struct=&struct_pm_reg_dump_list_type,
	},
	.guess_size=pm_reg_dump_guess_size,
};

DECL_STRUCT_MATCH(pm_reg_dump_match);
