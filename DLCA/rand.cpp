#include "rand.h"

random_device rd;
mt19937 rand_engine(rd());
