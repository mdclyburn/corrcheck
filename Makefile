CXX = g++
CXXFLAGS = -std=c++11 -O2 -g
OUT = corrcheck

$(OUT): main.o Database.o Directory.o corrcheck.o
	$(CXX) $(CXXFLAGS) -lcrypto $^ -o $@

main.o: corrcheck.h
Database.o: Database.h
Directory.o: Directory.h File.h
corrcheck.o: corrcheck.h File.h

.PHONY: clean
clean:
	rm -rf *.o $(OUT)

