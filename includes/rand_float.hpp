#pragma once


#include <random>
#include <ctime>




class RNG_float {
public:
	RNG_float() { rng.seed(std::random_device{}()); }
	float operator()(float min, float max) { //rng.seed(std::random_device{}()); 
	std::uniform_real_distribution<float> dist(min, max); return dist(rng); }
private:
	std::mt19937 rng;
};

/*
class RNG_double {
public:
	RNG_double() { rng.seed(std::random_device{}()); }
	double operator()(double min, double max)  { std::uniform_real_distribution<double> dist(min, max); return dist(rng); }
private:
	std::mt19937 rng;
};

*/