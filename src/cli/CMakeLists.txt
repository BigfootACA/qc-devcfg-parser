set(SOURCES main.c)

if(WIN32)
	list(APPEND SOURCES ../../res/windows.rc)
endif()

add_executable(qc_devcfg_parser_cli ${SOURCES})

target_link_libraries(qc_devcfg_parser_cli PRIVATE -Wl,--start-group -Wl,--whole-archive)
target_link_libraries(
	qc_devcfg_parser_cli
	PRIVATE
	qc_devcfg_parser_lib
	qc_devcfg_parser_devcfg
	qc_devcfg_parser_struct
	qc_devcfg_parser_po
)
target_link_libraries(qc_devcfg_parser_cli PRIVATE -Wl,--no-whole-archive -Wl,--end-group)

if(ENABLE_STATIC)
	target_link_options(qc_devcfg_parser_cli PRIVATE -static)
endif()

if(ENABLE_LTO)
	set_target_properties(
		qc_devcfg_parser_cli PROPERTIES
		INTERPROCEDURAL_OPTIMIZATION True
	)
endif()