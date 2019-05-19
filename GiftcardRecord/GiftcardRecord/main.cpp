#include <iostream>
#include "Blockchain.h"
#include "Transaction.h"
#include "Wallet.h"
using namespace std;

int main()
{
	// ���ü�� ���
	Blockchain bc("Giftcard 1.0");		// Blockchain ���� �� �ŷ� �߰�

	// ��ǰ�� ������
	Wallet w1("giftcard buyer pass phrase");		// ����� ���� ����

	// ��ǰ�� �Ǹ���
	Wallet w2("giftcard seller pass phrase");

	// ���ü�� ä����
	Wallet w3("blockchain miner pass phrase");


	//---------------------- ����� ��ǰ�� ��ȸ ----------------------
	/* ����ڰ� ���ü�� ��忡�� UTXO Table ��û */
	vector<UTXO> utxoTable = bc.getUTXOTable();
	w1.setUTXOTable(utxoTable);
	w1.printUTXOTable();
	

	//---------------------- ��ǰ�� ���� �� �Ǹ� ----------------------
	/* �Ǹ����� ù �ŷ� ���� */
	Output o(w2.getPublicKey(), 10);

	vector<Output> outputs;	// Transaction Output(������, ����, �ݾ�)
	outputs.push_back(o);

	Transaction * tx = new Transaction(outputs, "Giftcard Name", "Memo");

	/* �Ǹ����� ������ �ŷ� ���� */
	Input i2(w2.getPrivateKey(), 10, tx->getTransactionHash());
	Output o2(w3.getPublicKey(), 1);
	Output o2_2(w2.getPublicKey(), 9);
	vector<Input> inputs2;
	vector<Output> outputs2;	// Transaction Output(������, ����, �ݾ�)

	inputs2.push_back(i2);
	outputs2.push_back(o2);
	outputs2.push_back(o2_2);
	
	Transaction * tx2 = new Transaction(inputs2, outputs2, "Giftcard Name", "Memo");

	/* �Ǹ��ڰ� ���ü�� ��忡 �ŷ��� ����Ű ���� */
	if (tx->isValidCoinbase() && tx2->isValid(w2.getPrivateKey())) {	// ������ ����Ű ��ȣȭ�Ǿ� ����ǰ���?
		bc.addTransaction(tx);
		bc.addTransaction(tx2);

		/* �Ǹ��ڿ��� �޽��� ���� */
		cout << "�Ǹ� ����!\n";
		
		/* �����ڿ��� �޽��� ���� */
		cout << "���� ����!\n";
	}
	else {
		/* �Ǹ��ڿ��� �޽��� ���� */
		cout << "�߸��� �ŷ� ����\n";
	}
	

	//---------------------- �ڽ��� ��ǰ�� ��� ��û ----------------------
	/* ���ü�� ��忡�� �ڽ��� ����Ű ���� �� UTXO Table ���� */
	vector<UTXO> myUTXOTable = bc.getUTXOTable(w1.getPublicKey());

	/* ������ �ڽ��� UTXO ����Ʈ ���� */
	w1.setMyUTXOTable(myUTXOTable);
	w1.printMyUTXOTable();


	//---------------------- ��ǰ�� �ŷ� ----------------------
	vector<Input> inputs3;	// Transaction Input(�۽���, ����, �ݾ�)
	vector<Output> outputs3;	// Transaction Output(������, ����, �ݾ�)

	Input i3(w1.getPrivateKey(), 10, w1.getMyUTXOTable());
	Output o3(w2.getPublicKey(), 5);
	Output o3_2(w2.getPublicKey(), 3);
	Output o3_3(w1.getPublicKey(), 2);	// �Ž��� ��
	
	inputs3.push_back(i3);

	outputs3.push_back(o3);
	outputs3.push_back(o3_2);
	outputs3.push_back(o3_3);

	Transaction * tx3 = new Transaction(inputs3, outputs3, "Giftcard Name", "Memo");	// Transaction �߻�
	
	/* ���ü�� ��忡 �ŷ� ���� */
	if (tx3->isValid(w1.getPrivateKey())) {
		bc.addTransaction(tx3);

		/* �Ǹ��ڿ��� �޽��� ���� */
		cout << "�Ǹ� ����!\n";

		/* �����ڿ��� �޽��� ���� */
		cout << "���� ����!\n";
	}
	else {
		/* �Ǹ��ڿ��� �޽��� ���� */
		cout << "�߸��� �ŷ� ����\n";
	}




	//-----------------------------------------------------
	
	bc.setVersion("Giftcard 1.1");		// Blockchain Version ����

	bc.printAllBlockHash();
	bc.printAllMerkleHash();
	bc.printAllTransaction(cout);
	//bc.printWaitingBlock();

	bc.saveBlockchain();				// Blockchain�� txt ���Ϸ� ����(���)
	//bc.loadBlockchain();


	system("pause");
	return 0;
} 