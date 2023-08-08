set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

set(SOURCES
        main.cpp
	log.cpp
	about.cpp
	parser.cpp
	parser_events.cpp
	parser_value.cpp
	parser_list.cpp
	parser_gui.cpp
	struct.cpp
	modes.cpp
	../../res/qt.qrc
)

if(WIN32)
	list(APPEND SOURCES ../../res/windows.rc)
endif()

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
	qt_add_executable(
		qc_devcfg_parser_qt
		MANUAL_FINALIZATION
		${SOURCES}
	)
else()
	add_executable(
		qc_devcfg_parser_qt
		${SOURCES}
	)
endif()

target_link_libraries(qc_devcfg_parser_qt PRIVATE -Wl,--start-group -Wl,--whole-archive)
target_link_libraries(
	qc_devcfg_parser_qt
	PRIVATE
	qc_devcfg_parser_lib
	qc_devcfg_parser_devcfg
	qc_devcfg_parser_struct
	qc_devcfg_parser_po
)
target_link_libraries(qc_devcfg_parser_qt PRIVATE -Wl,--no-whole-archive -Wl,--end-group)

if(ENABLE_STATIC)
	target_link_options(qc_devcfg_parser_qt PRIVATE -static)
endif()

target_link_libraries(
	qc_devcfg_parser_qt
	PRIVATE
	Qt${QT_VERSION_MAJOR}::Widgets
)

set_target_properties(
	qc_devcfg_parser_qt PROPERTIES
	MACOSX_BUNDLE_GUI_IDENTIFIER classfun.cn
	MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
	MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
	MACOSX_BUNDLE TRUE
	WIN32_EXECUTABLE TRUE
)

if(ENABLE_LTO)
	set_target_properties(
		qc_devcfg_parser_qt PROPERTIES
		INTERPROCEDURAL_OPTIMIZATION True
	)
endif()

if(QT_VERSION_MAJOR EQUAL 6)
	qt_finalize_executable(qc_devcfg_parser_qt)
endif()
