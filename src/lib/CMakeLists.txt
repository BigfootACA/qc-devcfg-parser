add_library(
	qc_devcfg_parser_lib
	STATIC
	constructor.c
	vsnlprintf.c
	snlprintf.c
	language.c
	strndup.c
	strlcat.c
	strlcpy.c
	memdup.c
	regexp.c
	list.c
	str.c
	log.c
	io.c
)

if(ENABLE_LTO)
	set_target_properties(
		qc_devcfg_parser_lib PROPERTIES
		INTERPROCEDURAL_OPTIMIZATION True
	)
endif()
