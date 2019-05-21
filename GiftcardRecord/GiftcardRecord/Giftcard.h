#pragma once
#ifndef GIFTCARD_H
#define GIFTCARD_H
#include <string>

class Giftcard {
	std::string name;	// hashing 함

public:
	Giftcard(std::string _name);
	~Giftcard();

	inline std::string getName() const;
};

inline std::string Giftcard::getName() const {
	return name;
}

#endif