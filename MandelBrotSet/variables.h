#pragma once
#include "functions.h"

class variables {
public:
	double i, speed;
	int n, i2;
	bool hold;
	bool uihold;
	variables(ODouble _i = {}, OInt _n = {}, OInt _i2 = {}, ODouble _speed = {}) {
		i = _i.value_or(0);
		n = clamp(_n.value_or(2), 2, 10);
		i2 = clamp(_i2.value_or(1), 1, 20);
		speed = clamp(_speed.value_or(1.0), (long double) 0.1, (long double) 10.0);
		hold = false;
		uihold = false;
	}
	double increaseI() {
		i += std::log(n) / std::log(10);
		return i;
	}

	double decreaseI() {
		i -= std::log(n) / std::log(10);
		return i;
	}

	int increaseI2() {
		i2 = clamp(i2 + 1, 1, 20);
		return i2;
	}

	int decreaseI2() {
		i2 = clamp(i2 - 1, 1, 20);
		return i2;
	}

	int IncreaseSpeed() {
		speed = clamp(speed + 0.1, 0.1, 10.0);
		return speed;
	}

	int DecreaseSpeed() {
		speed = clamp(speed - 0.1, 0.1, 10.0);
		return speed;
	}

	void IncreaseN() {
		n = clamp(n + 1, 2, 10);
	}

	void DecreaseN() {
		n = clamp(n - 1, 2, 10);
	}
};