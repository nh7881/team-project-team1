#include <iostream>
#include "Blockchain.h"
#include "Transaction.h"
#include "Wallet.h"
using namespace std;

int main()
{
	//---------------------- Test ---------------------
	Wallet w1_("w1");
	Wallet w2_("w2");
	Blockchain bc_("Giftcard 1.0");

	Transaction * tx_ = w2_.createCoinbaseTransaction("Giftcard Name", 100, 0, "Memo");
	Transaction * tx2_ = w2_.createTransaction(w1_.getPublicKey(), "Giftcard Name", 10, 0, "Memo");
	Transaction * tx3_ = w2_.createTransaction(w1_.getPublicKey(), "Giftcard Name", 10, 0, "Memo");
	Transaction * tx4_ = w2_.createTransaction(w1_.getPublicKey(), "Giftcard Name", 10, 0, "Memo");

	bc_.addTransaction(tx_);
	bc_.addTransaction(tx2_);

	bc_.setVersion("Giftcard 1.1");
	bc_.addTransaction(tx3_);
	bc_.addTransaction(tx4_);


	bc_.printAllBlockHash();
	bc_.printAllMerkleHash();
	bc_.printAllTransaction(cout);
	//bc.printWaitingBlock();

	bc_.printBlockchain(cout);				// Blockchain�� txt ���Ϸ� ����(���)
	//bc.loadBlockchain();


	//---------------------- ��� ���� ----------------------
	/* ���ü�� ��� */
	Blockchain bc("Giftcard 1.0");		// Blockchain ���� �� �ŷ� �߰�

	/* ��ǰ�� ������ */
	Wallet w1("giftcard buyer pass phrase");		// ����� ���� ����

	/* ��ǰ�� �Ǹ��� */
	Wallet w2("giftcard seller pass phrase");

	/* ���ü�� ä���� */
	Wallet w3("blockchain miner pass phrase");


	//---------------------- ����� ��ǰ�� ��ȸ ----------------------
	/* ����ڰ� ���ü�� ��忡�� UTXO Table ��û */
	vector<UTXO> utxoTable = bc.getUTXOTable();
	w1.setUTXOTable(utxoTable);
	w1.printUTXOTable();
	

	//---------------------- ��ǰ�� ���� �� �Ǹ� ----------------------

	/* �Ǹ����� ù �ŷ� ���� */
	Transaction * tx = w2.createCoinbaseTransaction("Giftcard Name", 100, 0, "Memo"); // ��ǰ�� ����, ���� �ݾ�, ������, �޸�

	/* �Ǹ��ڰ� ���ü�� ��忡 �ŷ��� ����Ű ���� */
	if (tx->isValidCoinbase()) {
		bc.addTransaction(tx);

		/* �Ǹ��ڿ��� �޽��� ���� */
		cout << "��ǰ�� ���� ����!\n";		
	}
	else {
		/* �Ǹ��ڿ��� �޽��� ���� */
		cout << "�߸��� �ŷ� ����\n";
	}
	

	//---------------------- �ڽ��� ��ǰ�� ��� ��û ----------------------
	/* ���ü�� ��忡�� �ڽ��� ����Ű ���� �� UTXO Table ���� */
	vector<UTXO> myUTXOTable = bc.getUTXOTable(w1.getPrivateKey());

	/* ������ �ڽ��� UTXO ����Ʈ ���� */
	w1.setMyUTXOTable(myUTXOTable);
	w1.printMyUTXOTable();


	//---------------------- ��ǰ�� �ŷ� ----------------------
	/* ��ǰ�� �ŷ� ���� */
	Transaction * tx2 = w2.createTransaction(w1.getPublicKey(), "Giftcard Name", 1, 0, "Memo"); // �޴� ���, ��ǰ�� ����, ���� �ݾ�, ������, �޸� 

	/* ���ü�� ��忡 �ŷ� ���� */
	if (tx2->isValid(w2.getPrivateKey())) {
		bc.addTransaction(tx2);

		/* w2���� �޽��� ���� */
		cout << "�Ǹ� ����!\n";

		/* w1���� �޽��� ���� */
		cout << "���� ����!\n";
	}
	else {
		/* �Ǹ��ڿ��� �޽��� ���� */
		cout << "�߸��� �ŷ� ����\n";
	}






	system("pause");
	return 0;
} 