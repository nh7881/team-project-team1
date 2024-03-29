#include <iostream>
#include <string>
#include <ctime>
#include <vector>
#include <cstring>
#include "Blockchain.h"
#include "Transaction.h"
#include "Giftcard.h"
#include "Block.h"
#include "PrintHash.h"
using namespace std;


// Coinbase input
Input::Input(uint64_t _amount, std::uint64_t _blockIndex) : previousBlockIndex(_blockIndex), amount(_amount) {
	memset(previousTransactionHash, 0, SHA256_DIGEST_VALUELEN);
	memset(senderPublicKey, 0, SHA256_DIGEST_VALUELEN);

}

// Assertion: parameter로 32byte의 문자열 입력
Input::Input(const BYTE * _senderPrivateKey, std::uint64_t _amount, const BYTE * _previousTransactionHash) : amount(_amount) {
	SHA256_Encrpyt(_senderPrivateKey, SHA256_DIGEST_VALUELEN, senderPublicKey);
	memcpy(previousTransactionHash, _previousTransactionHash, SHA256_DIGEST_VALUELEN);
}


/*-----------------------------------------------------------------------------------------*/


// Assertion: parameter로 32byte의 문자열 입력
Output::Output(const BYTE * _receiverPublicKey, std::uint64_t _amount) : amount(_amount) {
	memcpy(receiverPublicKey, _receiverPublicKey, SHA256_DIGEST_VALUELEN);
}


/*-----------------------------------------------------------------------------------------*/


Transaction::Transaction(const BYTE * privateKey, string _propertyType, uint64_t _nonce, string _memo)
	: propertyType(_propertyType), nonce(_nonce), memo(_memo) {
	timestamp = time(NULL);
	hashing();
}

Transaction::Transaction(vector<Input *> & _inputs, vector<Output *> & _outputs, string _propertyType, uint64_t _nonce, string _memo)
	: inputs(_inputs), outputs(_outputs), propertyType(_propertyType), nonce(_nonce), memo(_memo) {
	timestamp = time(NULL);
	hashing();
}

Transaction::~Transaction() {
	for(Input * input : inputs) {
		delete input;
	}
	for(Output * output : outputs) {
		delete output;
	}
}

void Transaction::hashing() {
	const BYTE * txData = createTransactionData();
	SHA256_Encrpyt(txData, getTransactionDataSize(), transactionHash);
	delete[] txData;
}

// 거래 금액과 보내는 사람의 유효성 확인
bool Transaction::isValid(const BYTE * senderPrivateKey) const {
	if (getTotalInputs() < getTotalOutputs()) {
		cout << "\nTransaction hash: " << transactionHash << " in " << blockIndex << "th block\n";
		cout << "Total outputs are bigger than total inputs.\n";
		cout << "The transaction is unvalid...\n";
		return false;
	}
		
	BYTE senderPublicKey[SHA256_DIGEST_VALUELEN];
	SHA256_Encrpyt(senderPrivateKey, SHA256_DIGEST_VALUELEN, senderPublicKey);
	for (Input * input : inputs) {
		if (!Block::isMemoryEqual(input->getSenderPublicKey(), senderPublicKey, SHA256_DIGEST_VALUELEN)) {
			cout << "\nTransaction hash: " << transactionHash << " in " << blockIndex << "th block\n";
			cout << "The sender's private key is not equivalent to the sender's public key in the transaction inputs.\n";
			cout << "Or all sender's public keys in the transaction inputs aren't same.\n";
			cout << "The transaction is unvalid...\n";
		}
		return false;
	}

	return true;
}

bool Transaction::isValidCoinbase() const {
	BYTE nullHash[SHA256_DIGEST_VALUELEN];
	memset(nullHash, 0, SHA256_DIGEST_VALUELEN);
	if (Block::isMemoryEqual(inputs[0]->getPreviousTransactionHash(), nullHash, SHA256_DIGEST_VALUELEN)) {
		// isValidCoinbase
		// coinbase reward 확인
		//

	}

	return false;
}

// 반환된 포인터는 delete[]로 메모리 할당 해제가 필요함.
const BYTE * Transaction::createTransactionData() const {
	int i = 0;											// transactionData index
	BYTE * txData = new BYTE[getTransactionDataSize()];	// Assertion: getTransactionSize와 i의 최종 값은 동일해야 한다.
	uint64_t amount, previousBlockIndex;

	for(Input * input : inputs) {
		memcpy(txData + i, input->getPreviousTransactionHash(), SHA256_DIGEST_VALUELEN);
		i += SHA256_DIGEST_VALUELEN;

		memcpy(txData + i, input->getSenderPublicKey(), SHA256_DIGEST_VALUELEN);
		i += SHA256_DIGEST_VALUELEN;

		amount = input->getAmount();
		memcpy(txData + i, &amount, sizeof(input->getAmount()));
		i += sizeof(input->getAmount());

		previousBlockIndex = input->getPreviousBlockIndex();
		memcpy(txData + i, &previousBlockIndex, sizeof(input->getPreviousBlockIndex()));
		i += sizeof(input->getPreviousBlockIndex());
	}
	
	for(Output * output : outputs) {
		memcpy(txData + i, output->getReceiverPublicKey(), SHA256_DIGEST_VALUELEN);
		i += SHA256_DIGEST_VALUELEN;

		amount = output->getAmount();
		memcpy(txData + i, &amount, sizeof(output->getAmount()));
		i += sizeof(output->getAmount());
	}
	
	memcpy(txData + i, propertyType.c_str(), propertyType.length());
	i += propertyType.length();

	memcpy(txData + i, &timestamp, sizeof(timestamp));
	i += sizeof(timestamp);

	return txData;
}

int Transaction::getTransactionDataSize() const {
	return inputs.size() * sizeof(Input) + outputs.size() * sizeof(Output)
		+ propertyType.length() + sizeof(timestamp);
}

int Transaction::getTotalInputs() const
{
	return 0;
}

int Transaction::getTotalOutputs() const
{
	return 0;
}

std::string Transaction::toString() const {


	return std::string();
}

void Transaction::print(ostream & o) const {
	o << "Transaction Hash: \t" << transactionHash << "\n";
	o << "Type: " << propertyType << '\n';
	o << "Timestamp: " << Blockchain::timeToString(timestamp) << '\n';

	for (Input * input : inputs) {
		o << "\nSender's public key:		" << input->getSenderPublicKey() << '\n';
		o << "Sending amount: " << input->getAmount() << '\n';
		o << "Previous transaction hash:	" << input->getPreviousTransactionHash() << '\n';
		o << "Previous transaction in " << input->getPreviousBlockIndex() << "th Block\n";
	}

	for (Output * output : outputs) {
		o << "\nReceiver's public key:		" << output->getReceiverPublicKey() << '\n';
		o << "Receiving amount: " << output->getAmount() << '\n';
	}
	
	o << "Memo: " << memo << '\n';
}



