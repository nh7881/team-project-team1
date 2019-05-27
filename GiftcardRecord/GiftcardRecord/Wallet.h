#pragma once
#ifndef WALLET_H
#define WALLET_H
#include <vector>
#include <cstdint>
#include <string>
#include <cstring>
#include "KISA_SHA256.h"

class Input;
class Output;
class Giftcard;
class Transaction;

class UTXO {
	BYTE transactionHash[SHA256_DIGEST_VALUELEN];
	std::uint64_t blockIndex;
	std::uint64_t balance;

public:
	UTXO(const BYTE * txHash, std::uint64_t blockIndex, std::uint64_t balance);
	void print() const;

	// getter method
	inline const BYTE * getTransactionHash() const;
	inline std::uint64_t getBlockIndex() const;
	inline std::uint64_t getBalance() const;

	inline void setTransactionHash(const BYTE * _transactionHash);
	inline void setBlockIndex(std::uint64_t _blockIndex);
	inline void setBalance(std::uint64_t _balance);
};

class Wallet {
	BYTE privateKey[SHA256_DIGEST_VALUELEN];
	BYTE publicKey[SHA256_DIGEST_VALUELEN];

	std::vector<UTXO> myUTXOTable;
	std::vector<UTXO> UTXOTable;

	std::uint64_t nonce;

public:
	Wallet(std::string _passPhrase);

	Transaction * createCoinbaseTransaction(std::uint64_t blockIndex, std::string type, std::string memo = NULL);
	Transaction * createTransaction(const BYTE * receiverPublicKey, std::string type, std::int64_t sendingAmount,
		std::int64_t fee, std::string memo = NULL);

	// getter method
	inline const BYTE * getPrivateKey() const;
	inline const BYTE * getPublicKey() const;
	inline std::vector<UTXO> getMyUTXOTable() const;
	inline std::vector<UTXO> getUTXOTable() const;
 	int getMyUTXOAmount() const;

	// setter method
	inline void setMyUTXOTable(std::vector<UTXO> & _myUTXOTable);
	inline void setUTXOTable(std::vector<UTXO> & _utxoTable);

	void printTransactionHistory() const;
	void printMyUTXOTable() const;
	void printUTXOTable() const;
};



inline const BYTE * UTXO::getTransactionHash() const {
	return transactionHash;
}

inline std::uint64_t UTXO::getBlockIndex() const {
	return blockIndex;
}

inline std::uint64_t UTXO::getBalance() const {
	return balance;
}

inline void UTXO::setTransactionHash(const BYTE * _txHash) {
	memcpy(transactionHash, _txHash, SHA256_DIGEST_VALUELEN);
}

inline void UTXO::setBlockIndex(std::uint64_t _blockIndex) {
	blockIndex = _blockIndex;
}

inline void UTXO::setBalance(std::uint64_t _balance) {
	balance = _balance;
}



inline const BYTE * Wallet::getPrivateKey() const {
	return privateKey;
}

inline const BYTE * Wallet::getPublicKey() const {
	return publicKey;
}

inline std::vector<UTXO> Wallet::getMyUTXOTable() const {
	return myUTXOTable;
}

inline std::vector<UTXO> Wallet::getUTXOTable() const {
	return UTXOTable;
}

inline void Wallet::setMyUTXOTable(std::vector<UTXO> & _myUTXOTable) {
	myUTXOTable = _myUTXOTable;
}

inline void Wallet::setUTXOTable(std::vector<UTXO> & _utxoTable) {
	UTXOTable = _utxoTable;
}



#endif