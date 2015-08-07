
set(SOURCES	
 	proc.cpp 
	qps.cpp
    screenshot.cpp
	pstable.cpp 
	uidstr.cpp 
	ttystr.cpp 
	dialogs.cpp 
	infobar.cpp 
	fieldsel.cpp 
	wchan.cpp 
	prefs.cpp 
	lookup.cpp 
	details.cpp 
	command.cpp 
	misc.cpp 
	trayicon.cpp 
	htable.cpp
#	htable2.cpp
#	pstable2.cpp
	qttableview.cpp 
)

SET(HEADERS	
	qps.h 
	screenshot.h
	pstable.h 
	dialogs.h 
	fieldsel.h 
	prefs.h 
	infobar.h 
	lookup.h 
	details.h 
	command.h 
	misc.h  
	trayicon.h 
	htable.h 
#	htable2.h
#	pstable2.h
	qttableview.h 
)

set(UIS
    watchdog.ui
    message.ui
)

#-----------------------------------------------------------------------------
# Translations
#-----------------------------------------------------------------------------
lxqt_translate_ts(QM_FILES
    UPDATE_TRANSLATIONS ${UPDATE_TRANSLATIONS}
    SOURCES ${SOURCES} ${HEADERS} ${UIS}
    INSTALL_DIR "${QPS_TRANSLATIONS_DIR}/${PROJECT_NAME}"
)

qt5_translation_loader(
    QM_LOADER
    "${QPS_TRANSLATIONS_DIR}"
    ${PROJECT_NAME}
)

lxqt_translate_desktop(DESKTOP_FILES
    SOURCES "${CMAKE_SOURCE_DIR}/qps.desktop.in"
)
#-----------------------------------------------------------------------------
# End of translations
#-----------------------------------------------------------------------------


add_executable(${PROJECT}
    ${SOURCES}
    ${HEADERS}
    ${UIS}
    ${QM_FILES}
    ${QM_LOADER}
    ${DESKTOP_FILES}
)

target_link_libraries(${PROJECT}
    Qt5::Widgets
    Qt5::X11Extras
    Qt5::DBus
)

install(TARGETS ${PROJECT} RUNTIME DESTINATION ${CMAKE_INSTALL_FULL_BINDIR})
install(FILES
    ${DESKTOP_FILES}
    DESTINATION "${CMAKE_INSTALL_DATAROOTDIR}/applications"
)
