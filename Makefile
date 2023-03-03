# Makefile for sample program
.PHONY			: all clean

# the program to build
NAME			:= APITest

# Build tools and flags
CXX			:= g++
LD			:= g++
SRCS                    := $(wildcard *.cc)
OBJS                    := $(patsubst %cc, %o, $(SRCS))
CPPFLAGS		:= -w -I../../inc -L/usr/lib -L../../lib/x86_64/ 

LDFLAGS			:=-lgxiapi -lpthread 

all			: $(NAME)

$(NAME)			: $(OBJS)
	$(LD) -o $@ $^ $(CPPFLAGS) $(LDFLAGS)

%.o		: %.cc
	$(CXX) $(CPPFLAGS) -c -o $@ $<

clean			:
	$(RM) *.o $(NAME)

