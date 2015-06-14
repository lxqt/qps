#ifndef GLOBAL_H
#define GLOBAL_H

#include "qps.h"
#include "misc.h"

extern QList<Command*> commands;
extern ControlBar	*controlbar;
extern int	default_font_height;
extern bool 	flag_show_thread;	extern int 	flag_thread_ok;
extern bool 	previous_flag_show_thread;
extern int 	num_opened_files;

extern Qps			*qps;
extern SearchBox 	*search_box;
extern TFrame 		*infobox;

#endif	// GLOBAL_H
