CXX = g++
CXXFLAGS = -std=c++11 -O2 -g
OUT = corrcheck

$(OUT): main.o Directory.o File.o corrcheck.o
	$(CXX) $(CXXFLAGS) -lcrypto $^ -o $@

main.o: corrcheck.h
Directory.o: Directory.h
File.o: File.h
corrcheck.o: corrcheck.h File.h

.PHONY: clean
clean:
	rm -rf *.o $(OUT)

