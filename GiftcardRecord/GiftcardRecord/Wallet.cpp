#include <vector>
#include <string>
#include "Wallet.h"
#include "Transaction.h"
//#include "KISA_SHA256.h"
using namespace std;

UTXO::UTXO(const BYTE * txHash, std::uint64_t _blockIndex, std::uint64_t _balance) 
	: blockIndex(_blockIndex),  balance(_balance) {
	memcpy(transactionHash, txHash, SHA256_DIGEST_VALUELEN);
}



Wallet::Wallet(std::string _passPhrase) {
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

Transaction * Wallet::createCoinbaseTransaction(Giftcard * giftcard, std::int64_t sendingAmount, 
	std::int64_t fee, std::string memo) const {
	vector<Input *> inputs;
	vector<Output *> outputs;

	Input * input = new Input(privateKey, sendingAmount);
	inputs.push_back(input);

	Output * output = new Output(publicKey, sendingAmount - fee);
	outputs.push_back(output);

	Transaction * tx = new Transaction(inputs, outputs, giftcard, memo);
}

Transaction * Wallet::createTransaction(const BYTE * receiverPublicKey, Giftcard * giftcard, std::int64_t sendingAmount,
	std::int64_t fee, std::string memo) const {
	vector<Input *> inputs;
	vector<Output *> outputs;


	// ÀÚ±â ²¨ UTXO ¸ðÀ¸°í º¸³¿
	//Input * input = new Input(privateKey, sendingAmount);



	// °Å½º¸§µ·Àº ÀÚ±â Áö°©¿¡ º¸³¿
	//Output * output = new Output(publicKey, sendingAmount - fee);

	Transaction * tx = new Transaction(inputs, outputs, giftcard, memo);

}


void Wallet::printMyUTXOTable() const
{
}

void Wallet::printUTXOTable() const
{
}
