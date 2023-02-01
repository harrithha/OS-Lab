CXX=g++
CFLAGS= -I./inc -g -Wall -Werror -std=gnu++11 -O3
bin_dir = bin 
obj_dir = obj


all: check_bin check_obj SJF SRTF

SJF: obj/Hardware.o obj/Process.o obj/utility.o src/SJF.cpp
	$(CXX) $(CFLAGS) $^ -o bin/SJF

SRTF: obj/Hardware.o obj/Process.o obj/utility.o src/SRTF.cpp
	$(CXX) $(CFLAGS) $^ -o bin/SRTF

obj/Hardware.o: src/Hardware.cpp inc/Hardware.h
	$(CXX) $(CFLAGS) -c $< -o $@
obj/Process.o: src/Process.cpp inc/Process.h
	$(CXX) $(CFLAGS) -c $< -o $@
obj/utility.o: src/utility.cpp inc/utility.h
	$(CXX) $(CFLAGS) -c $< -o $@

check_bin:
	[ -d $(bin_dir) ] || mkdir -p $(bin_dir)

check_obj:
	[ -d $(obj_dir) ] || mkdir -p $(obj_dir)

clean:
	rm -f obj/* bin/*
