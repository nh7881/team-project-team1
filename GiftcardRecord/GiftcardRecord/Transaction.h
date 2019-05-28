#pragma once
#ifndef TRANSACTION_H
#define TRANSACTION_H
#define COINBASE_REWARD 50
#include <iostream>
#include <cstdint>
#include <vector>
#include <ctime>
#include <string>
#include "KISA_SHA256.h"

class Wallet;
class UTXO;

class Input {
	BYTE previousTransactionHash[SHA256_DIGEST_VALUELEN];
	BYTE senderPublicKey[SHA256_DIGEST_VALUELEN];
	std::uint64_t amount;									// UTXO
	std::uint64_t previousBlockIndex;

public:
	Input(std::uint64_t _amount, std::uint64_t _blockIndex);	// coinbase input
	Input(const BYTE * _senderPrivateKey, std::uint64_t _amount, const BYTE * _previousTransactionHash);
	
	// getter method
	inline const BYTE * getPreviousTransactionHash() const { return previousTransactionHash; }
	inline const BYTE * getSenderPublicKey() const { return senderPublicKey; }
	inline std::uint64_t getAmount() const { return amount; }
	inline std::uint64_t getPreviousBlockIndex() const { return previousBlockIndex; }
};

class Output {
	BYTE receiverPublicKey[SHA256_DIGEST_VALUELEN];
	std::uint64_t amount;

public:
	Output(const BYTE * _receiverPublicKey, std::uint64_t _amount);

	// getter method
	inline const BYTE * getReceiverPublicKey() const { return receiverPublicKey; }
	inline std::uint64_t getAmount() const { return amount; }
};

class Transaction {
	BYTE transactionHash[SHA256_DIGEST_VALUELEN];

	std::vector<Input *> inputs;
	std::vector<Output *> outputs;

	std::string propertyType;
	time_t timestamp;

	std::uint64_t nonce;

public:
	BYTE blockHash[SHA256_DIGEST_VALUELEN];
	std::uint64_t blockIndex;					// hash 안 함
	std::string memo;							// hash 안 함

	Transaction(const BYTE * privateKey, std::string _propertyType, std::uint64_t nonce, std::string _memo = NULL);
	Transaction(std::vector<Input *> & _inputs, std::vector<Output *> & _outputs, std::string _propertyType, 
		std::uint64_t nonce, std::string _memo = NULL);
	~Transaction();

	const BYTE * createTransactionData() const;
	std::string toString() const;
	void print(std::ostream & o) const;
	void hashing();
	bool isValid(const BYTE * senderPrivateKey) const;	// input amount >= output amount, UTXO를 참조하고 금액이 정확한지
	bool isValidCoinbase() const;

	// getter method
	inline time_t getTimestamp() const { return timestamp; }
	inline const std::string getType() const { return propertyType; }
	inline const std::string getMemo() const { return memo; }
	inline const BYTE * getTransactionHash() const { return transactionHash; }
	inline std::vector<Input *> getInputs() const { return inputs; }
	inline std::vector<Output *> getOutputs() const { return outputs; }

	int getTransactionDataSize() const;					// Hashing할 Transaction Data의 길이(byte)
	int getConfirmation() const;
	int getTotalInputs() const;
	int getTotalOutputs() const;
	int getTransactionFee() const;

	// setter method
};


#endif