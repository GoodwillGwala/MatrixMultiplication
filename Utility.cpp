#include "Utility.h"
#include <iostream>
#include <random>
#include <ctime>
#include <cassert>

namespace
{
	std::default_random_engine createRandomEngine()
	{
		auto seed = static_cast<unsigned long>(std::time(nullptr));
		return std::default_random_engine(seed);
	}

	auto RandomEngine = createRandomEngine();
}



int randomInt(int exclusiveMax, int inclusiveMin)
{
	std::uniform_int_distribution<> distr(inclusiveMin, exclusiveMax);
	return distr(RandomEngine);
}
