#include "Wallet.h"
#include "KISA_SHA256.h"


Wallet::Wallet(std::string _passPhrase) {
	SHA256_Encrpyt((BYTE *)_passPhrase.c_str(), _passPhrase.length(), privateKey);
	SHA256_Encrpyt(privateKey, SHA256_DIGEST_VALUELEN, publicKey);
}


