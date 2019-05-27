#include <vector>
#include <string>
#include <cstring>
#include "Wallet.h"
#include "Transaction.h"
using namespace std;

UTXO::UTXO(const BYTE * txHash, std::uint64_t _blockIndex, std::uint64_t _balance) 
	: blockIndex(_blockIndex),  balance(_balance) {
	memcpy(transactionHash, txHash, SHA256_DIGEST_VALUELEN);
}



Wallet::Wallet(std::string _passPhrase) : nonce(0) {
	SHA256_Encrpyt((BYTE *)_passPhrase.c_str(), _passPhrase.length(), privateKey);
	SHA256_Encrpyt(privateKey, SHA256_DIGEST_VALUELEN, publicKey);
}

//void Wallet::setInputsOuputs(Input & input, Output & output, const BYTE * receiverPublicKey, std::uint64_t _sendingAmount) {
//	uint64_t sendingAmount = _sendingAmount;
//
//	for (UTXO myUTXO : myUTXOTable) {
//		if (myUTXO.getBalance() < sendingAmount) {
//			
//		}
//
//	}
//}

Transaction * Wallet::createCoinbaseTransaction(uint64_t blockIndex, string type, string memo) {
	vector<Input *> inputs;
	vector<Output *> outputs;

	Input * input = new Input(privateKey, COINBASE_REWARD, blockIndex);
	inputs.push_back(input);

	Output * output = new Output(publicKey, COINBASE_REWARD);
	outputs.push_back(output);

	return new Transaction(inputs, outputs, type, nonce++, memo);
}

Transaction * Wallet::createTransaction(const BYTE * receiverPublicKey, std::string type, std::int64_t sendingAmount,
	std::int64_t fee, std::string memo) {
	vector<Input *> inputs;
	vector<Output *> outputs;

	// type 일치 여부 검사

	// 자기 꺼 UTXO 모으고 보냄
	//Input * input = new Input(privateKey, sendingAmount);



	// 거스름돈은 자기 지갑에 보냄
	//Output * output = new Output(publicKey, sendingAmount - fee);

	return new Transaction(inputs, outputs, type, nonce++, memo);
}


void Wallet::printMyUTXOTable() const
{
}

void Wallet::printUTXOTable() const
{
}
