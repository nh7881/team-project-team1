# 상품권 블록체인

## Wallet Node Interface 
```c++
/* ----------- Wallet Node 내부 Interface ----------- */
  
  
// pass phrase를 입력해 Wallet 생성
Wallet w1("상품권 판매자");
Wallet w2("상품권 구매자");

// 상품권 발행 (상품권 종류, 발행 금액, 발행 수수료, 메모)
Transaction * tx = w1.createCoinbaseTransaction("Giftcard Name", 100, 0, "Memo"); 

// 상품권 거래 (받는 사람, 상품권 종류, 보낼 금액, 수수료, 메모)
Transaction * tx = w1.createTransaction(w2.getPublicKey(), "Giftcard Name", 1, 0, "Memo");


/* ----------- Wallet Node와 Blockchain Node 간의 통신 ----------- */


// Blockchain Node에게 UTXO Table을 요청한다.
// Blockchain Node로부터 UTXO Table을 받아 지갑의 UTXO Table을 갱신한다.
w1.setUTXOTable(vector<UTXO> & utxoTable);

// Blockchain Node에게 자신의 private key를 주며 My UTXO Table을 요청한다.
BYTE * privateKey = w1.getPrivateKey();
// Blockchain Node에게 My UTXO Table을 받아 지갑의 My UTXO Table을 갱신한다.
w1.setMyUTXOTable(vector<UTXO> & myUTXOTable);

// Blockchain Node에게 거래(tx)와 privateKey를 전송하며 거래 채굴을 요청한다.
Transaction * tx = w1.createTransaction(w2.getPublicKey(), "Giftcard Name", 1, 0, "Memo");
BYTE * privateKey = w1.getPrivateKey();
```


## Blockchain Node Interface
```c++
/* ----------- Blockchain Node 내부 Interface ----------- */
  
  
// Blockchain 생성
Blockchain bc(string version);

// Blockchain version 변경
bc.setVersion(string version);

// Blockchain 내용을 ostream에 출력
bc.printBlockchain(ostream o);
  
  
/* ----------- Wallet Node와 Blockchain Node 간의 통신 ----------- */
  
  
// Wallet Node가 UTXO Table을 요청할 시 Wallet Node에게 함수 반환값을 전송한다.
vector<UTXO> getUTXOTable();

// Wallet Node가 privateKey를 주며 My UTXO Table을 요청할 시 Wallet Node에게 함수 반환값을 전송한다.
vector<UTXO> getUTXOTable(const BYTE * privateKey);
  
// Wallet Node가 transaction과 자신의 privateKey를 주며 거래 채굴을 요청할 시
// transaction이 유효한지 확인하고 블록체인에 추가한다.
if(tx->isValid(privateKey))
    bc.addTransaction(Transaction * tx);
```


  

  
