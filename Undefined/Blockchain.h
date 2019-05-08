#pragma once
#ifndef BLOCKCHAIN_H
#define BLOCKCHAIN_H
#include <cstdint>
#include "KISA_SHA256.h"

class Block;
class Transaction;

class Blockchain {
	const Block * genesisBlock;			// 첫번째 블록
	const Block * lastBlock;			// 마지막 블록
	Block * waitingBlock;				// 블록체인에 포함되기 위해 검증을 기다리는 블록(아직 검증되지 않은 블록)
	static std::uint64_t blockCount;	// 블록의 총 개수

	inline void addBlock(Block * _block);
	std::string timeToString(time_t t) const;

public:
	Blockchain();
	Blockchain(Transaction * _tx);

	void addTransaction(Transaction * _tx);
	void saveBlockchain() const;	// -> test 중 
	void loadBlockchain();			// -> 개발 중

	// getter method
	const Block * getGenesisBlock() const;
	const Block * getLastBlock() const;
	std::uint64_t getBlockCount() const;

	//for debug
	void printAllBlockHash() const;
	void printAllMerkleHash() const;
	void printAllTransaction(std::ostream& o) const;	// -> 개발 중	// -> printTransaction으로 변경할까?
	//void printWaitingBlock() const;					// -> 개발 중

	// tx 내용 검색 메소드								// -> 개발 중
	
};

// waiting block을 블록체인에 연결한다.
inline void Blockchain::addBlock(Block * _block) {
	lastBlock = _block;
	blockCount++;
}


#endif // !BLOCKCHAIN_H




