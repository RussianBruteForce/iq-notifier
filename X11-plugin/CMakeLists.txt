set(PLUGIN_NAME X11-plugin)

include_directories(..)

aux_source_directory(. X11_SRC_LIST)

add_library(${PLUGIN_NAME} ${X11_SRC_LIST})

target_link_libraries(${PLUGIN_NAME} X11)
