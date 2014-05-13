project(qps)

cmake_minimum_required(VERSION 2.6.1 FATAL_ERROR)

find_package(Qt4 REQUIRED)
#add_subdirectory(src)

#set(QTERM_BINDIR "${CMAKE_INSTALL_PREFIX}gbin")
#set(QTERM_DATADIR "${CMAKE_INSTALL_PREFIX}gshareqterm")
#configure_file(${CMAKE_CURRENT_SOURCE_DIR}gconfig.h.cmake ${CMAKE_CURRENT_BINARY_DIR}config.h)


set(SOURCES	
 	proc.cpp 
	qps.cpp screenshot.cpp
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
     

# the next line sets up include and link directories and defines some variables that we will use.
# you can modify the behavior by setting some variables, e.g.
# set(QT_USE_OPENGL TRUE) # -> this will cause cmake to include and link against the OpenGL module
include(${QT_USE_FILE})

INCLUDE_DIRECTORIES(
#	${QT_INCLUDES}
#	${PYTHON_INCLUDE_PATH} 
	)
#link_directories (${xmlrpc_BINARY_DIR}gsrc)

# the variable vterm_SRC that require such a file. note: this assumes that you use
# <<#include "header.moc">> in your files
#qt4_automoc(${VTERM_SRC})

QT4_WRAP_CPP(MOC ${HEADERS})
QT4_WRAP_UI(UI watchdog.ui message.ui )

# create an executable file named "qtproject" from srcs
add_executable(qps ${SOURCES} ${MOC})
target_link_libraries(qps ${QT_LIBRARIES} )

