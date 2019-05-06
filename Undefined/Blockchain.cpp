#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <fstream>
#include <cstdint>
#include <iomanip>
#include "BlockChain.h"
#include "Block.h"
#include "Transaction.h"
using namespace std;

uint64_t Blockchain::blockCount = 0;

ostream & operator<<(ostream& o, const BYTE * hash) {
	o << "0x";
	for (int i = 0; i < 32; i++) {
		o.width(2);
		o.fill('0');
		o << hex << (int)hash[i];
	}
	o << dec;
	return o;
}

// Genesis block을 생성한다.
Blockchain::Blockchain() {
	Block * _genesisBlock = new Block();
	_genesisBlock->tx.reserve(MAX_TRANSACTION_COUNT);
	genesisBlock = _genesisBlock;
	waitingBlock = _genesisBlock;
	lastBlock = NULL;
}

// Genesis block을 생성하고 그 블록에 Transaction을 추가한다.
Blockchain::Blockchain(Transaction * _tx) : Blockchain() {
	addTransaction(_tx);
}

// waiting block에 transaction을 추가한다.
void Blockchain::addTransaction(Transaction * _tx) {
	waitingBlock->addTransaction(_tx);

	// waiting block의 transaction이 꽉차면
	if (waitingBlock->isFull()) {
		waitingBlock->findMerkleHash();		// transaction으로 merkletree의 merkleroot(=merklehash) 계산
		waitingBlock->mining();				// waiting block을 채굴
		addBlock(waitingBlock);				// waiting block을 blockchain에 추가

		Block * newWaitingBlock = new Block(lastBlock);		// new waiting block 생성
		newWaitingBlock->tx.reserve(MAX_TRANSACTION_COUNT);	// transaction vector 크기 예약
		waitingBlock = newWaitingBlock;
	}
}

void Blockchain::printAllBlockHash() const {
	const Block * presentBlock = lastBlock;
	for (unsigned int i = 0; i < blockCount; i++) {
		cout << "Block Hash: 0x";
		for (int j = 0; j < 32; j++) {
			printf("%02x", presentBlock->blockHash[j]);
		}
		presentBlock = presentBlock->previousBlock;
		cout << '\n';
	}
}

void Blockchain::printAllMerkleHash() const {
	const Block * presentBlock = lastBlock;
	for (unsigned int i = 0; i < blockCount; i++) {
		cout << "Merkle Hash: 0x";
		for (int j = 0; j < 32; j++) {
			printf("%02x", presentBlock->merkleHash[j]);
		}
		presentBlock = presentBlock->previousBlock;
		cout << '\n';
	}
}

// must specify numeral system(dec, hex, ...) in I/O because numeral system is often changed. 
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

	fout << "Blockchain version: " << presentBlock->version << '\n';
	for (uint64_t i = 0; i < blockCount; i++) {
		fout << "----------------------------------------------------------\n";
		fout << "Block #" << blockCount - i - 1 << '\n';
		fout << "Block Hash: \t\t" << presentBlock->blockHash << '\n';

		if (presentBlock->previousBlock != NULL) {
			fout << "Previous Block Hash: \t" << presentBlock->previousBlock->blockHash << '\n';
		}
		else {
			fout << "Previous Block Hash: \t0x0000000000000000000000000000000000000000000000000000000000000000" << '\n';
		}

		fout << "Merkle Hash: \t\t" << presentBlock->merkleHash << '\n';
		fout << "Timestamp: " << timeToString(presentBlock->timestamp) << '\n';
		fout << "Bits: " << presentBlock->bits << '\n';
		fout << "Nonce: " << presentBlock->nonce << "\n\n";

		// 아래 코드는 Transaction 클래스 구조에 따라 달라질 수 있음.
		for (unsigned int j = 0; j < MAX_TRANSACTION_COUNT; j++) {
			fout << "Transaction #" << j + 1 << '\n';
			fout << "who: " << presentBlock->tx[j]->who << '\n';
			fout << "when: " << timeToString(presentBlock->tx[j]->when) << '\n'; // 날짜로 지정?
			fout << "what: " << presentBlock->tx[j]->what << '\n';
			fout << "why: " << presentBlock->tx[j]->why << '\n';
		}
		presentBlock = presentBlock->previousBlock;
	}

	cout << "\n\nFile write complete!\n";
}

void Blockchain::loadBlockchain() {

}

void Blockchain::printAllTransaction() const {





}


const Block * Blockchain::getGenesisBlock() const {
	return genesisBlock;
}

const Block * Blockchain::getLastBlock() const {
	return lastBlock;
}

uint64_t Blockchain::getBlockCount() const {
	return blockCount;
}

string Blockchain::timeToString(time_t t) const {
	struct tm * date = localtime(&t);
	string result = to_string(date->tm_year + 1900) + ". " + to_string(date->tm_mon + 1) + ". " + to_string(date->tm_mday)
		+ ". " + to_string(date->tm_hour) + ':' + to_string(date->tm_min) + ':' + to_string(date->tm_sec);
	return result;
}

// 블록의 모든 order 정보를 출력한다.
//void Blockchain::print_all_order() {
//	if (last_block == NULL) {
//		std::cout << "There is no history of your order...\n";
//		return;
//	}
//
//	// 마지막 블록부터 첫 블록까지, 모든 블록에 대하여
//	const Block * block = last_block;
//	for (std::uint64_t i = num_of_block; i > 0; i--, block = block->get_previous_block()) {
//		// 블록이 유효한 블록이면
//		if (block->is_valid_block() && block->is_valid_order()) {
//			std::cout << "\n\nBlockhash: ";
//			printSHA256(block->get_blockhash());
//			std::cout << "Orderhash: ";
//			printSHA256(block->get_orderhash());
//
//			std::size_t size = block->get_order().size();
//			// 그 블록의 모든 order를 출력
//			for (std::int32_t j = size - 1; j > -1; j--)
//				block->get_order()[j]->print();
//		}
//		// 유효하지 않으면 메세지 출력
//		else {
//			std::cout << "Digital forgery had occured in " << i << "th block...";
//			break;
//		}
//	}
//}
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