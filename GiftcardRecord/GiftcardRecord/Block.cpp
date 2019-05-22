#include <iostream>
#include <vector>
#include <ctime>
#include <queue>
#include "Block.h"
//#include "Giftcard.h"
#include "Transaction.h"
using namespace std;

Block::Block() : Block(NULL) {}

// 16진수 기준 64자리의 숫자 - 0이 최대 64개 나올 수 있다.
Block::Block(const Block * _previousBlock) : previousBlock(_previousBlock) {
	if (bits < 0 || bits > 32)	// bits 값의 범위를 체크한다.
		cout << "error: block bits must be between 0 and 32.\n\n";
}


void Block::mining() {
	nonce = 0;
	timestamp = time(NULL);
	if (previousBlock) // if(previousBlock != NULL)
		blockIndex = previousBlock->blockIndex + 1;
	else
		blockIndex = 0;
	
	for (Transaction * tx : transactions) {
		tx->blockIndex = blockIndex;
	}
	
	BYTE * blockHeader = createBlockHeader();
	SHA256_Encrpyt(blockHeader, getBlockHeaderSize(), blockHash);

	int i = sizeof(blockIndex) + version.length() + sizeof(previousBlock->blockHash) + sizeof(merkleHash) + sizeof(bits) + sizeof(timestamp);
	//const BYTE * pb;	// pointer to transactionData
	while (!miningSuccess()) {
		nonce++;

		//pb = (BYTE *)&nonce;
		//for (j = 0; j < sizeof(nonce); i++, j++)
		//	blockHeader[i] = pb[j];

		memcpy(blockHeader + i, &nonce, sizeof(nonce));

		SHA256_Encrpyt(blockHeader, getBlockHeaderSize(), blockHash);
	}

	delete[] blockHeader;
}

// 반환된 포인터는 후에 delete[]로 할당 해제해야 함.
BYTE * Block::createBlockHeader() const {
	int i = 0;			// transactionData index
	//size_t j;			// block header index	
	//const BYTE * pb;	// pointer to transactionData
	BYTE * blockHeader = new BYTE[getBlockHeaderSize()];

	memcpy(blockHeader + i, &blockIndex, sizeof(blockIndex));
	i += sizeof(blockIndex);

	memcpy(blockHeader + i, version.c_str(), version.length());
	i += version.length();

	if (previousBlock) {	// if (previousBlock != NULL)
		memcpy(blockHeader + i, previousBlock->blockHash, sizeof(previousBlock->blockHash));
		i += sizeof(previousBlock->blockHash);
	}
	else {
		memcpy(blockHeader + i, previousBlock, sizeof(previousBlock));
		i += sizeof(previousBlock->blockHash);
	}

	memcpy(blockHeader + i, merkleHash, sizeof(merkleHash));
	i += sizeof(merkleHash);

	memcpy(blockHeader + i, &bits, sizeof(bits));
	i += sizeof(bits);

	memcpy(blockHeader + i, &timestamp, sizeof(timestamp));
	i += sizeof(timestamp);

	memcpy(blockHeader + i, &nonce, sizeof(nonce));
	i += sizeof(nonce);

	//pb = (BYTE *)&blockIndex;
	//for (j = 0; j < sizeof(blockIndex); i++, j++)
	//	buffer[i] = pb[j];

	//for (j = 0; j < version.length(); i++, j++)
	//	buffer[i] = version[j];

	//if (previousBlock != NULL) {
	//	pb = previousBlock->blockHash;
	//	for (j = 0; j < sizeof(previousBlock->blockHash); i++, j++)
	//		buffer[i] = pb[j];
	//}
	//else {
	//	for (j = 0; j < sizeof(previousBlock->blockHash); i++, j++)
	//		buffer[i] = 0;
	//}

	//for (j = 0; j < sizeof(merkleHash); i++, j++)
	//	buffer[i] = merkleHash[j];

	//pb = (BYTE *)&bits;
	//for (j = 0; j < sizeof(bits); i++, j++)
	//	buffer[i] = pb[j];

	//pb = (BYTE *)&timestamp;
	//for (j = 0; j < sizeof(timestamp); i++, j++)
	//	buffer[i] = pb[j];

	//pb = (BYTE *)&nonce;
	//for (j = 0; j < sizeof(nonce); i++, j++)
	//	buffer[i] = pb[j];

	return blockHeader;
}

bool Block::isValid() const {
	if (previousBlock) {	// if (previousBlock != NULL)
		if(previousBlock->timestamp - VALID_TIMESTAMP_GAP > timestamp) {
			cout << "\n\n" << blockIndex << "th block timestamp is unvalid...\n";
			return false;
		}
		if (previousBlock->blockIndex + 1 != blockIndex) {
			cout << "\n\n" << blockIndex << "th block index is unvalid...\n";
			return false;
		}
	}
		
	BYTE * hash = new BYTE[SHA256_DIGEST_VALUELEN];
	const BYTE * blockHeader = createBlockHeader();
	SHA256_Encrpyt(blockHeader, getBlockHeaderSize(), hash);

	if (isMemoryEqual(hash, blockHash, SHA256_DIGEST_VALUELEN)) {
		cout << "\n\nUnvalid block... Block Header are changed...\n";
		delete[] hash;
		delete[] blockHeader;
		return false;
	} else {
		delete[] hash;
		delete[] blockHeader;
		return true;
	}
}

bool Block::transactionsAreValid() const {
	const BYTE * merkleRoot = createMerkleRoot();

	if (isMemoryEqual(merkleRoot, merkleHash, SHA256_DIGEST_VALUELEN)) {
		cout << "\n\nUnvalid transaction... Some of Transaction Data are changed...\n";
		delete[] merkleRoot;
		return false;
	} else {
		delete[] merkleRoot;
		return true;
	}
}

// Assertion: block에 transaction 개수 > 0
void Block::initializeMerkleHash() const {
	const BYTE * merkleRoot = createMerkleRoot();

	memcpy((void *)merkleHash, merkleRoot, SHA256_DIGEST_VALUELEN);		// Merkle tree의 root node를 merkleHash에 복사한다.
	delete[] merkleRoot;
}

void Block::addTransactionsFrom(queue<Transaction *> & transactionPool) {
	for (unsigned int i = 0; i < MAX_TRANSACTION_COUNT; i++) {
		transactions.push_back(transactionPool.front());
		transactionPool.pop();
	}
}

// Assertion: size는 4의 양의 배수
bool Block::isMemoryEqual(const void * a, const void * b, size_t byteSize) {
	int * p1 = (int *)a;
	int * p2 = (int *)b;

	int intSize = byteSize / sizeof(int);
	for (int i = 0; i < intSize; i++, p1++, p2++) {
		if (*p1 != *p2)
			return false;
	}

	return true;
}

// 반환된 포인터 delete[]로 메모리 해제 필요함.	// p.s.기존 blockchain(bitcoin)과 알고리즘 다름
const BYTE * Block::createMerkleRoot() const {
	vector<BYTE *> txHashes;
	vector<BYTE *> txHashes2;
	txHashes.reserve(MAX_TRANSACTION_COUNT);
	txHashes2.reserve(MAX_TRANSACTION_COUNT);

	// Block에 담긴 모든 transaction을 SHA256으로 해싱한다.
	//for (size_t i = 0; i < transactions.size(); i++) {
	//	BYTE * hash = new BYTE[SHA256_DIGEST_VALUELEN];
	//	const BYTE * transactionData = transactions[i]->createTransactionData();
	//	SHA256_Encrpyt(transactionData, transactions[i]->getTransactionLength(), hash);
	//	transactionHash.push_back(hash);
	//	delete[] transactionData;
	//}

	for (Transaction * tx : transactions) {
		BYTE * txHash = new BYTE[SHA256_DIGEST_VALUELEN];
		memcpy(txHash, tx->getTransactionHash(), SHA256_DIGEST_VALUELEN);
		txHashes.push_back(txHash);
	}

	while (txHashes.size() + txHashes2.size() != 1) {
		while (txHashes.size() > 1)
			hashingTwoHash(txHashes, txHashes2);

		if (txHashes.size() == 1) {
			txHashes2.push_back(txHashes[0]);
			txHashes.pop_back();
		}

		while (txHashes2.size() > 1)
			hashingTwoHash(txHashes2, txHashes);

		if (txHashes2.size() == 1) {
			txHashes.push_back(txHashes2[0]);
			txHashes2.pop_back();
		}
	}

	return txHashes[0];
}

// 반환된 포인터 delete[]로 메모리 해제 필요함. _hashIn back 2개 원소를 붙인 것(64 byte)을 SHA256해서(32 byte) _hashOut에 push_back
inline void Block::hashingTwoHash(vector<BYTE *> & _hashIn, vector<BYTE *> & _hashOut) const {
	BYTE * hashIn = new BYTE[64];
	BYTE * hashOut = new BYTE[SHA256_DIGEST_VALUELEN];

	memcpy(hashIn, _hashIn.back(), SHA256_DIGEST_VALUELEN);
	delete[] _hashIn.back();
	_hashIn.pop_back();

	memcpy(hashIn + SHA256_DIGEST_VALUELEN, _hashIn.back(), SHA256_DIGEST_VALUELEN);
	delete[] _hashIn.back();
	_hashIn.pop_back();

	SHA256_Encrpyt(hashIn, 64, hashOut);
	delete[] hashIn;
	_hashOut.push_back(hashOut);
}









