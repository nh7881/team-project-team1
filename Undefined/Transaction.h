#pragma once
#ifndef TRANSACTION_H
#define TRANSACTION_H
#include <cstdint>
#include <ctime>
#include <string>
#include "KISA_SHA256.h"

class Transaction
{
	friend class Blockchain;	// Transaction ������ ���� I/O�� �� Transactio class�� ����

	std::string who;
	time_t when;
	std::string what;
	std::string why;			// hash �� ��

	bool isValid() const;			// -> ���� ��
	inline int getTransactionLength() const;	// Hashing�� Transaction Data�� ����(byte)

public:
	Transaction(std::string _who, time_t _when, std::string _what, std::string _why);

	const BYTE * getTransactionData() const;

};

inline int Transaction::getTransactionLength() const {
	return who.length() + sizeof(when) + what.length();
}



#endif