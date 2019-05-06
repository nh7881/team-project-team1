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

	bool isValid() const;			// -> 개발 중
	

public:
	Transaction(std::string _who, time_t _when, std::string _what, std::string _why);

	const BYTE * getTransactionData() const;
	inline int getTransactionLength() const;	// Hashing할 Transaction Data의 길이(byte)
};

inline int Transaction::getTransactionLength() const {
	return who.length() + sizeof(when) + what.length();
}



#endif