cmake_path(GET CMAKE_CURRENT_SOURCE_DIR PARENT_PATH PARENT_DIR)

add_executable(app_main main.c)

target_sources(app_main
     PUBLIC
        FILE_SET HEADERS
        BASE_DIRS
            ${PARENT_DIR}/tusb_src
            ${PARENT_DIR}/file_processing
            ${PARENT_DIR}/hardware_processing
            ${PICO_TINYUSB_PATH}/hw
        FILES
            ${PARENT_DIR}/hardware_processing/queue.h
            ${PARENT_DIR}/file_processing/file_processing.h
            ${PICO_TINYUSB_PATH}/hw/bsp/board_api.h)

## target_include_directories(${EXECUTABLE_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
##target_compile_definitions(app_main PUBLIC CF:eG_TUH_ENABLED=1)



target_link_libraries(app_main PRIVATE tinyusb_host file_processing host_storage pico_multicore hardware_clocks file_processing common_headers)
# target_include_directories(hardware_processing
#     PUBLIC
#         ${CMAKE_CURRENT_SOURCE_DIR}
#         ${PARENT_DIR}/Pico-PIO-USB/src
# )

# set_target_properties(${lib_name} PROPERTIES
#     VS_DEBUGGER_COMMAND $<TARGET_FILE:app_main>
#     VS_DEBUGGER_COMMAND_ARGUMENTS ""
# )


message(STATUS "Linked libs for ${lib_name}: ${LINK_LIBS}")
 