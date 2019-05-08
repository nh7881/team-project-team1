#include <iostream>
#include <vector>
#include <ctime>
#include "Block.h"
#include "Transaction.h"
using namespace std;

const string Block::version = "RentalRecord";

Block::Block() : Block(NULL) {}

// 16진수 기준 64자리의 숫자 - 0이 최대 64개 나올 수 있다.
Block::Block(const Block * _previousBlock) : previousBlock(_previousBlock) {
	// bits 값의 범위를 체크한다.
	if (bits < 0 || bits > 32)
		cout << "error: block bits must be between 0 and 32.\n\n";
}

void Block::mining() {
	nonce = 0;
	timestamp = time(NULL);

	int i, j;
	BYTE * blockHeader;
	const BYTE * pb;	// pointer to transactionData
	
	blockHeader = createBlockHeader();
	SHA256_Encrpyt(blockHeader, getBlockHeaderLength(), blockHash);
	
	while (!miningSuccess()) {
		nonce++;
		i = version.length() + sizeof(previousBlock->blockHash)	+ sizeof(merkleHash) + sizeof(bits) + sizeof(timestamp);

		pb = (BYTE *)&nonce;
		for (j = 0; j < sizeof(nonce); i++, j++)
			blockHeader[i] = pb[j];

		SHA256_Encrpyt(blockHeader, getBlockHeaderLength(), blockHash);
	}

	delete[] blockHeader;
}

// 반환된 포인터는 후에 delete[]로 할당 해제해야 함.
BYTE * Block::createBlockHeader() const {
	size_t i = 0;		// transactionData index
	size_t j;			// block header index	
	const BYTE * pb;	// pointer to transactionData
	BYTE * buffer = new BYTE[getBlockHeaderLength()];
	
	for (j = 0; j < version.length(); i++, j++)
		buffer[i] = version[j];

	if (previousBlock != NULL) {
		pb = previousBlock->blockHash;
		for (j = 0; j < sizeof(previousBlock->blockHash); i++, j++)
			buffer[i] = pb[j];
	}
	else {
		for (j = 0; j < sizeof(previousBlock->blockHash); i++, j++)
			buffer[i] = 0;
	}

	for (j = 0; j < sizeof(merkleHash); i++, j++)
		buffer[i] = merkleHash[j];

	pb = (BYTE *)&bits;
	for (j = 0; j < sizeof(bits); i++, j++)
		buffer[i] = pb[j];

	pb = (BYTE *)&timestamp;
	for (j = 0; j < sizeof(timestamp); i++, j++)
		buffer[i] = pb[j];

	pb = (BYTE *)&nonce;
	for (j = 0; j < sizeof(nonce); i++, j++)
		buffer[i] = pb[j];

	return buffer;
}

bool Block::isValid() const {
	BYTE * hash = new BYTE[SHA256_DIGEST_VALUELEN];
	const BYTE * blockHeader = createBlockHeader();
	SHA256_Encrpyt(blockHeader, getBlockHeaderLength(), hash);

	if (memcmp(hash, blockHash, SHA256_DIGEST_VALUELEN) != 0) {
		cout << "\n\nUnvalid block... Block Header are changed...\n";
		delete[] hash;
		delete[] blockHeader;
		return false;
	}
	else {
		delete[] hash;
		delete[] blockHeader;
		return true;
	}
}

bool Block::transactionsAreValid() const {
	const BYTE * merkleRoot = createMerkleRoot();

	if (memcmp(merkleRoot, merkleHash, SHA256_DIGEST_VALUELEN) != 0) {
		cout << "\n\nUnvalid transaction... Transaction Data are changed...\n";
		delete[] merkleRoot;
		return false;
	}
	else {
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

// 반환된 포인터 delete[]로 메모리 해제 필요함.	// p.s.기존 blockchain(bitcoin)과 알고리즘 다름
const BYTE * Block::createMerkleRoot() const {
	const BYTE * transactionData;
	BYTE * hash = new BYTE[SHA256_DIGEST_VALUELEN];
	vector<BYTE *> transactionHash;
	vector<BYTE *> transactionHash2;
	transactionHash.reserve(MAX_TRANSACTION_COUNT);
	transactionHash2.reserve(MAX_TRANSACTION_COUNT);

	// Block에 담긴 모든 transaction을 SHA256으로 해싱한다.
	for (size_t i = 0; i < tx.size(); i++) {
		transactionData = tx[i]->createTransactionData();
		SHA256_Encrpyt(transactionData, tx[i]->getTransactionLength(), hash);
		transactionHash.push_back(hash);
		delete[] transactionData;
	}

	while (transactionHash.size() + transactionHash2.size() != 1) {
		while (transactionHash.size() > 1)
			hashingTwoHash(transactionHash, transactionHash2);

		if (transactionHash.size() == 1) {
			transactionHash2.push_back(transactionHash[0]);
			transactionHash.pop_back();
		}

		while (transactionHash2.size() > 1)
			hashingTwoHash(transactionHash2, transactionHash);

		if (transactionHash2.size() == 1) {
			transactionHash.push_back(transactionHash2[0]);
			transactionHash2.pop_back();
		}
	}

	return transactionHash[0];
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









