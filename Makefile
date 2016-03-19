#
# Makefile for apmode - use mingw-w64.
#
# vim: sw=4 ts=4 

PREFIX		:= 
#PREFIX		:= x86_64-w64-mingw32-
CC			:= $(PREFIX)gcc
CXX			:= $(PREFIX)g++
CFLAGS		:= -O0 -ggdb3
CXXFLAGS	:= $(CFLAGS)
LDFLAGS		:= -municode
EXT			:= .exe
SRCS		:= apmode.cpp
OBJS		:= $(SRCS:.cpp=.o)
RM			:= rm -f
PROG		:= apmode

$(PROG)$(EXT): $(OBJS)
	# For some reason, implicit link rules aren't kicking in...
	$(CXX) $(LDFLAGS) -o $(PROG)$(EXT) $(OBJS)
	
.o.cpp:

clean:
	$(RM) $(OBJS) $(PROG)$(EXT)