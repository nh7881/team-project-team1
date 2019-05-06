#include <string>
#include <ctime>
#include "Transaction.h"
using namespace std;

Transaction::Transaction(string _who, time_t _when, string _what, string _why) : who(_who), when(_when), what(_what), why(_why) {}


// ��ȯ�� �����ʹ� delete[]�� �޸� �Ҵ� ������ �ʿ���. transaction ������ ���� �ڵ� ������ �޶��� �� ����.
const BYTE * Transaction::getTransactionData() const {
	size_t i;		// index of tx ���� 
	size_t j;		// index of who, when, what
	size_t k = 0;	// index of transactionData
	const BYTE * pb;
	
	BYTE * transactionData = new BYTE[getTransactionLength()];

	for (j = 0; j < who.length(); k++, j++)
		transactionData[k] = who[j];

	pb = (BYTE *)&when;
	for (j = 0; j < sizeof(when); k++, j++)
		transactionData[k] = pb[j];

	for (j = 0; j < what.length(); k++, j++)
		transactionData[k] = what[j];

	return transactionData;
}

