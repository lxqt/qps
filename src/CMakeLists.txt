
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

add_executable(${PROJECT}
    ${SOURCES}
    ${HEADERS}
    ${UIS}
)

target_link_libraries(${PROJECT}
    Qt5::Widgets
    Qt5::X11Extras
    Qt5::DBus
)

install(TARGETS ${PROJECT} RUNTIME DESTINATION ${CMAKE_INSTALL_FULL_BINDIR})
