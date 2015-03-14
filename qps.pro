# initial written by fasthyun@show.co.kr
#

TEMPLATE	= app
LANGUAGE	= C++
CONFIG		+= warn_off qt  precompile_header release #debug
QT 		+= dbus	 	# qt > 4.5
QT              += widgets      # qt > 5.0
RESOURCES	= qps.qrc
FORMS		= watchdog.ui message.ui

PRECOMPILED_HEADER = stable.h

precompile_header:!isEmpty(PRECOMPILED_HEADER) {
	DEFINES += USING_PCH
}

HEADERS	+= qps.h \
	pstable.h \
	dialogs.h \
	fieldsel.h \
	prefs.h \
	infobar.h \
	lookup.h \
	details.h \
	command.h \
	misc.h  \
	trayicon.h \
	htable.h \
	qttableview.h


SOURCES	+= proc.cpp \
	qps.cpp \
	pstable.cpp \
	uidstr.cpp \
	ttystr.cpp \
	dialogs.cpp \
	infobar.cpp \
	fieldsel.cpp \
	wchan.cpp \
	prefs.cpp \
	lookup.cpp \
	details.cpp \
	command.cpp \
	misc.cpp \
	trayicon.cpp \
	htable.cpp\
	qttableview.cpp

TARGET 	= qps

UI_DIR = .ui
MOC_DIR = .moc
OBJECTS_DIR = .obj

bin.path = /usr/local/bin
bin.files= qps
man.path = /usr/local/share/man/man1
man.files= qps.1

INSTALLS += bin man

OS = $$system(uname -s)

contains( OS, [Ll]inux ) {
	system(ln -fs proc_linux.cpp proc.cpp)
	QMAKE_POST_LINK += strip -g qps  #same  "install -s"
}

contains( OS, SunOS ) {
	DEFINES -= USING_PCH
	system(ln -s proc_solaris.cpp proc.cpp)
	LIBS += -lkstat -lrt
	QMAKE_POST_LINK += strip -x qps
}

contains( OS, MOSIX ) {
	system(ln -s proc_mosix.cpp proc.cpp)
	QMAKE_POST_LINK += strip -g qps
}

