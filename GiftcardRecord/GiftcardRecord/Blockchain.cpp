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
Blockchain::Blockchain(string _version) : blockCount(0), version(_version) {
	Block * _genesisBlock = new Block();
	_genesisBlock->transactions.reserve(MAX_TRANSACTION_COUNT);
	genesisBlock = _genesisBlock;
	lastBlock = NULL;
	waitingBlock = _genesisBlock;
}

// Genesis block을 생성하고 그 블록에 Transaction을 추가한다.
Blockchain::Blockchain(string _version, Transaction * _tx) : Blockchain(_version) {
	addTransaction(_tx);
}

// transaction pool에 transaction을 추가한다.
void Blockchain::addTransaction(Transaction * _tx) {
	transactionPool.push(_tx);

	// transactionPool에 처리되지 않은 transaction이 많으면
	if (transactionPool.size() >= MAX_TRANSACTION_COUNT) {
		waitingBlock->version = version;		// 현재 blockchain version 복사
		waitingBlock->addTransactionsFrom(transactionPool);		
		waitingBlock->initializeMerkleHash();	// transaction으로 merkletree의 merkleroot(=merklehash) 계산
		waitingBlock->mining();					// waiting block을 채굴
		addBlock(waitingBlock);					// waiting block을 blockchain에 추가

		Block * newWaitingBlock = new Block(lastBlock);
		waitingBlock = newWaitingBlock;
	}
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
	time_t t = time(NULL);
	struct tm * date = localtime(&t);
	string filename = lastBlock->version + ' ' + to_string(date->tm_year + 1900) + ". " + to_string(date->tm_mon + 1)
		+ ". " + to_string(date->tm_mday) + '.' /*+ to_string(date->tm_hour) + '.' + to_string(date->tm_min)*/ + ".txt";

	ofstream fout(filename);
	const Block * presentBlock = lastBlock;

	if (!fout.is_open()) {
		cout << "File I/O error...\n";
		return;
	}

	fout << "Blockchain current version: " << presentBlock->version << '\n';
	for (uint64_t i = 0; i < blockCount; i++) {
		fout << "----------------------------------------------------------\n";
		fout << "Block #" << presentBlock->blockIndex << '\n';
		fout << "Block Hash: \t\t" << presentBlock->blockHash << '\n';
		
		fout << "\nVersion: " << presentBlock->version << '\n';
		if (presentBlock->previousBlock) {	// if (presentBlock->previousBlock != NULL)
			fout << "Previous Block Hash: \t" << presentBlock->previousBlock->blockHash << '\n';
		} else {
			fout << "Previous Block Hash: \t0x0000000000000000000000000000000000000000000000000000000000000000" << '\n';
		}
		fout << "Merkle Hash: \t\t" << presentBlock->merkleHash << '\n';
		fout << "Timestamp: " << timeToString(presentBlock->timestamp) << '\n';
		fout << "Bits: " << presentBlock->bits << '\n';
		fout << "Nonce: " << presentBlock->nonce << '\n';

		for (unsigned int j = 0; j < MAX_TRANSACTION_COUNT; j++) {
			fout << "\nTransaction #" << j + 1 << '\n';
			presentBlock->transactions[j]->print(fout);
		}
		presentBlock = presentBlock->previousBlock;
	}

	cout << "\n\nFile write complete!\n";
}

std::vector<UTXO> Blockchain::getUTXOTable() const
{
	return std::vector<UTXO>();
}

// Private Key를 받아 해당 key의 사용되지 않은 거래를 찾아서 반환한다.
vector<UTXO> Blockchain::getUTXOTable(const BYTE * privateKey) const {
	vector<Transaction *> myTransactions;
	
	const Block * presentBlock = lastBlock;
	BYTE publicKey[SHA256_DIGEST_VALUELEN];
	SHA256_Encrpyt(privateKey, SHA256_DIGEST_VALUELEN, publicKey);

	// 블록체인에서 Transaction의 receiverPublicKey가 publicKey인 Transaction을 찾는다.
	for (uint64_t i = 0; i < blockCount; i++) {
		for (Transaction * tx : presentBlock->getTransactions()) {
			for(Output * output : tx->getOutputs()) {
				if (Block::isMemoryEqual(publicKey, output->getReceiverPublicKey(), SHA256_DIGEST_VALUELEN)) {
					myTransactions.push_back(tx);
					break;
				}
			}
		}
		presentBlock = presentBlock->previousBlock;
	}

	// 모든 Transaction에 대해 참조되지 않은 Transaction만 찾는다. O(n^2) - n은 myTxSize.
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


string Blockchain::timeToString(time_t t) {		// -> 분, 초는 2자리 고정에 비어있는 자리는 0으로 채우기...어떻게?
	struct tm * date = localtime(&t);
	string result = to_string(date->tm_year + 1900) + ". " + to_string(date->tm_mon + 1) + ". " + to_string(date->tm_mday)
		+ ". " + to_string(date->tm_hour) + ':' + to_string(date->tm_min) + ':' + to_string(date->tm_sec);
	return result;	
}