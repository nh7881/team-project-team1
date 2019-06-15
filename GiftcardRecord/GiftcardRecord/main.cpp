#include <iostream>
#include <fstream>
#include <cstdlib>
#include <vector>
#include "Blockchain.h"
#include "Transaction.h"
#include "Wallet.h"
#include "Giftcard.h"
#include "PrintHash.h"
using namespace std;

int main()
{
	//---------------------- Test ---------------------	
	string password;
	Wallet w1_("receiver");
	cout << "Test receiver wallet was created\n\n";

	cout << "Input password: ";
	cin >> password;
	Wallet w2_(password);
	cout << "My wallet was created\n\n";

	Blockchain bc_("Blockchain 1.0", &w2_);
	cout << bc_.getVersion() << " was created\n\n";

	vector<Giftcard *> giftcardList;
	giftcardList.push_back(new Giftcard("Love card", 10, 9));
	giftcardList.push_back(new Giftcard("Like card", 5, 4));
	giftcardList.push_back(new Giftcard("SSum card", 3, 2));
	giftcardList.push_back(new Giftcard("Favorite card", 2, 1));
	

HOME://complete
	{
		system("pause");
		system("cls");
		int select;
		cout << "0.Home    1.Buy    2.Mining    3.Blockchain    4.My Wallet    \n";
		cin >> select;

		switch (select) {
		case 0:
			goto HOME;
		case 1:
			goto BUY;
		case 2:
			goto MINING;
		case 3:
			goto BLOCKCHAIN;
		case 4:
			goto MY_WALLET;
		default:
			cout << "Invalid number...\n";
			goto HOME;
		}
	}

BUY:
	{
		system("pause");
		system("cls");
		cout << "List    0.Back\n";

		int i = 1;
		for (Giftcard * giftcard : giftcardList) {
			cout << i << ". " << giftcard->getName() << "\t" << giftcard->getFaceValue() <<"KRW\n";
			i++;
		}

		int select;
		cin >> select;
		if (select == 0) {
			goto HOME;
		}
		
		if (select > 4) {
			cout << "invalid number...\n";
			goto BUY;
		}

		Transaction * tx = w1_.createTransaction(w2_.getPublicKey(), giftcardList[select]->getName(), giftcardList[select]->getFaceValue(), 0, "Buy");
		Transaction * tx2 = w2_.createTransaction(w1_.getPublicKey(), "coin", giftcardList[select]->getMarketValue(), 0, "Buy");

		if (tx == NULL) {
			cout << "Seller have no amount of the giftcard you select...\n";
		}
		if (tx2 == NULL) {
			cout << "You have no balance of the coin for paying...\n";
		}
		
		if (tx->isValid(w2_.getPrivateKey()) && bc_.isUTXO(tx)) {
			if (tx2->isValid(w2_.getPrivateKey()) && bc_.isUTXO(tx2)) {
				bc_.addTransactionToPool(tx);
				bc_.addTransactionToPool(tx2);
				cout << "Valid transaction!\n";
			}
		}
		else {
			delete tx;
			delete tx2;
			cout << "Invalid transaction...\n";
		}
		
		goto HOME;
	}


MINING://complete
	{
		system("pause");
		system("cls");
		bc_.produceBlock(&w2_);
		goto HOME;
	}

BLOCKCHAIN://complete
	{
		system("pause");
		system("cls");
		int select;
		cout << "0.Back    1.Print Blockchain    2.Save Blockchain    3.Load Blockchain    \n";
		cin >> select;

		switch (select) {
		case 0:
			goto HOME;
		case 1:
			goto PRINT;
		case 2:
			goto SAVE;
		case 3:
			goto LOAD;
		default:
			cout << "Invalid number...\n";
			goto BLOCKCHAIN;
		}

		goto HOME;
	}

PRINT://complete
	{
		system("pause");
		system("cls");
		bc_.printBlockchain(cout);
		cout << "\nPrinting was completed!\n";
		goto BLOCKCHAIN;
	}

SAVE://complete
	{
		system("pause");
		system("cls");
		ofstream fout;
		string path;

		int select;
		cout << "0.Back    1.Default path    2.User defined path    \n";
		cin >> select;

		switch (select) {
		case 0:
			goto BLOCKCHAIN;
		case 1:
			fout.open(bc_.getFileName());
			if (!fout.is_open()) {
				cout << "File I/O error...\n";
				break;
			}
			bc_.printBlockchain(fout);
			cout << "File writing was completed!";
			break;
		case 2:
			cout << "File path: ";
			cin >> path;
			fout.open(path);
			if (!fout.is_open()) {
				cout << "File I/O error...\n";
				break;
			}
			bc_.printBlockchain(fout);
			cout << "File writing was completed!";
			break;
		default:
			cout << "Invalid number...\n";
			goto SAVE;
		}
		goto BLOCKCHAIN;
	}

LOAD:
	{
		system("pause");
		system("cls");
		

		goto BLOCKCHAIN;
	}

MY_WALLET://complete
	{
		system("pause");
		system("cls");
		int select;
		cout << "0.Back    1.Send    2.Sell    3.Issue    4.My UTXO\n";
		cin >> select;

		switch (select) {
		case 0:
			goto HOME;
		case 1:
			goto SEND;
		case 2:
			goto SELL;
		case 3:
			goto ISSUE;
		case 4:
			goto MY_UTXO;
		default:
			cout << "Invalid number...\n";
			goto MY_WALLET;
		}
	}

SEND:
	{
		system("pause");
		system("cls");
		string propertyType, memo;
		uint64_t sendingAmount, fee;
		cout << "Which property(string): ";
		cin >> propertyType;

		vector<UTXO> utxo = bc_.getUTXOTable(w1_.getPrivateKey(), propertyType);
		w1_.setMyUTXOTable(utxo);

		cout << "Your property: " << w1_.getMyUTXOAmount(propertyType) << '\n';
		cout << "Receiver's public key: " << w1_.getPublicKey() << '\n';
		cout << "Sending amount(integer): ";
		cin >> sendingAmount;
		cout << "Transaction fee(integer): ";
		cin >> fee;
		cout << "Memo(string): ";
		cin >> memo;

		Transaction * tx_ = w2_.createTransaction(w1_.getPublicKey(), propertyType, sendingAmount, fee, memo);

		if (tx_ == NULL) {
			cout << "You have no property of the propertyType you input...\n";
		}
		else if (tx_->isValid(w2_.getPrivateKey()) && bc_.isUTXO(tx_)) {
			bc_.addTransactionToPool(tx_);
			cout << "Valid transaction!\n";
		}
		else {
			delete tx_;
			cout << "Invalid transaction...\n";
		}
		goto MY_WALLET;
	}

SELL:
	{

	}

ISSUE:
	{

	}

MY_UTXO:
	{

	}

	
	bc_.setVersion("Giftcard Blockchain 1.1");

	bc_.printAllBlockHash();
	bc_.printAllMerkleHash();
	bc_.printAllTransaction(cout);
	//bc.printWaitingBlock();

	bc_.printBlockchain(cout);				// Blockchain을 txt 파일로 저장(출력)
	//bc.loadBlockchain();


	////---------------------- 노드 생성 ----------------------
	///* 블록체인 노드 */
	//Blockchain bc("Giftcard 1.0");		// Blockchain 생성 및 거래 추가

	///* 상품권 구매자 */
	//Wallet w1("giftcard buyer pass phrase");		// 사용자 지갑 생성

	///* 상품권 판매자 */
	//Wallet w2("giftcard seller pass phrase");

	///* 블록체인 채굴자 */
	//Wallet w3("blockchain miner pass phrase");


	////---------------------- 발행된 상품권 조회 ----------------------
	///* 사용자가 블록체인 노드에게 UTXO Table 요청 */
	//vector<UTXO> utxoTable = bc.getUTXOTable();
	//w1.setUTXOTable(utxoTable);
	//w1.printUTXOTable();
	//

	////---------------------- 상품권 발행 및 판매 ----------------------

	///* 판매자의 첫 거래 생성 */
	//Transaction * tx = w2.createCoinbaseTransaction(new Giftcard("Giftcard Name"), 100, 0, "Memo"); // 상품권 종류, 보낼 금액, 수수료, 메모

	///* 판매자가 블록체인 노드에 거래와 개인키 전송 */
	//if (tx->isValidCoinbase()) {
	//	bc.addTransaction(tx);

	//	/* 판매자에게 메시지 전송 */
	//	cout << "상품권 발행 성공!\n";		
	//}
	//else {
	//	/* 판매자에게 메시지 전송 */
	//	cout << "Invalid transaction data...\n";
	//}
	//

	////---------------------- 자신의 상품권 목록 요청 ----------------------
	///* 블록체인 노드에게 자신의 개인키 전송 후 UTXO Table 얻음 */
	//vector<UTXO> myUTXOTable = bc.getUTXOTable(w1.getPrivateKey());

	///* 지갑에 자신의 UTXO 리스트 저장 */
	//w1.setMyUTXOTable(myUTXOTable);
	//w1.printMyUTXOTable();


	////---------------------- 상품권 거래 ----------------------
	///* 상품권 거래 생성 */
	//Transaction * tx2 = w2.createTransaction(w1.getPublicKey(), new Giftcard("Giftcard Name"), 1, 0, "Memo"); // 받는 사람, 상품권 종류, 보낼 금액, 수수료, 메모 

	///* 블록체인 노드에 거래 전송 */
	//if (tx2->isValid(w2.getPrivateKey()), bc.isUTXO(tx2)) {
	//	bc.addTransaction(tx2);

	//	/* w2에게 메시지 전송 */
	//	cout << "판매 성공!\n";

	//	/* w1에게 메시지 전송 */
	//	cout << "구매 성공!\n";
	//}
	//else {
	//	/* 판매자에게 메시지 전송 */
	//	cout << "Invalid transaction data...\n";
	//}





	system("pause");
	return 0;
} 
