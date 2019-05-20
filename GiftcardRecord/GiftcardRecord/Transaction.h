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
	BYTE senderPublicKey[SHA256_DIGEST_VALUELEN];
	std::uint64_t amount;									// UTXO
	std::uint64_t previousBlockIndex;						// hashing 안 함

public:
	Input();
	Input(const BYTE * _senderPrivateKey, std::uint64_t _amount, const BYTE * _previousTransactionHash);

	// getter method
	inline const BYTE * getPreviousTransactionHash() const;
	inline const BYTE * getSenderPublicKey() const;
	inline std::uint64_t getAmount() const;
	inline std::uint64_t getPreviousBlockIndex() const;
};


class Output {
	BYTE receiverPublicKey[SHA256_DIGEST_VALUELEN];
	std::uint64_t amount;

public:
	Output(const BYTE * _receiverPublicKey, std::uint64_t _amount);

	// getter method
	inline const BYTE * getReceiverPublicKey() const;
	inline std::uint64_t getAmount() const;
};

class Transaction {
	BYTE transactionHash[SHA256_DIGEST_VALUELEN];

	Input input;
	Output output;

	std::string giftcardName;
	time_t timestamp;

public:
	std::string memo;									// hash 안 함
	std::uint64_t includedBlockIndex;					// hash 안 함

	Transaction(Output & _outputs, std::string _giftcardName, std::string _memo);					// coinbase 거래
	Transaction(Input & _inputs, Output & _outputs, std::string _giftcardName, std::string _memo);	// 일반 거래

	const BYTE * createTransactionData() const;
	std::string toString() const;
	void print(std::ostream & o) const;
	void hashing();
	bool isValid(const BYTE * senderPrivateKey) const;	// input amount >= output amount
	bool isValidCoinbase() const;


	// getter method
	inline const std::string getGiftcardName() const;
	inline time_t getTimestamp() const;
	inline const std::string getMemo() const;
	inline const BYTE * getTransactionHash() const;
	inline const Input getInput() const;
	inline const Output getOutput() const;
	inline std::uint64_t getBlockIndex() const;

	int getTransactionDataSize() const;					// Hashing할 Transaction Data의 길이(byte)
	int getConfirmation() const;
	int getTotalInput() const;
	int getTotalOutput() const;
	int getTransactionFee() const;

	// setter method
	inline void setIncludedBlockIndex(std::uint64_t _blockIndex);
};

inline const BYTE * Input::getPreviousTransactionHash() const {
	return previousTransactionHash;
}

inline const BYTE * Input::getSenderPublicKey() const {
	return senderPublicKey;
}

inline std::uint64_t Input::getAmount() const {
	return amount;
}

inline std::uint64_t Input::getPreviousBlockIndex() const {
	return previousBlockIndex;
}

inline const BYTE * Output::getReceiverPublicKey() const {
	return receiverPublicKey;
}

inline std::uint64_t Output::getAmount() const {
	return amount;
}

inline int Transaction::getTransactionDataSize() const {
	return (sizeof(Input) - sizeof(std::uint64_t)) + sizeof(Output) 
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

inline const BYTE * Transaction::getTransactionHash() const {
	return transactionHash;
}

inline const Input Transaction::getInput() const {
	return input;
}

inline const Output Transaction::getOutput() const {
	return output;
}

inline std::uint64_t Transaction::getBlockIndex() const {
	return includedBlockIndex;
}

inline void Transaction::setIncludedBlockIndex(std::uint64_t _blockIndex) {
	includedBlockIndex = _blockIndex;
}

#endif