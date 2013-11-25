mitabbase=/home/aaji/proj/mitab-1.7.0

OPTFLAGS	= -O2
INCFLAGS	= -I $(mitabbase)/mitab
LIBS		= -lmitab -L $(mitabbase)/mitab

CC = gcc
CXX = g++


all: tab2wkt

tab2wkt: tab2wkt.c 
	$(CC) tab2wkt.c $(INCFLAGS) $(LIBS) $(OPTFLAGS) -o $@
clean:
	@rm -f tab2wkt

