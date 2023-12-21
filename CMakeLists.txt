cmake_minimum_required(VERSION 3.10)
project(PddAuthCode)

#设置输出目录

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/output/lib")#静态库
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/output/bin")#动态库
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/output/bin")#可执行文件

#设置启动项
set_property ( DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME} )

#QT开启自动编译
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /Zi")
set(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} /DEBUG /OPT:REF /OPT:ICF")
set(CMAKE_SHARED_LINKER_FLAGS_RELEASE "${CMAKE_SHARED_LINKER_FLAGS_RELEASE} /DEBUG /OPT:REF /OPT:ICF")

#-----------------------------------------------------------------------------------------------------------------------------------------
# 创建 public_include 指向的文件夹
#set(public_include ${CMAKE_CURRENT_SOURCE_DIR}/public)
#execute_process( COMMAND ${CMAKE_COMMAND} -E make_directory ${public_include})

# 拷贝 src文件夹下的内容  到  save_src_dir 中 
#set(save_src_dir ${CMAKE_CURRENT_SOURCE_DIR}/output)
#execute_process( COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/src ${save_src_dir})

# 拷贝 main.cpp 到 public_include 中
#set(public_include ${CMAKE_CURRENT_SOURCE_DIR}/public)
#execute_process( COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp ${public_include})
#------------------------------------------------------------------------------------------------------------------------------------------


file(GLOB_RECURSE SRC_FILE
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
)

file (GLOB_RECURSE INC_FILE
   "${CMAKE_CURRENT_SOURCE_DIR}/src/*.h"
)

file (GLOB_RECURSE RESOURCE_FILE
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.ui"
)

source_group(TREE ${CMAKE_SOURCE_DIR}/src FILES ${SRC_FILE})
source_group(TREE ${CMAKE_SOURCE_DIR}/src FILES ${INC_FILE})
source_group(forms ${CMAKE_SOURCE_DIR}/src FILES ${RESOURCE_FILE})


if(WIN32)
	# 整理自动生成的文件
	source_group("CMake Rules"  REGULAR_EXPRESSION "^$")
	source_group("Header Files" REGULAR_EXPRESSION "^$")
	source_group("Source Files" REGULAR_EXPRESSION "^$")
	source_group("moc"			REGULAR_EXPRESSION "\\.h$|\\.cpp$|\\.stamp$|\\.rule$")
endif()


add_executable(${PROJECT_NAME} WIN32
 ${SRC_FILE}
 ${INC_FILE}
 ${RESOURCE_FILE}
 ${CMAKE_CURRENT_SOURCE_DIR}/src/pdd_auth_code.qrc
)

add_compile_options("$<$<C_COMPILER_ID:MSVC>:/utf-8>")
add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")
	
# 链接可执行文件
find_package(Qt6 COMPONENTS Widgets REQUIRED)
find_package(Qt6 COMPONENTS Core REQUIRED)
find_package(Qt6 COMPONENTS Network REQUIRED)


target_link_libraries(${PROJECT_NAME} 
	Qt6::Widgets
	Qt6::Core
	Qt6::Network
	)
	

#编译完成后拷贝对应依赖库	
get_target_property(_qmake_executable Qt6::qmake IMPORTED_LOCATION)
get_filename_component(_qt_bin_dir "${_qmake_executable}" DIRECTORY)
find_program(DEPLOYQT_EXECUTABLE NAMES windeployqt macdeployqt HINTS "${_qt_bin_dir}")
if(APPLE)
    configure_file(${CMAKE_SOURCE_DIR}/Info.plist.in ${CMAKE_BINARY_DIR}/Info.plist)
    set_target_properties(${PROJECT_NAME} PROPERTIES MACOSX_BUNDLE_INFO_PLIST ${CMAKE_BINARY_DIR}/Info.plist)
    add_custom_command(TARGET ${PROJECT_NAME}
        POST_BUILD
        COMMAND ${DEPLOYQT_EXECUTABLE} "$<TARGET_BUNDLE_DIR:${PROJECT_NAME}>" -qmldir=${CMAKE_SOURCE_DIR}
    )
elseif(WIN32)
    add_custom_command(TARGET ${PROJECT_NAME}
        POST_BUILD
        COMMAND ${DEPLOYQT_EXECUTABLE} "$<TARGET_FILE:${PROJECT_NAME}>"
    )
endif()


if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    message("build debug")
else()
    # 添加优化选项
endif()