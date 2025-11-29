#
# Makefile for apmode - use mingw-w64.
#
# vim: set sw=4 ts=4 ft=make et fenc=utf-8:

#PREFIX		:=
PREFIX		:= x86_64-w64-mingw32-
CC			:= $(PREFIX)gcc
CXX			:= $(PREFIX)g++
CFLAGS		:= -O0 -ggdb3
CXXFLAGS	:= $(CFLAGS) -std=c++20 -DUNICODE
LDFLAGS		:= -lwlanapi -municode
EXT			:= .exe
SRCS		:= apmode.cpp
OBJS		:= $(SRCS:.cpp=.o)
RM			:= rm -f
PROG		:= apmode

$(PROG)$(EXT): $(OBJS)
	# For some reason, implicit link rules aren't kicking in...
	$(CXX) -o $(PROG)$(EXT) $(OBJS) $(LDFLAGS)
	
.o.cpp:

clean:
	$(RM) $(OBJS) $(PROG)$(EXT)