#include <iostream>
#include "Blockchain.h"
#include "Transaction.h"
#include "Wallet.h"
using namespace std;

int main()
{
	Wallet w("qwerty");		// 사용자 지갑 생성

	vector<Input> inputs;	// Transaction Input(송신인, 서명, 금액)
	vector<Output> outputs;	// Transaction Output(수신인, 서명, 금액)

	Input i, i2, i3;
	Output o, o2, o3;
	
	inputs.push_back(i);
	inputs.push_back(i2);
	inputs.push_back(i3);

	outputs.push_back(o);
	outputs.push_back(o2);
	outputs.push_back(o3);

	Transaction * tx = w.createTransaction(inputs, outputs, "Blockchain Version", "Giftcard Name", "Memo");		// Transaction 발생
	Transaction * tx2 = w.createTransaction(inputs, outputs, "Blockchain Version", "Giftcard Name", "Memo");	// Transaction2 발생
	Transaction * tx3 = w.createTransaction(inputs, outputs, "Blockchain Version", "Giftcard Name", "Memo");	// Transaction3 발생
	Transaction * tx4 = w.createTransaction(inputs, outputs, "Blockchain Version", "Giftcard Name", "Memo");	// Transaction4 발생

	Blockchain bc("Giftcard 1.0", tx);	// Blockchain 생성 및 거래 추가
	bc.addTransaction(tx2);				// Blockchain에 거래2 추가
	
	bc.setVersion("Giftcard 1.1");		// Blockchain Version 변경
	bc.addTransaction(tx3);				// Blockchain에 거래3 추가
	bc.addTransaction(tx4);				// Blockchain에 거래4 추가

	bc.printAllBlockHash();
	bc.printAllMerkleHash();
	bc.printAllTransaction(cout);
	//bc.printWaitingBlock();

	bc.saveBlockchain();				// Blockchain을 txt 파일로 저장(출력)
	//bc.loadBlockchain();


	system("pause");
	return 0;
} 