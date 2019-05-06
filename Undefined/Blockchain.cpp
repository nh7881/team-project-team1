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

// Genesis block�� �����Ѵ�.
Blockchain::Blockchain() {
	Block * _genesisBlock = new Block();
	_genesisBlock->tx.reserve(MAX_TRANSACTION_COUNT);
	genesisBlock = _genesisBlock;
	waitingBlock = _genesisBlock;
	lastBlock = NULL;
}

// Genesis block�� �����ϰ� �� ��Ͽ� Transaction�� �߰��Ѵ�.
Blockchain::Blockchain(Transaction * _tx) : Blockchain() {
	addTransaction(_tx);
}

// waiting block�� transaction�� �߰��Ѵ�.
void Blockchain::addTransaction(Transaction * _tx) {
	waitingBlock->addTransaction(_tx);

	// waiting block�� transaction�� ������
	if (waitingBlock->isFull()) {
		waitingBlock->findMerkleHash();		// transaction���� merkletree�� merkleroot(=merklehash) ���
		waitingBlock->mining();				// waiting block�� ä��
		addBlock(waitingBlock);				// waiting block�� blockchain�� �߰�

		Block * newWaitingBlock = new Block(lastBlock);		// new waiting block ����
		newWaitingBlock->tx.reserve(MAX_TRANSACTION_COUNT);	// transaction vector ũ�� ����
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

		// �Ʒ� �ڵ�� Transaction Ŭ���� ������ ���� �޶��� �� ����.
		for (unsigned int j = 0; j < MAX_TRANSACTION_COUNT; j++) {
			fout << "Transaction #" << j + 1 << '\n';
			fout << "who: " << presentBlock->tx[j]->who << '\n';
			fout << "when: " << timeToString(presentBlock->tx[j]->when) << '\n'; // ��¥�� ����?
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

// ����� ��� order ������ ����Ѵ�.
//void Blockchain::print_all_order() {
//	if (last_block == NULL) {
//		std::cout << "There is no history of your order...\n";
//		return;
//	}
//
//	// ������ ��Ϻ��� ù ��ϱ���, ��� ��Ͽ� ���Ͽ�
//	const Block * block = last_block;
//	for (std::uint64_t i = num_of_block; i > 0; i--, block = block->get_previous_block()) {
//		// ����� ��ȿ�� ����̸�
//		if (block->is_valid_block() && block->is_valid_order()) {
//			std::cout << "\n\nBlockhash: ";
//			printSHA256(block->get_blockhash());
//			std::cout << "Orderhash: ";
//			printSHA256(block->get_orderhash());
//
//			std::size_t size = block->get_order().size();
//			// �� ����� ��� order�� ���
//			for (std::int32_t j = size - 1; j > -1; j--)
//				block->get_order()[j]->print();
//		}
//		// ��ȿ���� ������ �޼��� ���
//		else {
//			std::cout << "Digital forgery had occured in " << i << "th block...";
//			break;
//		}
//	}
//}
//
//// ����� order�� ��ȯ�Ѵ�.
//const Order * Blockchain::get_order(time_t _order_timestamp) const {
//	if (last_block == NULL) {
//		std::cout << "There is no history of your order...\n";
//		return NULL;
//	}
//
//	// ������ ��Ϻ��� ���������� ����� ã�´�.
//	const Block * block = last_block;
//	while (block->get_timestamp() >= _order_timestamp) {
//		block = block->get_previous_block();
//	}
//
//	// ã�� ��� �ȿ��� ����Ž������ ���ϴ� order�� ã�´�.
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