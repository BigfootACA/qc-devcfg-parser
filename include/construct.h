//
// Created by bigfoot on 2023/5/17.
//

#ifndef CONSTRUCT_H
#define CONSTRUCT_H
#ifdef __cplusplus
extern "C"{
#endif
typedef void(*constructor)(void);
typedef enum construct_type{
	CONSTRUCT_FUNC_CONSTRUCTOR = 0x10000001,
	CONSTRUCT_FUNC_DESTRUCTOR  = 0x10000002,
	CONSTRUCT_STRUCT_MATCH     = 0x10000003,
	SIMPLE_CONSTRUCT_TYPE_MAX  = 0xFFFFFFFFFFFFFFFFULL,
}construct_type;
typedef struct construct_data{
	construct_type type:64;
	void*value;
}construct_data;
extern void run_constructor(void);
extern void run_destructor(void);
extern construct_data __start_constructor[],__stop_constructor[];
#define CONSTRUCT_DATA(_data,_type) ((_type)((_data).data->value))
#define CONSTRUCT_DATA_DECLARE(_name,_data,_type) _type (_name)=((_data).data->value)
#define FOREACH_CONSTRUCT(_type,_loop)for(\
	struct{size_t i;construct_data*data;}(_loop)={0,NULL};\
	(_loop).i<(size_t)(__stop_constructor-__start_constructor)&&\
	((_loop).data=&__start_constructor[(_loop).i]);\
	(_loop).i++\
)if((_loop).data->type==(_type))
#define DECL_CONSTRUCT_DATA(_name,_type,_value) \
	const __attribute__((section("constructor"),used)) \
	construct_data _construct_##_name={.type=CONSTRUCT_##_type,.value=(void*)(_value)}
#define DECL_CONSTRUCTOR(_func) DECL_CONSTRUCT_DATA(_func,FUNC_CONSTRUCTOR,_func)
#define DECL_DESTRUCTOR(_func)  DECL_CONSTRUCT_DATA(_func,FUNC_DESTRUCTOR,_func)
#undef DECL_FUNC
#ifdef __cplusplus
}
#endif
#endif
