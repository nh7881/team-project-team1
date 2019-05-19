# 상품권 블록체인

## Wallet Node Interface 
```c++
  /* Wallet Node 내부 동작 */
  
  // pass phrase를 입력해 Wallet 생성
	Wallet(std::string passPhrase);

  // 거래 구성 요소인 inputs(생략 가능)과 outputs, 전송 금액, 받는 사람 PublicKey를 작성한다.
	void setInputsOuputs(std::vector<Output> & outputs, std::uint64_t sendingAmount, const BYTE * receiverPublicKey);
	void setInputsOuputs(std::vector<Input> & inputs, std::vector<Output> & outputs, std::uint64_t sendingAmount, const BYTE * receiverPublicKey);


  /* Wallet Node와 Blockchain Node 간의 통신 */

  // Blockchain Node에게 UTXO Table을 요청하고, Blockchain Node로부터 UTXO Table을 받는다.
  // 그 후 자신의 UTXO Table을 갱신한다.
	void setUTXOTable(std::vector<UTXO> & utxoTable);

  // Blockchain Node에게 자신의 private key를 주며 UTXO Table을 요청하고, Blockchain Node로부터 UTXO Table을 받는다.
  // 그 후 자신의 UTXO Table을 갱신한다.
	void setMyUTXOTable(std::vector<UTXO> & myUTXOTable);
  
  // setInputsOutputs()으로 거래 내용을 작성하고 거래를 생성한다.
  // 생성된 거래와 private key를 함께 Blockchain Node에게 전송하며 거래 채굴을 요청한다.
  Transaction * tx = new Transaction(inputs, outputs, "Giftcard Name", "Memo");
```

## Blockchain Node Interface
```c++
  /* Blockchain Node 내부 동작 */
  
  // Blockchain 생성
  Blockchain(std::string version);
  Blockchain(std::string version, Transaction * tx);

  void setVersion(std::string version);
  void saveBlockchain() const;
  
  
  /* Wallet Node와 Blockchain Node 간의 통신 */
  
  // Wallet Node가 UTXO Table을 요청할 시 Wallet Node에게 반환된 값을 전송한다.
  std::vector<UTXO> getUTXOTable() const;
  
  // Wallet Node가 private key를 주며 UTXO Table을 요청할 시 Wallet Node에게 반환된 값을 전송한다.
  std::vector<UTXO> getUTXOTable(const BYTE * privateKey) const;
  
  // Wallet Node가 private key와 transaction을 주며 거래 채굴을 요청할 시 
  // transaction이 유효한지 검증하고 블록체인에 추가한다.
  if(tx->isValid(wallet.getPrivateKey()))
      void addTransaction(Transaction * tx);
```


  

  
