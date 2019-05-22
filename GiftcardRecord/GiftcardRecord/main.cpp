#include <iostream>
#include "Blockchain.h"
#include "Transaction.h"
#include "Wallet.h"
#include "Giftcard.h"
using namespace std;

int main()
{
	//---------------------- Test ---------------------
	Wallet w1_("w1");
	Wallet w2_("w2");
	Blockchain bc_("Giftcard Blockchain 1.0");
	
	Transaction * tx_ = w2_.createCoinbaseTransaction(new Giftcard("Giftcard Name"), 100, 0, "Memo");
	Transaction * tx2_ = w2_.createTransaction(w1_.getPublicKey(), new Giftcard("Giftcard Name"), 10, 0, "Memo");
	Transaction * tx3_ = w2_.createTransaction(w1_.getPublicKey(), new Giftcard("Giftcard Name"), 10, 0, "Memo");
	Transaction * tx4_ = w2_.createTransaction(w1_.getPublicKey(), new Giftcard("Giftcard Name"), 10, 0, "Memo");


	bc_.addTransaction(tx_);
	bc_.addTransaction(tx2_);

	bc_.setVersion("Giftcard Blockchain 1.1");
	bc_.addTransaction(tx3_);
	bc_.addTransaction(tx4_);


	bc_.printAllBlockHash();
	bc_.printAllMerkleHash();
	bc_.printAllTransaction(cout);
	//bc.printWaitingBlock();

//	bc_.printBlockchain(cout);				// Blockchain을 txt 파일로 저장(출력)
	//bc.loadBlockchain();


	//---------------------- 노드 생성 ----------------------
	/* 블록체인 노드 */
	Blockchain bc("Giftcard 1.0");		// Blockchain 생성 및 거래 추가

	/* 상품권 구매자 */
	Wallet w1("giftcard buyer pass phrase");		// 사용자 지갑 생성

	/* 상품권 판매자 */
	Wallet w2("giftcard seller pass phrase");

	/* 블록체인 채굴자 */
	Wallet w3("blockchain miner pass phrase");


	//---------------------- 발행된 상품권 조회 ----------------------
	/* 사용자가 블록체인 노드에게 UTXO Table 요청 */
	vector<UTXO> utxoTable = bc.getUTXOTable();
	w1.setUTXOTable(utxoTable);
	w1.printUTXOTable();
	

	//---------------------- 상품권 발행 및 판매 ----------------------

	/* 판매자의 첫 거래 생성 */
	Transaction * tx = w2.createCoinbaseTransaction(new Giftcard("Giftcard Name"), 100, 0, "Memo"); // 상품권 종류, 보낼 금액, 수수료, 메모

	/* 판매자가 블록체인 노드에 거래와 개인키 전송 */
	if (tx->isValidCoinbase()) {
		bc.addTransaction(tx);

		/* 판매자에게 메시지 전송 */
		cout << "상품권 발행 성공!\n";		
	}
	else {
		/* 판매자에게 메시지 전송 */
		cout << "잘못된 거래 내역\n";
	}
	

	//---------------------- 자신의 상품권 목록 요청 ----------------------
	/* 블록체인 노드에게 자신의 개인키 전송 후 UTXO Table 얻음 */
	vector<UTXO> myUTXOTable = bc.getUTXOTable(w1.getPrivateKey());

	/* 지갑에 자신의 UTXO 리스트 저장 */
	w1.setMyUTXOTable(myUTXOTable);
	w1.printMyUTXOTable();


	//---------------------- 상품권 거래 ----------------------
	/* 상품권 거래 생성 */
	Transaction * tx2 = w2.createTransaction(w1.getPublicKey(), new Giftcard("Giftcard Name"), 1, 0, "Memo"); // 받는 사람, 상품권 종류, 보낼 금액, 수수료, 메모 

	/* 블록체인 노드에 거래 전송 */
	if (tx2->isValid(w2.getPrivateKey())) {
		bc.addTransaction(tx2);

		/* w2에게 메시지 전송 */
		cout << "판매 성공!\n";

		/* w1에게 메시지 전송 */
		cout << "구매 성공!\n";
	}
	else {
		/* 판매자에게 메시지 전송 */
		cout << "잘못된 거래 내역\n";
	}






	system("pause");
	return 0;
} 