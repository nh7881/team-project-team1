#include <iostream>
#include <string>
#include <ctime>
#include <vector>
#include "Transaction.h"
#include "Blockchain.h"
#include "Wallet.h"
#include "Giftcard.h"
using namespace std;

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

std::string Transaction::toString() const {


	return std::string();
}

//void Transaction::print(ostream & o) const {
//	o << "Who: " << who << '\n';
//	o << "When: " << Blockchain::timeToString(timestamp) << '\n';
//	o << "What: " << what << '\n';
//	o << "Why: " << why << '\n';
//}



