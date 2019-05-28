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


Transaction * Wallet::createCoinbaseTransaction(uint64_t blockIndex, string propertyType, string memo) {
	vector<Input *> inputs;
	vector<Output *> outputs;

	Input * input = new Input(COINBASE_REWARD, blockIndex);
	inputs.push_back(input);

	Output * output = new Output(publicKey, COINBASE_REWARD);
	outputs.push_back(output);

	return new Transaction(inputs, outputs, propertyType, nonce++, memo);
}

Transaction * Wallet::createTransaction(const BYTE * receiverPublicKey, string propertyType, int64_t sendingAmount,
	int64_t fee, string memo) {
	vector<Input *> inputs;
	vector<Output *> outputs;

	// type 일치 여부 검사 - type에 해당하는 재화가 없으면 null return

	// 자기 꺼 UTXO 모으고 보냄
	//Input * input = new Input(privateKey, sendingAmount);



	// 거스름돈은 자기 지갑에 보냄
	//Output * output = new Output(publicKey, sendingAmount - fee);

	return new Transaction(inputs, outputs, propertyType, nonce++, memo);
}


int Wallet::getMyUTXOAmount(string _propertyType) const {
	return 99999;
}

void Wallet::printMyUTXOTable() const
{
}

void Wallet::printUTXOTable() const
{
}
