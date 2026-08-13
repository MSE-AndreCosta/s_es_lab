set(LV_BUILD_CONF_PATH "${CMAKE_CURRENT_LIST_DIR}/lv_conf.h" CACHE PATH "" FORCE)
set(LV_BUILD_SET_CONFIG_OPTS ON CACHE BOOL "" FORCE)

include(lvgl)

add_executable(image_acquisition image_acquisition.c)
target_link_libraries(image_acquisition PRIVATE lvgl::lvgl)

