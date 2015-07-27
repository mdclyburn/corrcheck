CXX = g++
CXXFLAGS = -std=c++11 -O2 -g
OUT = corrcheck

$(OUT): main.o corrcheck.o Database.o Directory.o Options.o
	$(CXX) $(CXXFLAGS) -lcrypto $^ -o $@

main.o: corrcheck.h
corrcheck.o: corrcheck.h File.h
Database.o: Database.h
Directory.o: Directory.h File.h
Options.o: Options.h

.PHONY: clean
clean:
	rm -rf *.o $(OUT)

