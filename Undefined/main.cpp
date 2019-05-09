#include <iostream>
#include "Blockchain.h"
#include "Transaction.h"
using namespace std;

int main()
{
	Transaction * tx = new Transaction("k", "book", "borrow");
	Transaction * tx2 = new Transaction("k2", "book2", "borrow2");
	Transaction * tx3 = new Transaction("k3", "book3", "borrow3");
	Transaction * tx4 = new Transaction("k4", "book4", "borrow4");

	Blockchain bc(tx);
	bc.addTransaction(tx2);
	bc.addTransaction(tx3);
	bc.addTransaction(tx4);

	bc.printAllBlockHash();
	bc.printAllMerkleHash();
	bc.printAllTransaction(cout);
	//bc.printWaitingBlock();


	bc.saveBlockchain();
	//bc.loadBlockchain();
	






	system("pause");
	return 0;
} 