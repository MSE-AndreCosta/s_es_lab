set(LV_BUILD_CONF_PATH "${CMAKE_CURRENT_LIST_DIR}/lv_conf.h" CACHE PATH "" FORCE)
set(LV_BUILD_SET_CONFIG_OPTS ON CACHE BOOL "" FORCE)
include(lvgl)

add_subdirectory(${REPO_ROOT}/desktop_ui ui)

find_package(CURL REQUIRED)

add_executable(raspi_controller controller.c
        receiver.c
	ui_interface.c
	camera.c
	${REPO_ROOT}/mjpeg/mjpeg.c
	${REPO_ROOT}/protocol/protocol.c
	${REPO_ROOT}/net/net_udp.c
	${REPO_ROOT}/msgq/msgq.c
)

target_link_libraries(raspi_controller PRIVATE lib-ui CURL::libcurl m)
target_include_directories(raspi_controller PRIVATE ${REPO_ROOT})
link_with_cjson(raspi_controller)
