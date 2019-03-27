CXX=g++
CXXFLAGS=-std=c++11 -O2
MAIN=STIMULIZE

$(MAIN): $(MAIN).cpp
	$(CXX) -o $@ $^ $(CXXFLAGS) $(LDFLAGS)

install: $(MAIN) 
	cp $(MAIN) /usr/local/bin/$(MAIN)

clean:
	rm -f $(MAIN)

uninstall:
	rm -f /usr/local/bin/$(MAIN)
