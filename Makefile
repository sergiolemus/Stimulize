CXX=g++
CXXFLAGS=-std=c++11 -O2
BIN_DIRECTORY=/usr/local/bin
MAN_DIRECTORY=/usr/local/share/man
MAIN=stimulize

UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Linux)
	MAN_DIRECTORY=/usr/local/man/man1
endif
ifeq ($(UNAME_S),Darwin)
	MAN_DIRECTORY=/usr/local/share/man/man1
endif

all: $(MAIN)

$(MAIN): $(MAIN).cpp
	$(CXX) -o $@ $^ $(CXXFLAGS) $(LDFLAGS)

install: $(MAIN) 
	cp $(MAIN) $(BIN_DIRECTORY)/$(MAIN)
	cp $(MAIN).man $(MAN_DIRECTORY)/$(MAIN).1
	gzip $(MAN_DIRECTORY)/$(MAIN).1

clean:
	rm -f $(MAIN)

uninstall:
	rm -f $(BIN_DIRECTORY)/$(MAIN)
	rm -f $(MAN_DIRECTORY)/$(MAIN).1.gz