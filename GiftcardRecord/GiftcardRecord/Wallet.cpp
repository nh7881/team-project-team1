#include "Wallet.h"
#include "KISA_SHA256.h"

UTXO::UTXO() {}

UTXO::UTXO(const BYTE * txHash, std::uint64_t _blockIndex) : blockIndex(_blockIndex) {
	memcpy(transactionHash, txHash, SHA256_DIGEST_VALUELEN);
}

void UTXO::setTransactionHash(const BYTE * _txHash) {
	memcpy(transactionHash, _txHash, SHA256_DIGEST_VALUELEN);
}

void UTXO::setBlockIndex(std::uint64_t _blockIndex) {
	blockIndex = _blockIndex;
}


Wallet::Wallet(std::string _passPhrase) {
	SHA256_Encrpyt((BYTE *)_passPhrase.c_str(), _passPhrase.length(), privateKey);
	SHA256_Encrpyt(privateKey, SHA256_DIGEST_VALUELEN, publicKey);
}

Transaction * Wallet::createTransaction(std::vector<Input>& inputs, std::vector<Output>& outputs, std::string giftcardName, std::string memo) const {
	



	return nullptr;
}
