#include <iostream>
#include "Blockchain.h"
#include "Transaction.h"
using namespace std;

int main()
{
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

	Transaction * tx = new Transaction(inputs, outputs, "Blockchain Version", "Giftcard Name", "Memo");		// Transaction �߻�
	Transaction * tx2 = new Transaction(inputs, outputs, "Blockchain Version", "Giftcard Name", "Memo");	// Transaction2 �߻�
	Transaction * tx3 = new Transaction(inputs, outputs, "Blockchain Version", "Giftcard Name", "Memo");	// Transaction3 �߻�
	Transaction * tx4 = new Transaction(inputs, outputs, "Blockchain Version", "Giftcard Name", "Memo");	// Transaction4 �߻�

	Blockchain bc("Giftcard 1.0", tx);	// Blockchain ���� �� �ŷ� �߰�
	bc.addTransaction(tx2);				// Blockchain�� �ŷ� �߰�
	bc.addTransaction(tx3);				// Blockchain�� �ŷ� �߰�

	bc.setVersion("Giftcard 1.1");		// Blockchain Version ����
	bc.addTransaction(tx4);				// Blockchain�� �ŷ� �߰�

	bc.printAllBlockHash();
	bc.printAllMerkleHash();
	bc.printAllTransaction(cout);
	//bc.printWaitingBlock();

	bc.saveBlockchain();				// Blockchain�� txt ���Ϸ� ����(���)
	//bc.loadBlockchain();


	system("pause");
	return 0;
} 