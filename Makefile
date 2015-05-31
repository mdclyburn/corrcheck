CXX = g++
CXXFLAGS = -std=c++11 -O2
OUT = corrcheck

$(OUT): main.o corrcheck.o
	$(CXX) $(CXXFLAGS) -lcrypto $^ -o $@

main.o: corrcheck.h
corrcheck.o: corrcheck.h File.h

.PHONY: clean
clean:
	rm -rf *.o $(OUT)

