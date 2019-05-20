#include "Wallet.h"
#include "Transaction.h"
#include "KISA_SHA256.h"

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

inline void Wallet::printMyUTXOTable() const
{
}

void Wallet::printUTXOTable() const
{
}
