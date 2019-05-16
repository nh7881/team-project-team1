#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <fstream>
#include <cstdint>
#include <iomanip>
#include <cstdint>
#include <thread>
#include <mutex>
#include <condition_variable>
#include "BlockChain.h"
#include "Block.h"
#include "Transaction.h"
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
	_genesisBlock->tx.reserve(MAX_TRANSACTION_COUNT);
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
	//m.lock();
	transactionPool.push(_tx);
	//m.unlock();
	//cv.notify_one();

	// transactionPool에 처리되지 않은 transaction이 많으면
	if (transactionPool.size() >= MAX_TRANSACTION_COUNT) {
		//unique_lock<mutex> lk(m);
		//cv.wait(lk, [&] { return !transactionPool.empty(); });

		waitingBlock->addTransactionsFrom(transactionPool);
		//lk.unlock();
		
		waitingBlock->version = version;		// 현재 blockchain version 복사
		waitingBlock->initializeMerkleHash();	// transaction으로 merkletree의 merkleroot(=merklehash) 계산
		waitingBlock->mining();					// waiting block을 채굴
		addBlock(waitingBlock);					// waiting block을 blockchain에 추가

		Block * newWaitingBlock = new Block(lastBlock);
		waitingBlock = newWaitingBlock;
	}
}

//void Blockchain::consumeTransaction() {
//	// waiting block의 transaction이 꽉차면
//	while (transactionPool.size() >= MAX_TRANSACTION_COUNT) {
//		unique_lock<mutex> lk(m);
//		cv.wait(lk, [&] { return !transactionPool.empty(); }); // || *num_processed == 25; });
//
//		Block * newBlock = new Block();
//		newBlock->addTransactionsFrom(transactionPool);
//		lk.unlock();
//
//		newBlock->initializeMerkleHash();	// transaction으로 merkletree의 merkleroot(=merklehash) 계산
//		newBlock->mining();					// waiting block을 채굴
//		addBlock(newBlock);					// waiting block을 blockchain에 추가
//	}	
//}

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
			size_t txSize = presentBlock->tx.size();
			for (size_t j = 0; j < txSize; j++) {
				o << "\nTransaction #" << j + 1 << '\n';
				presentBlock->tx[j]->print(o);
			}
			o << '\n';
			presentBlock = presentBlock->previousBlock;
		} else {
			cout << "Digital forgery had occured in " << presentBlock->blockIndex << "th block...";
			break;
		}
	}
}

void Blockchain::saveBlockchain() const {
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
			presentBlock->tx[j]->print(fout);
		}
		presentBlock = presentBlock->previousBlock;
	}

	cout << "\n\nFile write complete!\n";
}

void Blockchain::loadBlockchain() {





}






string Blockchain::timeToString(time_t t) {		// -> 분, 초는 2자리 고정에 비어있는 자리는 0으로 채우기...어떻게?
	struct tm * date = localtime(&t);
	string result = to_string(date->tm_year + 1900) + ". " + to_string(date->tm_mon + 1) + ". " + to_string(date->tm_mday)
		+ ". " + to_string(date->tm_hour) + ':' + to_string(date->tm_min) + ':' + to_string(date->tm_sec);
	return result;	
}


//
//// 블록의 order를 반환한다.
//const Order * Blockchain::get_order(time_t _order_timestamp) const {
//	if (last_block == NULL) {
//		std::cout << "There is no history of your order...\n";
//		return NULL;
//	}
//
//	// 마지막 블록부터 순차적으로 블록을 찾는다.
//	const Block * block = last_block;
//	while (block->get_timestamp() >= _order_timestamp) {
//		block = block->get_previous_block();
//	}
//
//	// 찾은 블록 안에서 이진탐색으로 원하는 order를 찾는다.
//	for (std::int32_t low = 0, high = block->get_order().size() - 1; low <= high;) {
//		std::int32_t mid = ((unsigned int)low + (unsigned int)high) >> 1;
//		time_t midVal = block->get_order()[mid]->get_timestamp();
//
//		if (midVal < _order_timestamp)
//			low = mid + 1;
//		else if (midVal > _order_timestamp)
//			high = mid - 1;
//		else
//			return block->get_order()[mid]; // order found
//	}
//
//	return NULL; // order not found.
//}