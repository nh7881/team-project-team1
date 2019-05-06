#include <iostream>
#include "Blockchain.h"
#include "Transaction.h"
using namespace std;

int main()
{
	Transaction * tx = new Transaction("kim", time(NULL), "book", "borrow");
	Blockchain bc(tx);

	bc.addTransaction(tx);
	bc.addTransaction(tx);

	bc.printAllBlockHash();
	bc.printAllMerkleHash();
	//bc.printAllTransaction();
	//bc.printWaitingBlock();


	bc.saveBlockchain();
	//bc.loadBlockchain();




	system("pause");
	return 0;
} 