//
// Created by bigfoot on 2023/8/1.
//

// TODO: implement struct viewer
#include"internal.h"

typedef struct num_cores{uint32_t num;}num_cores;

typedef struct plat_clock_config{
	uint32_t  se_clock_frequency_khz;
	uint32_t  bus_speed_khz;
	uint8_t   clk_div;
	uint8_t   t_cycle;
	uint8_t   t_high;
	uint8_t   t_low;
}plat_clock_config;

typedef struct plat_device_config{
	uint32_t           *core_base_addr;
	uint32_t           core_offset;
	uint8_t            qupv3_instance;
	uint8_t            core_index;
	uint16_t           core_irq;
	bool               polled_mode;
	uint16_t           min_data_length_for_dma;
	uint8_t            gpi_index;
	uint32_t           scl_encoding;
	uint32_t           sda_encoding;
	uint8_t           *tcsr_base_addr;
	uint32_t           tcsr_reg_offset;
	uint32_t           tcsr_reg_value;
	uint8_t          **common_clocks;
	uint8_t          **se_clock;
	uint8_t          **se_src_clock;
	uint32_t           resource_votes;
	plat_clock_config *clock_config;
}plat_device_config;

devcfg_struct struct_plat_clock_config={
	.name  = "plat_clock_config",
	.size  = sizeof(plat_clock_config),
	.items = (devcfg_struct_item[]){
		STRUCT_ITEM_SINGLE(plat_clock_config,TYPE_UINT32,se_clock_frequency_khz,),
		STRUCT_ITEM_SINGLE(plat_clock_config,TYPE_UINT32,bus_speed_khz,),
		STRUCT_ITEM_SINGLE(plat_clock_config,TYPE_UINT8,clk_div,),
		STRUCT_ITEM_SINGLE(plat_clock_config,TYPE_UINT8,t_cycle,),
		STRUCT_ITEM_SINGLE(plat_clock_config,TYPE_UINT8,t_high,),
		STRUCT_ITEM_SINGLE(plat_clock_config,TYPE_UINT8,t_low,),
		STRUCT_ITEM_EMPTY
	},
};

static devcfg_struct struct_plat_device_config={
	.name  = "plat_device_config",
	.size  = sizeof(plat_device_config),
	.items = (devcfg_struct_item[]){
		STRUCT_ITEM_SINGLE(plat_device_config,TYPE_UINTN,core_base_addr,),
		STRUCT_ITEM_SINGLE(plat_device_config,TYPE_UINT32,core_offset,),
		STRUCT_ITEM_SINGLE(plat_device_config,TYPE_UINT8,qupv3_instance,),
		STRUCT_ITEM_SINGLE(plat_device_config,TYPE_UINT8,core_index,),
		STRUCT_ITEM_SINGLE(plat_device_config,TYPE_UINT16,core_irq,),
		STRUCT_ITEM_SINGLE(plat_device_config,TYPE_BOOL,polled_mode,),
		STRUCT_ITEM_SINGLE(plat_device_config,TYPE_UINT16,min_data_length_for_dma,),
		STRUCT_ITEM_SINGLE(plat_device_config,TYPE_UINT8,gpi_index,),
		STRUCT_ITEM_SINGLE(plat_device_config,TYPE_UINT32,scl_encoding,),
		STRUCT_ITEM_SINGLE(plat_device_config,TYPE_UINT32,sda_encoding,),
		STRUCT_ITEM_SINGLE(plat_device_config,TYPE_UINTN,tcsr_base_addr,),
		STRUCT_ITEM_SINGLE(plat_device_config,TYPE_UINT32,tcsr_reg_offset,),
		STRUCT_ITEM_SINGLE(plat_device_config,TYPE_UINT32,tcsr_reg_value,),
		// FIXME
		STRUCT_ITEM_EMPTY
	},
};

static ssize_t i2c_config_guess_size(
	devcfg_struct_match*st,
	devcfg_prop*prop,
	uintptr_t ptr __attribute__((unused))
){
	num_cores cores={0};
	devcfg_device*dev=devcfg_prop_get_device(prop);
	devcfg_prop*nc=devcfg_device_get_prop_by_name(dev,"num_i2c_cores");
	if(!nc||!devcfg_prop_get_value_bytes_buf(
		nc,&cores,sizeof(cores),sizeof(cores)
	))cores.num=1;
	return (ssize_t)st->type.field*cores.num;
}

static devcfg_struct struct_num_cores={
	.name  = "num_cores",
	.size  = sizeof(num_cores),
	.items = (devcfg_struct_item[]){
		STRUCT_ITEM_SINGLE(num_cores,TYPE_UINT32,num,),
		STRUCT_ITEM_EMPTY
	},
};

static devcfg_struct_match i2c_config_match={
	.device_regex="^/dev/i2c$",
	.prop_regex="^i2c_device_config$",
	.type={
		.name="i2c_device_config",
		.type=TYPE_STRUCT,
		.pointer=true,
		.array=true,
		.size=sizeof(void*),
		.field=sizeof(plat_device_config),
		.use_struct=&struct_plat_device_config,
	},
	.guess_size=i2c_config_guess_size,
};

DECL_STRUCT_MATCH(i2c_config_match);

static devcfg_struct_match num_cores_match={
	.device_regex="^/dev/i2c$",
	.prop_regex="^num_i2c_cores$",
	.type={
		.name="num_i2c_cores",
		.type=TYPE_STRUCT,
		.pointer=true,
		.size=sizeof(void*),
		.field=sizeof(num_cores),
		.use_struct=&struct_num_cores,
	},
};

DECL_STRUCT_MATCH(num_cores_match);
