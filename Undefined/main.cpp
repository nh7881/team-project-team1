#include <iostream>
#include "Blockchain.h"
#include "Transaction.h"
#include "Wallet.h"
using namespace std;

int main()
{
	Wallet w("qwerty");		// ����� ���� ����

	vector<Input> inputs;	// Transaction Input(�۽���, ����, �ݾ�)
	vector<Output> outputs;	// Transaction Output(������, ����, �ݾ�)

	Input i, i2, i3;
	Output o, o2, o3;
	
	inputs.push_back(i);
	inputs.push_back(i2);
	inputs.push_back(i3);

	outputs.push_back(o);
	outputs.push_back(o2);
	outputs.push_back(o3);

	Transaction * tx = w.createTransaction(inputs, outputs, "Blockchain Version", "Giftcard Name", "Memo");		// Transaction �߻�
	Transaction * tx2 = w.createTransaction(inputs, outputs, "Blockchain Version", "Giftcard Name", "Memo");	// Transaction2 �߻�
	Transaction * tx3 = w.createTransaction(inputs, outputs, "Blockchain Version", "Giftcard Name", "Memo");	// Transaction3 �߻�
	Transaction * tx4 = w.createTransaction(inputs, outputs, "Blockchain Version", "Giftcard Name", "Memo");	// Transaction4 �߻�

	Blockchain bc("Giftcard 1.0", tx);	// Blockchain ���� �� �ŷ� �߰�
	bc.addTransaction(tx2);				// Blockchain�� �ŷ�2 �߰�
	
	bc.setVersion("Giftcard 1.1");		// Blockchain Version ����
	bc.addTransaction(tx3);				// Blockchain�� �ŷ�3 �߰�
	bc.addTransaction(tx4);				// Blockchain�� �ŷ�4 �߰�

	bc.printAllBlockHash();
	bc.printAllMerkleHash();
	bc.printAllTransaction(cout);
	//bc.printWaitingBlock();

	bc.saveBlockchain();				// Blockchain�� txt ���Ϸ� ����(���)
	//bc.loadBlockchain();


	system("pause");
	return 0;
} 