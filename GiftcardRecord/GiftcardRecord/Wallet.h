#pragma once
#ifndef WALLET_H
#define WALLET_H
#include <vector>
#include <cstdint>
#include <string>
#include "KISA_SHA256.h"

class UTXO {
	BYTE transactionHash[SHA256_DIGEST_VALUELEN];
	std::uint64_t blockIndex;

public:
	UTXO();
	UTXO(const BYTE * txHash, std::uint64_t blockIndex);
	void print() const;

	// getter method
	const BYTE * getTransactionHash() const;
	std::uint64_t getBlockIndex() const;

	void setTransactionHash(const BYTE * _transactionHash);
	void setBlockIndex(std::uint64_t _blockIndex);
};

class Wallet {
	BYTE privateKey[SHA256_DIGEST_VALUELEN];
	BYTE publicKey[SHA256_DIGEST_VALUELEN];

	std::vector<UTXO> myUTXOTable;
	std::vector<UTXO> utxoTable;

public:
	Wallet(std::string _passPhrase);

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
	return utxoTable;
}

inline void Wallet::setMyUTXOTable(std::vector<UTXO> & _myUTXOTable) {
	myUTXOTable = _myUTXOTable;
}

inline void Wallet::setUTXOTable(std::vector<UTXO> & _utxoTable) {
	utxoTable = _utxoTable;
}



#endif