#pragma once
#ifndef WALLET_H
#define WALLET_H
#include <vector>
#include <cstdint>
#include "KISA_SHA256.h"

class MyUTXO {
	BYTE transactionHash[SHA256_DIGEST_VALUELEN];
	std::uint64_t blockIndex;						// 지갑


};


class Wallet {
	BYTE privateKey[SHA256_DIGEST_VALUELEN];
	BYTE publicKey[SHA256_DIGEST_VALUELEN];

	std::vector<MyUTXO> myUTXOTable;

public:
	Wallet();

	int getMyUTXO;


};

#endif