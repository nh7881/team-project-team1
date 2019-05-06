#pragma once
#ifndef BLOCK_H
#define BLOCK_H
#define MAX_TRANSACTION_COUNT 1			// �� ��Ͽ� �� �� �ִ� �ִ� transaction�� ����
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cstdint>
#include <ctime>
#include "KISA_SHA256.h"

class Transaction;

class Block {
	friend class Blockchain;				// Block ������

	BYTE blockHash[32];						// Block�� ID

	const static std::string version;		// Blockchain �̸�
	const Block * previousBlock;			// ���� ���
	BYTE merkleHash[32];					// ���� transaction �ؽð����� ���� ��ŬƮ��(�ؽ�Ʈ��)�� ��Ŭ��Ʈ
	time_t timestamp;						// ����� ���ü�ο� ���ε� �ð�
	int bits = 2;							// blockhash �տ� ���;� �� 0�� ����(���̵� ����)
	std::int64_t nonce;						// ���� ���� ��

	std::vector<const Transaction *> tx;	// Transaction

	// Block class���� ����ϴ� �޼ҵ�
	BYTE * getBlockHeader() const;
	inline bool miningSuccess() const;
	inline void hashingTwoHash(std::vector<BYTE *> & hash, std::vector<BYTE *> & hash2) const;

	// Blockchain class���� ����ϴ� �޼ҵ�
	Block();
	Block(const Block * _previousBlock);

	bool isValid() const;					// -> test ��
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



