include(libgpiod)

add_executable(server server.c 
	${REPO_ROOT}/protocol/protocol.c 
	${REPO_ROOT}/net/local_ip.c 
	${REPO_ROOT}/net/net_udp.c 
	${REPO_ROOT}/broadcast/broadcast.c
	${REPO_ROOT}/transmitter/transmitter.c
	${REPO_ROOT}/msgq/msgq.c
	${REPO_ROOT}/receiver/receiver.c
	${REPO_ROOT}/led/led.c
	${REPO_ROOT}/joystick/joystick.c
	${REPO_ROOT}/sht/sht.c
	${REPO_ROOT}/reader/reader.c
	${REPO_ROOT}/game/game.c
)

target_include_directories(server PRIVATE ${REPO_ROOT})
link_with_cjson(server)
target_link_libraries(server PRIVATE gpiod m)
