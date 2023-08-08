add_library(
	qc_devcfg_parser_devcfg
	STATIC
	io.c
	loader.c
	dump.c
	devcfg.c
	section.c
	target.c
	device.c
	prop.c
	value.c
)

if(ENABLE_LTO)
	set_target_properties(
		qc_devcfg_parser_devcfg PROPERTIES
		INTERPROCEDURAL_OPTIMIZATION True
	)
endif()
