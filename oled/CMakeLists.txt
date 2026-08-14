set(MCULIB_DIR ${CMAKE_CURRENT_LIST_DIR}/McuLib)

set(MCULIB_SOURCES
    ${MCULIB_DIR}/src/McuLib.c
    ${MCULIB_DIR}/src/McuRTOS.c
    ${MCULIB_DIR}/src/McuWait.c
    ${MCULIB_DIR}/src/McuLog.c
    ${MCULIB_DIR}/src/McuGPIO.c
    ${MCULIB_DIR}/src/McuLED.c
    ${MCULIB_DIR}/src/McuButton.c
    ${MCULIB_DIR}/src/McuUtility.c
    ${MCULIB_DIR}/src/McuShell.c
    ${MCULIB_DIR}/src/McuXFormat.c
    ${MCULIB_DIR}/src/McuCriticalSection.c
    ${MCULIB_DIR}/src/McuGenericI2C.c
    ${MCULIB_DIR}/src/McuI2cLib.c
    ${MCULIB_DIR}/src/McuSHT31.c
    ${MCULIB_DIR}/src/McuSHT40.c
    ${MCULIB_DIR}/src/McuGDisplaySSD1306.c
    ${MCULIB_DIR}/src/McuFontDisplay.c
    ${MCULIB_DIR}/src/McuGFont.c
    ${MCULIB_DIR}/src/McuSSD1306.c
)

set(MCULIB_FONT_SOURCES
    ${MCULIB_DIR}/fonts/McuFontCour08Bold.c
    ${MCULIB_DIR}/fonts/McuFontCour08Normal.c
    ${MCULIB_DIR}/fonts/McuFontCour10Bold.c
    ${MCULIB_DIR}/fonts/McuFontCour10Normal.c
    ${MCULIB_DIR}/fonts/McuFontCour12Bold.c
    ${MCULIB_DIR}/fonts/McuFontCour12Normal.c
    ${MCULIB_DIR}/fonts/McuFontCour14Bold.c
    ${MCULIB_DIR}/fonts/McuFontCour14Normal.c
    ${MCULIB_DIR}/fonts/McuFontCour18Bold.c
    ${MCULIB_DIR}/fonts/McuFontCour18Normal.c
    ${MCULIB_DIR}/fonts/McuFontCour24Bold.c
    ${MCULIB_DIR}/fonts/McuFontCour24Normal.c
    ${MCULIB_DIR}/fonts/McuFontHelv08Bold.c
    ${MCULIB_DIR}/fonts/McuFontHelv08Normal.c
    ${MCULIB_DIR}/fonts/McuFontHelv10Bold.c
    ${MCULIB_DIR}/fonts/McuFontHelv10Normal.c
    ${MCULIB_DIR}/fonts/McuFontHelv12Bold.c
    ${MCULIB_DIR}/fonts/McuFontHelv12Normal.c
    ${MCULIB_DIR}/fonts/McuFontHelv14Bold.c
    ${MCULIB_DIR}/fonts/McuFontHelv14Normal.c
    ${MCULIB_DIR}/fonts/McuFontHelv18Bold.c
    ${MCULIB_DIR}/fonts/McuFontHelv18Normal.c
    ${MCULIB_DIR}/fonts/McuFontHelv24Bold.c
    ${MCULIB_DIR}/fonts/McuFontHelv24Normal.c
)

add_library(mculib STATIC
    ${MCULIB_SOURCES}
    ${MCULIB_FONT_SOURCES}
)

target_include_directories(mculib PUBLIC
    ${MCULIB_DIR}/config
    ${MCULIB_DIR}/config/fonts
    ${MCULIB_DIR}/src
    ${MCULIB_DIR}/fonts
)

target_compile_options(mculib PUBLIC
    -include ${CMAKE_CURRENT_LIST_DIR}/IncludeMcuLibConfig.h
)

# Lib réutilisable exposant oled_init / oled_display_ip / oled_deinit
add_library(oled_lib STATIC
    oled.c
)

target_include_directories(oled_lib PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}
)

target_link_libraries(oled_lib PUBLIC mculib)