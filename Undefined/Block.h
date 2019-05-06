#pragma once
#ifndef BLOCK_H
#define BLOCK_H
#define MAX_TRANSACTION_COUNT 1			// 한 블록에 들어갈 수 있는 최대 transaction의 개수
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cstdint>
#include <ctime>
#include "KISA_SHA256.h"

class Transaction;

class Block {
	friend class Blockchain;				// Block 내용을

	BYTE blockHash[32];						// Block의 ID

	const static std::string version;		// Blockchain 이름
	const Block * previousBlock;			// 이전 블록
	BYTE merkleHash[32];					// 개별 transaction 해시값으로 만든 머클트리(해시트리)의 머클루트
	time_t timestamp;						// 블록이 블록체인에 승인된 시각
	int bits = 2;							// blockhash 앞에 나와야 할 0의 개수(난이도 조절)
	std::int64_t nonce;						// 임의 대입 수

	std::vector<const Transaction *> tx;	// Transaction

	// Block class에서 사용하는 메소드
	BYTE * getBlockHeader() const;
	inline bool miningSuccess() const;
	inline void hashingTwoHash(std::vector<BYTE *> & hash, std::vector<BYTE *> & hash2) const;

	// Blockchain class에서 사용하는 메소드
	Block();
	Block(const Block * _previousBlock);

	bool isValid() const;					// -> test 중
	inline bool isFull() const;
	void mining();
	void findMerkleHash() const;
	inline void addTransaction(Transaction * _tx);
	
public:
	// getter method
	inline int getBlockHeaderLength() const;
	inline const BYTE * getBlockHash() const;
	inline const Block * getPreviousBlock() const;
	inline const BYTE * getMerkleHash() const;
	inline time_t getTimestamp() const;
	inline std::vector<const Transaction *> getTransaction() const;
};

inline void Block::addTransaction(Transaction * _tx) {
	tx.push_back(_tx);
}

inline int Block::getBlockHeaderLength() const {
	return version.length() + sizeof(previousBlock->blockHash) + sizeof(merkleHash) 
		+ sizeof(bits) + sizeof(timestamp) + sizeof(nonce);	// 12 + 32 + 32 + 4 + 8 + 8
}

inline bool Block::isFull() const {
	return tx.size() >= MAX_TRANSACTION_COUNT ? true : false;
}

inline bool Block::miningSuccess() const {
	for (int i = 0; i < bits; i++) {
		if (blockHash[i] != 0)
			return false;
	}
	return true;
}

inline const BYTE * Block::getBlockHash() const {
	return blockHash;
}

inline const Block * Block::getPreviousBlock() const {
	return previousBlock;
}

inline const BYTE * Block::getMerkleHash() const {
	return merkleHash;
}

inline time_t Block::getTimestamp() const {
	return timestamp;
}

inline std::vector<const Transaction *> Block::getTransaction() const {
	return tx;
}

#endif // !BLOCK_H



