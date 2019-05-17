#pragma once
#ifndef TRANSACTION_H
#define TRANSACTION_H
#include <iostream>
#include <cstdint>
#include <ctime>
#include <string>
#include "KISA_SHA256.h"

class Wallet;
class UTXO;

class Input {
	BYTE previousTransactionHash[SHA256_DIGEST_VALUELEN];
	BYTE previousTransactionBlockHash[SHA256_DIGEST_VALUELEN];	// 참고용 정보. hashing 안 함
	BYTE senderPublicKey[SHA256_DIGEST_VALUELEN];
	std::uint64_t amount;										// UTXO

public:
	Input(const BYTE & _senderPublicKey, std::uint64_t _amount, const BYTE & _previousTransactionHash);
	Input(const BYTE & _senderPublicKey, std::uint64_t _amount, std::vector<UTXO> senderUTXOTable);
};

class Output {
	BYTE receiverPublicKey[SHA256_DIGEST_VALUELEN];
	std::uint64_t amount;

public:
	Output(const BYTE & _receiverPublicKey, std::uint64_t _amount);
};

class Transaction {
	BYTE transactionHash[SHA256_DIGEST_VALUELEN];

	std::vector<Input> inputs;
	std::vector<Output> outputs;

	std::string giftcardName;
	time_t timestamp;

public:
	std::string memo;									// hash 안 함
	std::uint64_t includedBlockIndex;					// hash 안 함

	Transaction(std::vector<Input> & inputs, std::vector<Output> & outputs, std::string _giftcardName, std::string memo);

	const BYTE * createTransactionData() const;			// Hashing할 Transaction Data
	std::string toString() const;
	void print(std::ostream & o) const;
	void hashing();
	bool isValid(const BYTE & senderPrivateKey) const;	// input amount > output amount
	bool isValidCoinbase() const;

	// getter method
	inline const std::string getGiftcardName() const;
	inline time_t getTimestamp() const;
	inline const std::string getMemo() const;
	inline const BYTE & getTransactionHash() const;

	inline int getTransactionLength() const;			// Hashing할 Transaction Data의 길이(byte)
	inline int getTransactionSize() const;
	inline int getConfirmation() const;
	inline int getTotalInput() const;
	inline int getTotalOutput() const;
	inline int getTransactionFee() const;

	// setter method
};

inline int Transaction::getTransactionLength() const {
	return inputs.size() * (sizeof(Input) - 32) + outputs.size() * sizeof(Output) 
		+ giftcardName.length() + sizeof(timestamp);
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

inline const BYTE & Transaction::getTransactionHash() const {
	return *transactionHash;
}

#endif