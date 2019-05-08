#include <iostream>
#include "Blockchain.h"
#include "Transaction.h"
using namespace std;

int main()
{
	Transaction * tx = new Transaction("kio", "book", "borrow");
	Transaction * tx2 = new Transaction("kim", "book1", "borrow");
	Blockchain bc(tx);

	bc.addTransaction(tx);
	bc.addTransaction(tx2);

	bc.printAllBlockHash();
	bc.printAllMerkleHash();
	bc.printAllTransaction(cout);
	//bc.printWaitingBlock();


	bc.saveBlockchain();
	//bc.loadBlockchain();
	



	system("pause");
	return 0;
} 