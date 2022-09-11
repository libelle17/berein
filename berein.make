ich := $(firstword $(MAKEFILE_LIST))
CC=g++
#CFLAGS=-c -Wall -lvmime -lgnutls -lgsasl
CFLAGS=-c -Wall -I/usr/include/mysql 
LDFLAGS=-L/mysql/client/ -lmysqlclient 
ifneq (,$(filter $(HOST),linux1 linux4))
  PFAD=/DATA/down/cpp
else ifeq ($(HOST),linux2)
  PFAD =/vista/V/cpp
endif
FILES=berein.cpp konsole.cpp DB.cpp
SRC= $(FILES:%.cpp=$(PFAD)/%.cpp)
#OBJ=$(FILES:.cpp=.o)
OBJ=$(SRC:.cpp=.o) 
EXEC=/root/bin/berein
DEPFILE= $(EXEC).d

all: anzeig dep $(OBJ) $(EXEC)
#	$(EXEC)
	@echo "Fertig mit $(ich)!" 
#	@if test -s fehler.txt; then vi +0/error fehler.txt; exit; fi;

anzeig:
	@echo "Zeige an:"
	@echo "SRC: $(SRC)"
	@echo "OBJ: $(OBJ)"
	@echo "CFLAGS: $(CFLAGS)"
	@echo "LDFLAGS: $(LDFLAGS)"
	@echo "DEPFILE: $(DEPFILE)"
	@echo "EXEC: $(EXEC)"

dep: $($(PFAD)SRC)
	-@rm fehler.txt
	@echo "erstelle $(DEPFILE)"
	-$(CC) -MM $(SRC) > $(DEPFILE) 2> fehler.txt
#	@if test -s fehler.txt; then vi +0/error fehler.txt; exit; fi;

#.cpp.o:
%.o: %.cpp
#%.o: $(PFAD)/%.cpp
	@echo "kompiliere $<"
	-$(CC) $(CFLAGS) -o $@ $< 2> fehler.txt
	@if test -s fehler.txt; then vi +0/error fehler.txt; exit; fi;

$(EXEC): $(OBJ)
	@echo "verlinke $(OBJ) zu $@"
	-$(CC) $(LDFLAGS) $(OBJ) -o $@ 2> fehler.txt
	@if test -s fehler.txt; then vi +0/error fehler.txt; exit; fi;

-include $(DEPFILE)

.PHONY: clean

clean: 
	rm $(EXEC) $(OBJ)
