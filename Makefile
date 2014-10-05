#
#  Makefile for fltk applications
#

#set local path to fltk
LOCAL = /cse/courses/cse557/local

INCLUDE = -I$(LOCAL)/
LIBDIR = -L$(LOCAL)/lib -L/usr/X11R6/lib

#Linux
LIBS = -lfltk -lfltk_gl -lXext -lX11 -lm -lGL -lGLU -lpng -ljpeg
#Mac
#LIBS = -lXext -lX11 -lm -lpthread -lpng -ljpeg -lfltk -lfltk_gl -framework AGL -framework OpenGL -framework ApplicationServices -framework Cocoa

CFLAGS = -g -fpermissive 

CC = g++

.SUFFIXES: .o .cpp

.cpp.o: 
	$(CC) $(CFLAGS) $(INCLUDE) -c -o $*.o $<

ALL.O = imageio.o impBrush.o \
        impressionist.o FltDesignUI.o \
        impressionistDoc.o impressionistUI.o \
	originalView.o paintView.o pointBrush.o

impressionist: $(ALL.O)
	$(CC) $(CFLAGS) -o $@ $(ALL.O) $(INCLUDE) $(LIBDIR) $(LIBS)
