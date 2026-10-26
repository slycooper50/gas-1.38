/* messages.c - error reporter -
   Copyright (C) 1987 Free Software Foundation, Inc.

This file is part of GAS, the GNU Assembler.

GAS is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 1, or (at your option)
any later version.

GAS is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with GAS; see the file COPYING.  If not, write to
the Free Software Foundation, 675 Mass Ave, Cambridge, MA 02139, USA.  */

#include <stdarg.h>
#include <string.h>
#include <stdio.h>		/* define stderr */
#ifdef	VMS
#include <perror.h>	/* Need this to make sys_errlist/sys_nerr right */
#endif /* VMS */

#include <errno.h>	/* Need this to make errno declaration right */

#include "as.h"

#ifndef NO_VARARGS
#include <varargs.h>
#endif

/*
		ERRORS

	JF: this is now bogus.  We now print more standard error messages
	that try to look like everyone else's.

	We print the error message 1st, beginning in column 1.
	All ancillary info starts in column 2 on lines after the
	key error text.
	We try to print a location in logical and physical file
	just after the main error text.
	Caller then prints any appendices after that, begining all
	lines with at least 1 space.

	Optionally, we may die.
	There is no need for a trailing '\n' in your error text format
	because we supply one.

as_warn(fmt,args)  Like fprintf(stderr,fmt,args) but also call errwhere().

as_fatal(fmt,args) Like as_warn() but exit with a fatal status.

*/


/* Nonzero if we've hit a 'bad error', and should not write an obj file,
   and exit with a nonzero error code */

int bad_error = 0;


/*
 *			a s _ p e r r o r
 *
 * Like perror(3), but with more info.
 */
/* JF moved from input-scrub.c to here.  */

void
as_perror(gripe, filename)
     char *	gripe;		/* Unpunctuated error theme. */
     char *	filename;
{
  as_where();
  fprintf (stderr, gripe, filename);
  fprintf (stderr, ": %s\n", strerror(errno));
  errno = 0;			/* After reporting, clear it. */
}


/*
 *			a s _ w a r n ( )
 *
 * Send to stderr a string (with bell) (JF: Bell is obnoxious!) as a warning, and locate warning
 * in input file(s).
 * Please only use this for when we have some recovery action.
 * Please explain in string (which may have '\n's) what recovery was done.
 */

void as_warn(const char *format, ...)
{
  va_list args;

  as_where();
  va_start(args, format);
  vfprintf(stderr, format, args);
  va_end(args);
  fprintf(stderr, "\n");
}

#ifdef DONTDEF
void
as_warn(Format,aa,ab,ac,ad,ae,af,ag,ah,ai,aj,ak,al,am,an)
char *format;
{
	if(!flagseen['W']) {
		as_where();
		fprintf(stderr,Format,aa,ab,ac,ad,ae,af,ag,ah,ai,aj,ak,al,am,an);
		(void)putc('\n',stderr);
	}
}
#endif
/*
 *			a s _ b a d ( )
 *
 * Send to stderr a string (with bell) (JF: Bell is obnoxious!) as a warning,
 * and locate warning in input file(s).
 * Please us when there is no recovery, but we want to continue processing
 * but not produce an object file.
 * Please explain in string (which may have '\n's) what recovery was done.
 */

void as_bad(const char *format, ...)
{
  va_list args;

  as_where();
  va_start(args, format);
  vfprintf(stderr, format, args);
  va_end(args);
  fprintf(stderr, "\n");
  bad_error = 1;
}

#ifdef DONTDEF
void
as_bad(Format,aa,ab,ac,ad,ae,af,ag,ah,ai,aj,ak,al,am,an)
char *format;
{
	as_where();
	bad_error=1;
	fprintf(stderr,Format,aa,ab,ac,ad,ae,af,ag,ah,ai,aj,ak,al,am,an);
	(void)putc('\n',stderr);
}
#endif
/*
 *			a s _ f a t a l ( )
 *
 * Send to stderr a string (with bell) (JF: Bell is obnoxious!) as a fatal
 * message, and locate stdsource in input file(s).
 * Please only use this for when we DON'T have some recovery action.
 * It exit()s with a warning status.
 */

void as_fatal(const char *format, ...)
{
  va_list args;

  as_where();
  va_start(args, format);
  vfprintf(stderr, format, args);
  va_end(args);
  fprintf(stderr, "\n");
  exit(42); /* Standard fatal exit status for GAS */
}

#ifdef DONTDEF
void
as_fatal(Format,aa,ab,ac,ad,ae,af,ag,ah,ai,aj,ak,al,am,an)
char *Format;
{
  as_where();
  fprintf (stderr, "FATAL:");
  fprintf(stderr, Format,aa,ab,ac,ad,ae,af,ag,ah,ai,aj,ak,al,am,an);
  (void) putc('\n', stderr);
  exit(42);
}
#endif

/* end: messages.c */
