C = g++
CFLAGS = -lX11 -lGL -lpthread -lpng -lstdc++ -lstdc++fs -std=c++17 -lm
VPATH = src inc lib lib/imgui lib/imgui/backends
SRCDIR = src
INCDIR = inc
OBJDIR = obj
BINDIR = bin
OBJ = $(notdir $(patsubst %.cpp,%.o,$(wildcard $(SRCDIR)/*))) # Project object files

all: KnightCA
		
KnightCA : $(OBJ)
	$(CC) -o $(BINDIR)/$@  $(?:%.o=$(OBJDIR)/%.o) $(CFLAGS)

KnightCA_Debug : $(OBJ)
	$(CC) -o $(BINDIR)/$@  $(?:%.o=$(OBJDIR)/%.o) $(CFLAGS) -g

%.o : %.cpp
	$(CC) -c $< -o $(OBJDIR)/$@

clean :
	rm ./obj/*

build:
	mkdir obj
	mkdir bin

debug: KnightCA_Debug
