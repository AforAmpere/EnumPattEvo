range=-1
ifneq ($(range), -1)
	extr=-DRANGE=$(range)
else
	extr=
endif

MNCXX = EPEMain.cpp parseinput.cpp arrayfuncs.cpp
CXXFLAGS= -o EPE -O3 -std=c++17 $(extr)

INT:
	g++ $(MNCXX) INT.cpp Rot4ReflMoore.cpp -include INT.h $(CXXFLAGS)

INTGen:
	g++ $(MNCXX) INTGen.cpp Rot4ReflMoore.cpp -include INTGen.h $(CXXFLAGS)

B0INT:
	g++ $(MNCXX) B0INT.cpp Rot4ReflMoore.cpp -include B0INT.h $(CXXFLAGS)

HROT:
	g++ $(MNCXX) HROT.cpp Rot4ReflMoore.cpp -include HROT.h $(CXXFLAGS)

MAP:
	g++ $(MNCXX) MAP.cpp NoSymmMoore.cpp -include MAP.h $(CXXFLAGS)

B0MAP:
	g++ $(MNCXX) B0MAP.cpp NoSymmMoore.cpp -include B0MAP.h $(CXXFLAGS)