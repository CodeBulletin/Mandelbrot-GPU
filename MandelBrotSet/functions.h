#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>
#include <optional>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <iomanip>
#include <sstream>

#define Vec2d sf::Vector2<long double>
#define Vec2f sf::Vector2f
#define OVec2d std::optional<Vec2d>
#define OTexture std::optional<sf::Texture>
#define OColor std::optional<sf::Color>
#define ODouble std::optional<long double>
#define OInt std::optional<int>
#define String std::string

template <typename T>
std::string toStr(T value) {
	std::ostringstream os;
	os << std::setprecision(20) << value;
	return os.str();
}

template <typename T>
std::string toStr(T value, int per) {
	std::ostringstream os;
	os << std::setprecision(per) << value;
	return os.str();
}

template <typename T>
T clamp(const T &value, const T &min, const T &max) {
	return value < min ? min : value > max ? max : value;
}

template<typename T>
float distance(const T& a, const T& b) {
	return std::sqrtf((a.x * a.x) + (b.x * b.x));
}

template<typename T>
float magnitude(const T& a) {
	return std::sqrtf(a.x * a.x + a.y * a.y);
}

template<typename T>
T map(const T& value, const T& min, const T& max, const T& minRange, const T& maxRange) {
	return (minRange + (maxRange - minRange) * ((value - min) / (max - min)));
}

template<typename T>
T constrain(const T& value, const T& min, const T& max) {
	return value < min ? min : value > max ? max : value;
}