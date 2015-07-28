#pragma once
#include <random>

using std::random_device;
using std::mt19937;

extern random_device rd;
// Mersenne Twister PRNG
extern mt19937 rand_engine;
