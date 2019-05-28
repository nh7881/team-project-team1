#pragma once
#ifndef GIFTCARD_H
#define GIFTCARD_H
#include <string>

class Giftcard {
	std::string name;	// hashing 함
	std::uint64_t faceValue;
	std::uint64_t marketValue;

public:
	Giftcard(std::string _name, std::uint64_t faceValue, std::uint64_t marketValue);
	~Giftcard();

	inline std::string getName() const { return name; }
	inline std::uint64_t getFaceValue() const { return faceValue; }
	inline std::uint64_t getMarketValue() const { return marketValue; }
};

#endif