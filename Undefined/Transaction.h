#pragma once
#ifndef TRANSACTION_H
#define TRANSACTION_H
#include <cstdint>
#include <ctime>
#include <string>
#include "KISA_SHA256.h"

class Transaction
{
	friend class Blockchain;	// Transaction 내용을 파일 I/O할 때 Transactio class에 접근

	std::string who;
	time_t when;
	std::string what;
	std::string why;			// hash 안 함

public:
	Transaction(std::string _who, std::string _what, std::string _why, time_t _when = time(NULL));

	// getter method
	const BYTE * createTransactionData() const;
	inline int getTransactionLength() const;	// Hashing할 Transaction Data의 길이(byte)
	inline std::string getWho() const;
	inline time_t getWhen() const;
	inline std::string getWhat() const;
	inline std::string getWhy() const;
};

inline int Transaction::getTransactionLength() const {
	return who.length() + sizeof(when) + what.length();
}

inline std::string Transaction::getWho() const {
	return who;
}

inline time_t Transaction::getWhen() const {
	return when;
}

inline std::string Transaction::getWhat() const {
	return what;
}

inline std::string Transaction::getWhy() const {
	return why;
}

#endif