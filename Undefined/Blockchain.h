#pragma once
#ifndef BLOCKCHAIN_H
#define BLOCKCHAIN_H
#include <cstdint>
#include "KISA_SHA256.h"

class Block;
class Transaction;

class Blockchain {
	const Block * genesisBlock;			// ù��° ���
	const Block * lastBlock;			// ������ ���
	Block * waitingBlock;				// ���ü�ο� ���ԵǱ� ���� ������ ��ٸ��� ���(���� �������� ���� ���)
	static std::uint64_t blockCount;	// ����� �� ����

	inline void addBlock(Block * _block);
	std::string timeToString(time_t t) const;

public:
	Blockchain();
	Blockchain(Transaction * _tx);

	void addTransaction(Transaction * _tx);
	void saveBlockchain() const;	// -> test �� 
	void loadBlockchain();			// -> ���� ��

	// getter method
	const Block * getGenesisBlock() const;
	const Block * getLastBlock() const;
	std::uint64_t getBlockCount() const;

	//for debug
	void printAllBlockHash() const;
	void printAllMerkleHash() const;
	void printAllTransaction() const;		// -> ���� ��
	//void printWaitingBlock() const;		// -> ���� ��

	// tx ���� �˻� �޼ҵ�					// -> ���� ��
	
};

// waiting block�� ���ü�ο� �����Ѵ�.
inline void Blockchain::addBlock(Block * _block) {
	lastBlock = _block;
	blockCount++;
}


#endif // !BLOCKCHAIN_H




