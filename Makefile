CXX=g++
CXXFLAGS=-std=c++11 -O2
BIN_DIRECTORY=/usr/local/bin
MAN_DIRECTORY=/usr/local/share/man/man1
MAIN=stimulize


$(MAIN): $(MAIN).cpp
	$(CXX) -o $@ $^ $(CXXFLAGS) $(LDFLAGS)

install: $(MAIN) 
	cp $(MAIN) $(BIN_DIRECTORY)/$(MAIN)
	cp $(MAIN).man $(MAN_DIRECTORY)/$(MAIN).1
clean:
	rm -f $(MAIN)

uninstall:
	rm -f $(BIN_DIRECTORY)/$(MAIN)
	rm -f $(MAN_DIRECTORY)/$(MAIN).1
