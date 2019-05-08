#include <string>
#include <ctime>
#include "Transaction.h"
using namespace std;

Transaction::Transaction(string _who, string _what, string _why, time_t _when) : who(_who), what(_what), why(_why), when(_when) {}


// 반환된 포인터는 delete[]로 메모리 할당 해제가 필요함. Transaction 클래스 구조에 따라 아래 코드 내용이 달라질 수 있음.
const BYTE * Transaction::createTransactionData() const {
	size_t i = 0;	// index of transactionData
	size_t j;		// index of who, when, what
	const BYTE * pb;
	
	BYTE * transactionData = new BYTE[getTransactionLength()];

	for (j = 0; j < who.length(); i++, j++)
		transactionData[i] = who[j];

	pb = (BYTE *)&when;
	for (j = 0; j < sizeof(when); i++, j++)
		transactionData[i] = pb[j];

	for (j = 0; j < what.length(); i++, j++)
		transactionData[i] = what[j];

	return transactionData;
}

