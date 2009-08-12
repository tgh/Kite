# Copyright © 2009 Tyler Hayes
# ALL RIGHTS RESERVED
#
# [This program is licensed under the GPL version 3 or later.]
# Please see the file COPYING in the source
# distribution of this software for license terms.
#
# Thanks to Tom Szilagyi (http://tap-plugins.sourceforge.net/)
# for his Makefile as an example for me.  Also special thanks
# to Bart Massey (http://web.cecs.pdx.edu/~bart/) for his help
# in getting the correct flags for creating the shared object.
#
# NOTE: since LADSPA_PATH varies depending on your distro,
# you must change the LADSPA_PATH variable and UNINSTALL
# variable in this Makefile to match your LADSPA_PATH
# environment variable!

#-----------------------------------------------------

CC = gcc
CFLAGS	= -Wall -O3 -fPIC
LDFLAGS = -nostartfiles -shared -Wl,-Bsymbolic
LADSPA_PATH = /usr/lib/ladspa      # change these 2 variables to match
UNINSTALL = /usr/lib/ladspa/sb_*   # your LADSPA_PATH environment
                                   # variable (type 'echo $LADSPA_PATH
                                   # at your shell prompt)
PLUGINS	=	sb_kite.so

# ----------------------------------------------------

all: $(PLUGINS)

sb_kite.o: sb_kite.c ../xorgens.c ../ladspa.h ../xorgens.h
	$(CC) $(CFLAGS) -c sb_kite.c
	$(CC) $(CFLAGS) -c ../xorgens.c

sb_kite.so: sb_kite.o xorgens.o
	$(CC) $(LDFLAGS) -o sb_kite.so sb_kite.o xorgens.o

install: sb_kite.so
	cp sb_kite.so $(LADSPA_PATH)

uninstall:
	rm -f $(UNINSTALL)

clean:
	rm -f *.o *.so *~
