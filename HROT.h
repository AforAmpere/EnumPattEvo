#include "Rot4ReflMoore.h"
#define HROT_H
#define TRANSCOUNT ((2*RANGE+1)*(2*RANGE+1)*2)
#define STATECOUNT 2
#ifndef RANGE
#define RANGE 5
#endif