#pragma once
#ifndef TRANSACTION_H
#define TRANSACTION_H
#include <iostream>
#include <cstdint>
#include <vector>
#include <ctime>
#include <string>
#include "KISA_SHA256.h"

class Wallet;
class UTXO;
class Giftcard;

class Input {
	BYTE previousTransactionHash[SHA256_DIGEST_VALUELEN];
	BYTE senderPublicKey[SHA256_DIGEST_VALUELEN];
	std::uint64_t amount;									// UTXO
	std::uint64_t previousBlockIndex;						// hashing 안 함

public:
	Input(const BYTE * _senderPrivateKey, std::uint64_t _amount, const BYTE * _previousTransactionHash = NULL);

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

	std::vector<Input *> inputs;
	std::vector<Output *> outputs;

	Giftcard * giftcard;
	time_t timestamp;

public:
	std::string memo;							// hash 안 함
	std::uint64_t blockIndex;					// hash 안 함

	Transaction(std::vector<Input *> _inputs, std::vector<Output *> _outputs, Giftcard * _giftcard, std::string _memo);	// 일반 거래
	~Transaction();

	const BYTE * createTransactionData() const;
	std::string toString() const;
	void print(std::ostream & o) const;
	void hashing();
	bool isValid(const BYTE * senderPrivateKey) const;	// input amount >= output amount, UTXO를 참조하고 금액이 정확한지
	bool isValidCoinbase() const;


	// getter method
	inline const Giftcard * getGiftcard() const;
	inline time_t getTimestamp() const;
	inline const std::string getMemo() const;
	inline const BYTE * getTransactionHash() const;
	inline std::vector<Input *> getInputs() const;
	inline std::vector<Output *> getOutputs() const;

	int getTransactionDataSize() const;					// Hashing할 Transaction Data의 길이(byte)
	int getConfirmation() const;
	int getTotalInput() const;
	int getTotalOutput() const;
	int getTransactionFee() const;

	// setter method
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

inline time_t Transaction::getTimestamp() const {
	return timestamp;
}

inline const Giftcard * Transaction::getGiftcard() const {
	return giftcard;
}

inline const std::string Transaction::getMemo() const {
	return memo;
}

inline const BYTE * Transaction::getTransactionHash() const {
	return transactionHash;
}

inline std::vector<Input *> Transaction::getInputs() const {
	return inputs;
}

inline std::vector<Output *> Transaction::getOutputs() const {
	return outputs;
}


#endif