#pragma once
#ifndef TRANSACTION_H
#define TRANSACTION_H
#include <iostream>
#include <cstdint>
#include <ctime>
#include <string>
#include "KISA_SHA256.h"

class Input {
	friend class Transaction;

	BYTE senderPublicKey[SHA256_DIGEST_VALUELEN];
	BYTE previousTransactionHash[SHA256_DIGEST_VALUELEN];
	BYTE previousTransactionBlockHash[SHA256_DIGEST_VALUELEN];	// hashing 안 함
	std::uint64_t amount;										// UTXO

public:
	Input(BYTE & _senderPublicKey, BYTE & _previousTransactionHash, BYTE & previousTransactionBlockHash, std::uint64_t amount);

	//void setSenderPublicKey();
};

class Output {
	friend class Transaction;

	BYTE receiverPublicKey[SHA256_DIGEST_VALUELEN];
	std::uint64_t amount;

public:
	Output(BYTE & _senderPublicKey, BYTE & _previousTransactionHash, BYTE & previousTransactionBlockHash, std::uint64_t amount);
};

class Transaction {
	const BYTE transactionHash[SHA256_DIGEST_VALUELEN];

	std::vector<Input> Inputs;
	std::vector<Output> Outputs;

	std::string blockchainVersion;
	std::string giftcardName;
	time_t timestamp;
	std::string memo;			// hash 안 함

public:
	std::uint64_t includedBlockIndex;
	BYTE includedBlockHash[SHA256_DIGEST_VALUELEN];	//--------------꼭 필요있나?--------------

	Transaction(std::vector<Input> inputs, std::vector<Output> outputs, std::string _version, std::string _giftcardName, std::string memo);

	const BYTE * createTransactionData() const;		// Hashing할 Transaction Data
	std::string toString() const;
	void print(std::ostream & o) const;
	void hashing();

	bool isValid() const;							// input amount > output amount

	// getter method
	inline const std::string getGiftcardName() const;
	inline time_t getTimestamp() const;
	inline const std::string getMemo() const;
	inline const BYTE * getTransactionHash() const;

	inline int getTransactionLength() const;		// Hashing할 Transaction Data의 길이(byte)
	int getTransactionSize() const;
	int getConfirmation() const;
	int getTotalInput() const;
	int getTotalOutput() const;
	int getTransactionFee() const;
};

inline int Transaction::getTransactionLength() const {
	return who.length() + sizeof(timestamp) + what.length();
}

inline time_t Transaction::getTimestamp() const {
	return timestamp;
}

inline const std::string Transaction::getGiftcardName() const {
	return giftcardName;
}

inline const std::string Transaction::getMemo() const {
	return memo;
}

inline const BYTE * Transaction::getTransactionHash() const {
	return transactionHash;
}

#endif