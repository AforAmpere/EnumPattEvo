range=-1
ifneq ($(range), -1)
	extr=-DRANGE=$(range)
else
	extr=
endif

MNCXX = src/EPEMain.cpp src/parseinput.cpp src/arrayfuncs.cpp
CXXFLAGS= -o EPE -O3 -std=c++17 -I . $(extr)

%:
	Q=$(wildcard */*/$@.cpp); NBRHD=$${Q%/rulespace/$@.cpp}; g++ $(MNCXX) */*/$@.cpp $${NBRHD}/$${NBRHD}.cpp -include */*/$@.h $(CXXFLAGS)