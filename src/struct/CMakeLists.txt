add_library(
	qc_devcfg_parser_struct
	STATIC
	struct.c
	pmic.c
	i2c_config.c
)

if(ENABLE_LTO)
	set_target_properties(
		qc_devcfg_parser_struct PROPERTIES
		INTERPROCEDURAL_OPTIMIZATION True
	)
endif()
