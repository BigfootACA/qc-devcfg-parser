set(languages
	zh_CN
	en_US
)
set(asm_files)
find_program(SED_EXECUTABLE sed)
find_program(MSGFMT_EXECUTABLE msgfmt)
foreach(lang ${languages})
	set(po_file ${CMAKE_CURRENT_SOURCE_DIR}/${lang}.po)
	set(gmo_file ${CMAKE_CURRENT_BINARY_DIR}/${lang}.gmo)
	set(asm_file ${CMAKE_CURRENT_BINARY_DIR}/mo_file_${lang}.s)
	add_custom_command(
		OUTPUT ${gmo_file}
		COMMAND ${MSGFMT_EXECUTABLE}
			-o ${gmo_file} ${po_file}
		DEPENDS ${po_file}
	)
	add_custom_command(
		OUTPUT ${asm_file}
		COMMAND ${SED_EXECUTABLE}
			-e s@%FILE%@${gmo_file}@g
			-e s@%SYMBOL%@_mo_file_${lang}@g
			${INCBIN_FILE} > ${asm_file}
		DEPENDS ${gmo_file}
	)
	list(APPEND asm_files "${asm_file}")
endforeach()
add_library(qc_devcfg_parser_po STATIC ${asm_files} languages.c)
target_compile_definitions(qc_devcfg_parser_po PRIVATE -DMO_PATH=\"${CMAKE_CURRENT_BINARY_DIR}\")
