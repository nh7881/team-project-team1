#pragma once
#ifndef BLOCK_H
#define BLOCK_H
#define MAX_TRANSACTION_COUNT 2			// 한 블록에 들어갈 수 있는 최대 transaction의 개수
#define VALID_TIMESTAMP_GAP 60
#include <iostream>
#include <vector>
#include <queue>
#include <string>
#include <cstdint>
#include <ctime>
#include "KISA_SHA256.h"

class Transaction;

class Block {
	friend class Blockchain;

	std::uint64_t blockIndex;				// Block의 높이
	BYTE blockHash[SHA256_DIGEST_VALUELEN];	// Block의 ID

	std::string version;					// Blockchain 이름
	const Block * previousBlock;			// 이전 블록
	BYTE merkleHash[SHA256_DIGEST_VALUELEN];// 개별 transaction 해시값으로 만든 머클트리(해시트리)의 머클루트
	time_t timestamp;						// 블록이 블록체인에 승인된 시각
	int bits = 2;							// blockhash 앞에 나와야 할 0의 개수(난이도 조절)
	std::int64_t nonce;						// 임의 대입 수

	std::vector<Transaction *> transactions;// Transaction

	// Block class에서 사용하는 메소드
	BYTE * createBlockHeader() const;		// <- test 필요
	const BYTE * createMerkleRoot() const;
	inline bool miningSuccess() const;
	inline void hashingTwoHash(std::vector<BYTE *> & hash, std::vector<BYTE *> & hash2) const;

	// Blockchain class에서 사용하는 메소드
	Block();
	Block(const Block * _previousBlock);

	bool isValid() const;
	bool transactionsAreValid() const;
	inline bool isFull() const;
	void mining();							// <- test 필요
	void initializeMerkleHash() const;
	void addTransactionsFrom(std::queue<Transaction *> & transactionPool);
	inline int getBlockHeaderSize() const;

	// getter method
	inline const BYTE * getBlockHash() const { return blockHash; }
	inline const Block * getPreviousBlock() const { return previousBlock; }
	inline const BYTE * getMerkleHash() const { return merkleHash; }
	inline time_t getTimestamp() const { return timestamp; }
	//inline std::vector<Transaction *> getTransactions() const { return transactions; }

	// setter method
	inline void setBits(int _bits) { bits = _bits; }

public:
	static bool isMemoryEqual(const void * a, const void * b, size_t size);
};

inline int Block::getBlockHeaderSize() const {
	return sizeof(blockIndex) + version.length() + sizeof(previousBlock->blockHash) + sizeof(merkleHash) 
		+ sizeof(bits) + sizeof(timestamp) + sizeof(nonce);	// 8 + 12 + 32 + 32 + 4 + 8 + 8
}

inline bool Block::isFull() const {
	return transactions.size() >= MAX_TRANSACTION_COUNT ? true : false;
}

inline bool Block::miningSuccess() const {
	for (int i = 0; i < bits; i++) {
		if (blockHash[i] != 0)
			return false;
	}
	return true;
}

#endif // !BLOCK_H



