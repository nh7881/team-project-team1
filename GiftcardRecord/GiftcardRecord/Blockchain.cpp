#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <fstream>
#include <cstdint>
#include <iomanip>
#include <cstdint>
#include "BlockChain.h"
#include "Block.h"
#include "Transaction.h"
#include "Wallet.h"
using namespace std;

ostream & operator<<(ostream& o, const BYTE * hash) {
	o << "0x";
	for (int i = 0; i < SHA256_DIGEST_VALUELEN; i++) {
		o.width(2);
		o.fill('0');
		o << hex << (int)hash[i];
	}
	o << dec;
	return o;
}

// Genesis block을 생성한다.
Blockchain::Blockchain(string _version, Wallet * wallet) : blockCount(0), version(_version) {
	Block * _genesisBlock = new Block();
	genesisBlock = _genesisBlock;
	_genesisBlock->version = version;		// 현재 blockchain version 입력
	_genesisBlock->transactions.push_back(wallet->createCoinbaseTransaction(blockCount, "coin", "Coinbase Transaction"));
	_genesisBlock->initializeMerkleHash();	// transaction으로 merkletree의 merkleroot(=merklehash) 계산
	_genesisBlock->mining();				// waiting block을 채굴
	addBlock(_genesisBlock);				// waiting block을 blockchain에 추가

	Block * newWaitingBlock = new Block(lastBlock);
	waitingBlock = newWaitingBlock;
}

// transaction pool에 transaction을 추가한다.
void Blockchain::addTransactionToPool(Transaction * _tx) {
	transactionPool.push(_tx);	
}

// transaction pool에서 transaction을 가져와 채굴한다.
void Blockchain::produceBlock(Wallet * wallet) {
	if (transactionPool.size() >= MAX_TRANSACTION_COUNT) {
		waitingBlock->version = version;		// 현재 blockchain version 입력
		waitingBlock->transactions.push_back(wallet->createCoinbaseTransaction(blockCount, "coin", "Coinbase Transaction"));
		waitingBlock->addTransactionsFrom(transactionPool);
		waitingBlock->initializeMerkleHash();	// transaction으로 merkletree의 merkleroot(=merklehash) 계산
		waitingBlock->mining();					// waiting block을 채굴
		addBlock(waitingBlock);					// waiting block을 blockchain에 추가

		Block * newWaitingBlock = new Block(lastBlock);
		waitingBlock = newWaitingBlock;
		cout << "Mining block was completed!\n";
	}
	else {
		cout << "There aren't enough transactions in the memory pool to produce block...\n";
	}
}

string Blockchain::getFileName() const {
	time_t t = time(NULL);
	struct tm * date = localtime(&t);
	return lastBlock->version + ' ' + to_string(date->tm_year + 1900) + ". " + to_string(date->tm_mon + 1)
		+ ". " + to_string(date->tm_mday) + ".txt";;
}

void Blockchain::printAllBlockHash() const {
	if (!lastBlock) {	// if (lastBlock == NULL)
		cout << "\nThere is no block in the blockchain...\n\n";
		return;
	}

	const Block * presentBlock = lastBlock;
	for (uint64_t i = 0; i < blockCount; i++) {
		cout << "Block Hash: 0x";
		for (int j = 0; j < SHA256_DIGEST_VALUELEN; j++) {
			printf("%02x", presentBlock->blockHash[j]);
		}
		presentBlock = presentBlock->previousBlock;
		cout << '\n';
	}
}

void Blockchain::printAllMerkleHash() const {
	if (!lastBlock) {	// if (lastBlock == NULL)
		cout << "\nThere is no block in the blockchain...\n\n";
		return;
	}

	const Block * presentBlock = lastBlock;
	for (uint64_t i = 0; i < blockCount; i++) {
		cout << "Merkle Hash: 0x";
		for (int j = 0; j < SHA256_DIGEST_VALUELEN; j++) {
			printf("%02x", presentBlock->merkleHash[j]);
		}
		presentBlock = presentBlock->previousBlock;
		cout << '\n';
	}
}

void Blockchain::printAllTransaction(ostream& o) const {
	if (!lastBlock) {	// if (lastBlock == NULL)
		cout << "\nThere is no block in the blockchain...\n\n";
		return;
	}

	const Block * presentBlock = lastBlock;
	for (uint64_t i = 0; i < blockCount; i++) {
		if (presentBlock->transactionsAreValid() && presentBlock->isValid()) {		// merkleHash와 blockHash 유효성 검사
			size_t txSize = presentBlock->transactions.size();
			for (size_t j = 0; j < txSize; j++) {
				o << "\nTransaction #" << j + 1 << '\n';
				presentBlock->transactions[j]->print(o);
			}
			o << '\n';
			presentBlock = presentBlock->previousBlock;
		} else {
			cout << "Digital forgery had occured in " << presentBlock->blockIndex << "th block...";
			break;
		}
	}
}

void Blockchain::printBlockchain(ostream & o) const {
	if (!lastBlock) {	// if (lastBlock == NULL)
		cout << "\nThere is no block in the blockchain...\n\n";
		return;
	}

	const Block * presentBlock = lastBlock;

	o << "Blockchain current version: " << presentBlock->version << '\n';
	for (uint64_t i = 0; i < blockCount; i++) {
		o << "----------------------------------------------------------\n";
		o << "Block #" << presentBlock->blockIndex << '\n';
		o << "Block Hash:		" << presentBlock->blockHash << '\n';
		
		o << "\nVersion:		" << presentBlock->version << '\n';
		if (presentBlock->previousBlock != NULL) {
			o << "Previous Block Hash:	" << presentBlock->previousBlock->blockHash << '\n';
		} else {
			o << "Previous Block Hash:	0x0000000000000000000000000000000000000000000000000000000000000000" << '\n';
		}
		o << "Merkle Hash:		" << presentBlock->merkleHash << '\n';
		o << "Timestamp:		" << timeToString(presentBlock->timestamp) << '\n';
		o << "Bits:			" << presentBlock->bits << '\n';
		o << "Nonce:			" << presentBlock->nonce << '\n';

		int j = 1;
		for (Transaction * tx : presentBlock->transactions) {
			o << "\nTransaction #" << j << '\n';
			tx->print(o);
			j++;
		}
		
		presentBlock = presentBlock->previousBlock;
	}
}

vector<UTXO> Blockchain::getUTXOTable() const
{
	return vector<UTXO>();
}

// Private Key를 받아 해당 key의 사용되지 않은 거래를 찾아서 반환한다.
vector<UTXO> Blockchain::getUTXOTable(const BYTE * privateKey) const {
	if (!lastBlock) {
		cout << "\nThere is no block in the blockchain...\n";
		return vector<UTXO>();
	}
	
	vector<Transaction *> myTransactions;
	const Block * presentBlock = lastBlock;
	BYTE publicKey[SHA256_DIGEST_VALUELEN];
	SHA256_Encrpyt(privateKey, SHA256_DIGEST_VALUELEN, publicKey);

	// 블록체인에서 Transaction의 receiverPublicKey가 publicKey인 Transaction을 찾는다.
	for (uint64_t i = 0; i < blockCount; i++) {
		for (Transaction * tx : presentBlock->transactions) {
			for(const Output * output : tx->getOutputs()) {
				if (Block::isMemoryEqual(publicKey, output->getReceiverPublicKey(), SHA256_DIGEST_VALUELEN)) {
					myTransactions.push_back(tx);
					break;
				}
			}
		}
		presentBlock = presentBlock->previousBlock;
	}

	// 모든 Transaction에 대해 참조되지 않은 Transaction만 찾는다. O(n^2), n = myTransactions.size()
	vector<UTXO> myUTXOTable;
	for (const Transaction * myTx : myTransactions) {
		for (const Transaction * myTx2 : myTransactions) {
			for(const Input * input : myTx2->getInputs()) {
				if (Block::isMemoryEqual(myTx->getTransactionHash(), input->getPreviousTransactionHash(), SHA256_DIGEST_VALUELEN))
					goto REFERENCED;
			}
		}

		// 참조되지 않은 Transaction의 output만 myUTXOTable에 넣는다.
		for (const Output * output : myTx->getOutputs()) {
			if (Block::isMemoryEqual(publicKey, output->getReceiverPublicKey(), SHA256_DIGEST_VALUELEN)) {
				UTXO myUTXO(myTx->getTransactionHash(), myTx->blockIndex, output->getAmount());
				myUTXOTable.push_back(myUTXO);
			}
		}
		
		// 참조된 Transaction output은 Table에 넣지 않는다.
		REFERENCED:
		continue;
	}
	
	return myUTXOTable;
}

vector<UTXO> Blockchain::getUTXOTable(const BYTE * privateKey, string propertyType) const {
	return vector<UTXO>();
}

bool Blockchain::isUTXO(const Transaction * _tx) const {
	if (!lastBlock) {
		cout << "\nThere is no block in the blockchain...\n";
		return false;
	}

	const Block * presentBlock = lastBlock;
	for (uint64_t i = 0; i < blockCount - 1; i++) {
		for (const Transaction * tx : presentBlock->transactions) {
			for (const Input * input : tx->getInputs()) {
				if (Block::isMemoryEqual(_tx->getTransactionHash(), input->getPreviousTransactionHash(), SHA256_DIGEST_VALUELEN))
					return false;
			}
		}
		presentBlock = presentBlock->previousBlock;
	}

	for (const Transaction * tx : presentBlock->transactions) {
		for (const Input * input : tx->getInputs()) {
			if (Block::isMemoryEqual(_tx->getTransactionHash(), input->getPreviousTransactionHash(), SHA256_DIGEST_VALUELEN))
				return false;
		}
	}
	return true;
}

string Blockchain::timeToString(time_t t) {		// -> 분, 초는 2자리 고정에 비어있는 자리는 0으로 채우기...어떻게?
	struct tm * date = localtime(&t);
	string result = to_string(date->tm_year + 1900) + ". " + to_string(date->tm_mon + 1) + ". " + to_string(date->tm_mday)
		+ ". " + to_string(date->tm_hour) + ':' + to_string(date->tm_min) + ':' + to_string(date->tm_sec);
	return result;	
}