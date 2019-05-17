#pragma once
#ifndef WALLET_H
#define WALLET_H
#include <vector>
#include <cstdint>
#include <string>
#include "KISA_SHA256.h"

class Input;
class Output;
class Transaction;

class UTXO {
	BYTE transactionHash[SHA256_DIGEST_VALUELEN];
	std::uint64_t blockIndex;						// 지갑

public:
	void print() const;
};

class Wallet {
	BYTE privateKey[SHA256_DIGEST_VALUELEN];
	BYTE publicKey[SHA256_DIGEST_VALUELEN];

	std::vector<UTXO> myUTXOTable;

public:
	Wallet(std::string _passPhrase);

	Transaction & createCoinbaseTransaction(std::vector<Output> & outputs, std::string giftcardName, std::string memo) const;
	Transaction & createTransaction(std::vector<Input> & inputs, std::vector<Output> & outputs, std::string giftcardName, std::string memo) const;

	// getter method
	inline const BYTE & getPrivateKey() const;
	inline const BYTE & getPublicKey() const;
	std::vector<UTXO> getMyUTXOTable() const;
 	int getMyUTXOAmount() const;

	// setter method
	void setMyUTXOTable(std::vector<UTXO> & _myUTXOTable);

	void printTransactionHistory() const;
	void printMyUTXOTable() const;
};

inline const BYTE & Wallet::getPublicKey() const {
	return *publicKey;
}


#endif