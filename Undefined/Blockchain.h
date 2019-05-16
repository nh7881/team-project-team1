#pragma once
#ifndef BLOCKCHAIN_H
#define BLOCKCHAIN_H
#include <cstdint>
#include <queue>
#include <string>
#include <condition_variable>
#include <mutex>
#include "KISA_SHA256.h"

class Wallet;
class Block;
class Transaction;

class Blockchain {
	const Block * genesisBlock;					// 첫번째 블록
	const Block * lastBlock;					// 마지막 블록
	Block * waitingBlock;
	std::queue<Transaction *> transactionPool;	// 검증을 기다리는 거래(아직 검증되지 않은 블록) //--Producer, Consumer의 공유 자원--
	std::uint64_t blockCount;					// 블록의 총 개수
	std::string version;						// Blockchain version

	//std::mutex m;								// Transaction Producer, Consumer 관리
	//std::condition_variable cv;				// Transaction Producer, Consumer 관리

	inline void addBlock(Block * _block);

public:
	Blockchain(std::string _version);
	Blockchain(std::string _version, Transaction * _tx);//, Wallet & _wallet);

	void addTransaction(Transaction * _tx);// , Wallet & _wallet);
	//void consumeTransaction();	// -> 개발 중
	void saveBlockchain() const;
	void loadBlockchain();			// -> 개발 중	// isValid와 비슷

	// getter method
	inline const Block * getGenesisBlock() const;
	inline const Block * getLastBlock() const;
	inline std::uint64_t getBlockCount() const;
	inline std::string getVersion() const;

	// setter method
	void setVersion(std::string _version);

	//for debug
	void printAllBlockHash() const;
	void printAllMerkleHash() const;
	void printAllTransaction(std::ostream& o) const;
	//void printWaitingBlock() const;					// -> 개발 중

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




