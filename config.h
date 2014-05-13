// config.h					-*-c++-*-
//
// This program is free software. See the file COPYING for details.
// Author: Mattias Engdegård, 1997-1999

#ifndef CONFIG_H
#define CONFIG_H

#if defined(linux)
#define LINUX

#elif defined(sun) && defined(__SVR4)
#define SOLARIS

#else
#error Only Linux >= 2.0 and Solaris >= 2.6 supported, sorry

#endif

#endif // CONFIG_H
