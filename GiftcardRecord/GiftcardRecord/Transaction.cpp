#include <iostream>
#include <string>
#include <ctime>
#include <vector>
#include <cstring>
#include "Blockchain.h"
#include "Transaction.h"
#include "Giftcard.h"
using namespace std;

ostream & operator<<(ostream& o, const BYTE * hash);

// Assertion: parameter로 32byte의 문자열 입력
Input::Input(const BYTE * _senderPrivateKey, std::uint64_t _amount, const BYTE * _previousTransactionHash) : amount(_amount) {
	SHA256_Encrpyt(_senderPrivateKey, SHA256_DIGEST_VALUELEN, senderPublicKey);
	if(_previousTransactionHash)
		memcpy(previousTransactionHash, _previousTransactionHash, SHA256_DIGEST_VALUELEN);
	else {
		memset(previousTransactionHash, 0, SHA256_DIGEST_VALUELEN);
	}
}


// Assertion: parameter로 32byte의 문자열 입력
Output::Output(const BYTE * _receiverPublicKey, std::uint64_t _amount) : amount(_amount) {
	memcpy(receiverPublicKey, _receiverPublicKey, SHA256_DIGEST_VALUELEN);
}

//Transaction::Transaction(Output * _output, std::string _giftcardName, std::string _memo) 
//	: output(_output), giftcardName(_giftcardName), memo(_memo) {
//	hashing();
//}

Transaction::Transaction(vector<Input *> _inputs, vector<Output *> _outputs, Giftcard * _giftcard, string _memo) 
	: inputs(_inputs), outputs(_outputs), giftcard(_giftcard), memo(_memo) {
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
	delete giftcard;
}

// Assertion: getTransactionSize와 i의 최종 값은 동일해야 한다.
void Transaction::hashing() {
	const BYTE * txData = createTransactionData();

	SHA256_Encrpyt(txData, getTransactionDataSize(), transactionHash);
	delete[] txData;
}

bool Transaction::isValid(const BYTE * senderPrivateKey) const
{
	return false;
}

bool Transaction::isValidCoinbase() const
{
	return false;
}

// 반환된 포인터는 delete[]로 메모리 할당 해제가 필요함.
const BYTE * Transaction::createTransactionData() const {
	int i = 0;							// transactionData index
	BYTE * txData = new BYTE[getTransactionDataSize()];
	uint64_t amount;

	for(Input * input : inputs) {
		memcpy(txData + i, input->getPreviousTransactionHash(), SHA256_DIGEST_VALUELEN);
		i += SHA256_DIGEST_VALUELEN;

		memcpy(txData + i, input->getSenderPublicKey(), SHA256_DIGEST_VALUELEN);
		i += SHA256_DIGEST_VALUELEN;

		amount = input->getAmount();
		memcpy(txData + i, &amount, sizeof(input->getAmount()));
		i += sizeof(input->getAmount());
	}
	
	for(Output * output : outputs) {
		memcpy(txData + i, output->getReceiverPublicKey(), SHA256_DIGEST_VALUELEN);
		i += SHA256_DIGEST_VALUELEN;

		amount = output->getAmount();
		memcpy(txData + i, &amount, sizeof(output->getAmount()));
		i += sizeof(output->getAmount());
	}
	
	memcpy(txData + i, giftcard->getName().c_str(), giftcard->getName().length());
	i += giftcard->getName().length();

	memcpy(txData + i, &timestamp, sizeof(timestamp));
	i += sizeof(timestamp);

	return txData;
}

int Transaction::getTransactionDataSize() const {
	return inputs.size() * (sizeof(Input) - sizeof(std::uint64_t)) + outputs.size() * sizeof(Output)
		+ giftcard->getName().length() + sizeof(timestamp);
}

std::string Transaction::toString() const {


	return std::string();
}

void Transaction::print(ostream & o) const {
	o << "Transaction Hash: " << transactionHash << "\n";
	
	for (Input * input : inputs) {
		o << "\tSender's public key: " << input->getSenderPublicKey() << '\n';
		o << "\tSending amount: " << input->getAmount() << '\n';
		o << "\tPrevious transaction hash: " << input->getPreviousTransactionHash() << '\n';
		o << "\tPrevious transaction in " << input->getPreviousBlockIndex() << "th Block\n\n";
	}

	for (Output * output : outputs) {
		o << "\tReceiver's public key: " << output->getReceiverPublicKey() << '\n';
		o << "\tReceiving amount: " << output->getAmount() << "\n\n";
	}
	
	o << "Giftcard name: " << giftcard->getName() << '\n';
	o << "Timestamp: " << Blockchain::timeToString(timestamp) << '\n';
	o << "Memo: " << memo << '\n';
}



