#pragma once
#ifndef BLOCKCHAIN_H
#define BLOCKCHAIN_H
#include <cstdint>
#include <queue>
#include <string>
#include "KISA_SHA256.h"

class Wallet;
class Block;
class Transaction;
class UTXO;

class Blockchain {
	const Block * genesisBlock;					// 첫번째 블록
	const Block * lastBlock;					// 마지막 블록
	Block * waitingBlock;						// 채굴을 기다리는 블록
	std::queue<Transaction *> transactionPool;	// 검증을 기다리는 거래(아직 검증되지 않은 블록)
	std::uint64_t blockCount;					// 블록의 총 개수
	std::string version;						// Blockchain version

	inline void addBlock(Block * _block);

public:
	Blockchain(std::string _version);
	Blockchain(std::string _version, Transaction * _tx);

	void addTransaction(Transaction * _tx);

	// getter method
	inline const Block * getGenesisBlock() const;
	inline const Block * getLastBlock() const;
	inline std::uint64_t getBlockCount() const;
	inline std::string getVersion() const;


	std::vector<UTXO> getUTXOTable() const;
	std::vector<UTXO> getUTXOTable(const BYTE * privateKey) const;


	// setter method
	inline void setVersion(std::string _version);

	//for debug
	void printAllBlockHash() const;
	void printAllMerkleHash() const;
	void printAllTransaction(std::ostream & o) const;
	void printWaitingBlock() const;						// -> 개발 중
	void printBlockchain(std::ostream & o) const;
	void loadBlockchain();								// -> 개발 중	// isValid와 비슷


	static std::string timeToString(time_t t);

	// tx 내용 검색 메소드								// -> 개발 중
	
};

// waiting block을 블록체인에 연결한다.
inline void Blockchain::addBlock(Block * _block) {
	lastBlock = _block;
	blockCount++;
}

inline const Block * Blockchain::getGenesisBlock() const {
	return genesisBlock;
}

inline const Block * Blockchain::getLastBlock() const {
	return lastBlock;
}

inline uint64_t Blockchain::getBlockCount() const {
	return blockCount;
}

inline std::string Blockchain::getVersion() const {
	return std::string();
}

inline void Blockchain::setVersion(std::string _version) {
	version = _version;
}


#endif // !BLOCKCHAIN_H




