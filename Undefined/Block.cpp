#include <iostream>
#include <vector>
#include <ctime>
#include "Block.h"
#include "Transaction.h"
using namespace std;

const string Block::version = "RentalRecord";

Block::Block() : Block(NULL) {}

// 16���� ���� 64�ڸ��� ���� - 0�� �ִ� 64�� ���� �� �ִ�.
Block::Block(const Block * _previousBlock) : previousBlock(_previousBlock) {
	// bits ���� ������ üũ�Ѵ�.
	if (bits < 0 || bits > 32)
		cout << "error: block bits must be between 0 and 32.\n\n";
}

void Block::mining() {
	nonce = 0;
	timestamp = time(NULL);

	int i, j;
	BYTE * buffer;
	const BYTE * pb;	// pointer to transactionData
	
	buffer = getBlockHeader();
	SHA256_Encrpyt(buffer, getBlockHeaderLength(), blockHash);
	
	while (!miningSuccess()) {
		nonce++;
		i = version.length() + sizeof(previousBlock->blockHash)	+ sizeof(merkleHash) + sizeof(bits) + sizeof(timestamp);

		pb = (BYTE *)&nonce;
		for (j = 0; j < sizeof(nonce); i++, j++)
			buffer[i] = pb[j];

		SHA256_Encrpyt(buffer, getBlockHeaderLength(), blockHash);
	}

	delete[] buffer;
}

// ��ȯ�� �����ʹ� �Ŀ� delete[]�� �Ҵ� �����ؾ� ��.
BYTE * Block::getBlockHeader() const {
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

// Assertion: block�� transaction ���� > 0
void Block::findMerkleHash() const {
	BYTE * hash;
	vector<BYTE *> transactionHash;
	vector<BYTE *> transactionHash2;
	transactionHash.reserve(MAX_TRANSACTION_COUNT);
	transactionHash2.reserve(MAX_TRANSACTION_COUNT);

	SHA256_Encrpyt(getTransactionData(), )
	transactionHash.push_back(getTransactionData);

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

	// ��Ŭ��Ʈ�� orderhash�� �����Ѵ�.
	memcpy((void *)merkleHash, transactionHash[0], 32);
	delete[] transactionHash[0];
}

// hashOut delete[] �ʿ���. _hashIn back 2�� ���Ҹ� ���� ��(64 byte)�� SHA256�ؼ�(32 byte) _hashOut�� push_back
inline void Block::hashingTwoHash(vector<BYTE *> & _hashIn, vector<BYTE *> & _hashOut) const {
	BYTE * hashIn = new BYTE[64];
	BYTE * hashOut = new BYTE[32];

	memcpy(hashIn, _hashIn.back(), 32);
	delete[] _hashIn.back();
	_hashIn.pop_back();

	memcpy(hashIn + 32, _hashIn.back(), 32);
	delete[] _hashIn.back();
	_hashIn.pop_back();

	SHA256_Encrpyt(hashIn, 64, hashOut);
	delete[] hashIn;
	_hashOut.push_back(hashOut);
}

bool Block::isValid() const {
	BYTE * hash;
	BYTE * blockHeader = getBlockHeader();
	SHA256_Encrpyt(blockHeader, getBlockHeaderLength(), hash);
	
	if (memcmp(hash, blockHash, 32) != 0) {
		cout << "\n\nUnvalid block... Block Header are changed...\n";
		delete[] blockHeader;
		return false;
	}
	else {
		delete[] blockHeader;
		return true;
	}
}





