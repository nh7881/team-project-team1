#include <iostream>
#include "Blockchain.h"
#include "Transaction.h"
#include "Wallet.h"
using namespace std;

int main()
{
	// 블록체인 노드
	Blockchain bc("Giftcard 1.0");		// Blockchain 생성 및 거래 추가

	// 상품권 구매자
	Wallet w1("giftcard buyer pass phrase");		// 사용자 지갑 생성

	// 상품권 판매자
	Wallet w2("giftcard seller pass phrase");

	// 블록체인 채굴자
	Wallet w3("blockchain miner pass phrase");


	//---------------------- 발행된 상품권 조회 ----------------------
	/* 사용자가 블록체인 노드에게 UTXO Table 요청 */
	vector<UTXO> utxoTable = bc.getUTXOTable();
	w1.setUTXOTable(utxoTable);
	w1.printUTXOTable();
	

	//---------------------- 상품권 발행 및 판매 ----------------------
	/* 판매자의 첫 거래 생성 */
	Output o(w2.getPublicKey(), 10);

	vector<Output> outputs;	// Transaction Output(수신인, 서명, 금액)
	outputs.push_back(o);

	Transaction * tx = new Transaction(outputs, "Giftcard Name", "Memo");

	/* 판매자의 수수료 거래 생성 */
	Input i2(w2.getPrivateKey(), 10, tx->getTransactionHash());
	Output o2(w3.getPublicKey(), 1);
	Output o2_2(w2.getPublicKey(), 9);
	vector<Input> inputs2;
	vector<Output> outputs2;	// Transaction Output(수신인, 서명, 금액)

	inputs2.push_back(i2);
	outputs2.push_back(o2);
	outputs2.push_back(o2_2);
	
	Transaction * tx2 = new Transaction(inputs2, outputs2, "Giftcard Name", "Memo");

	/* 판매자가 블록체인 노드에 거래와 개인키 전송 */
	if (tx->isValidCoinbase() && tx2->isValid(w2.getPrivateKey())) {	// 서버에 개인키 암호화되어 저장되겠지?
		bc.addTransaction(tx);
		bc.addTransaction(tx2);

		/* 판매자에게 메시지 전송 */
		cout << "판매 성공!\n";
		
		/* 구매자에게 메시지 전송 */
		cout << "구매 성공!\n";
	}
	else {
		/* 판매자에게 메시지 전송 */
		cout << "잘못된 거래 내역\n";
	}
	

	//---------------------- 자신의 상품권 목록 요청 ----------------------
	/* 블록체인 노드에게 자신의 공개키 전송 후 UTXO Table 얻음 */
	vector<UTXO> myUTXOTable = bc.getUTXOTable(w1.getPublicKey());

	/* 지갑에 자신의 UTXO 리스트 저장 */
	w1.setMyUTXOTable(myUTXOTable);
	w1.printMyUTXOTable();


	//---------------------- 상품권 거래 ----------------------
	vector<Input> inputs3;	// Transaction Input(송신인, 서명, 금액)
	vector<Output> outputs3;	// Transaction Output(수신인, 서명, 금액)

	Input i3(w1.getPrivateKey(), 10, w1.getMyUTXOTable());
	Output o3(w2.getPublicKey(), 5);
	Output o3_2(w2.getPublicKey(), 3);
	Output o3_3(w1.getPublicKey(), 2);	// 거스름 돈
	
	inputs3.push_back(i3);

	outputs3.push_back(o3);
	outputs3.push_back(o3_2);
	outputs3.push_back(o3_3);

	Transaction * tx3 = new Transaction(inputs3, outputs3, "Giftcard Name", "Memo");	// Transaction 발생
	
	/* 블록체인 노드에 거래 전송 */
	if (tx3->isValid(w1.getPrivateKey())) {
		bc.addTransaction(tx3);

		/* 판매자에게 메시지 전송 */
		cout << "판매 성공!\n";

		/* 구매자에게 메시지 전송 */
		cout << "구매 성공!\n";
	}
	else {
		/* 판매자에게 메시지 전송 */
		cout << "잘못된 거래 내역\n";
	}




	//-----------------------------------------------------
	
	bc.setVersion("Giftcard 1.1");		// Blockchain Version 변경

	bc.printAllBlockHash();
	bc.printAllMerkleHash();
	bc.printAllTransaction(cout);
	//bc.printWaitingBlock();

	bc.saveBlockchain();				// Blockchain을 txt 파일로 저장(출력)
	//bc.loadBlockchain();


	system("pause");
	return 0;
} 