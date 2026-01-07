// SPDX-License-Identifier: GPL-3.0
pragma solidity >=0.7.0 <0.9.0;

contract BlindAuction {
    struct Bid {
        bytes32 blindedBid;
        uint deposit;
    }

    // Init - 0; Bidding - 1; Reveal - 2; Done - 3
    enum Phase {Init, Bidding, Reveal, Done}

    // Owner
    address payable public beneficiary;

    // Keep track of the highest bid,bidder
    address public highestBidder;
    uint public highestBid = 0;

    // Only one bid allowed per address
    mapping(address => Bid) public bids;
    mapping(address => uint) pendingReturns;

    // [추가 기능] 반환할 금액을 전체 저장하는 변수
    uint public totalPendingAmount = 0;

    Phase public currentPhase = Phase.Init;

    // Events
    event AuctionEnded(address winner, uint highestBid);
    event BiddingStarted();
    event RevealStarted();
    event AuctionInit();

    // Modifiers
    modifier OnlyBene(){ // Beneficiary인 경우만 접근
        require(msg.sender == beneficiary, "Only Beneficiary can access");
        _;
    }
    modifier Check_Phase(Phase _phase){ // Phase가 맞는지 확인
        require(currentPhase == _phase, "Invalid phase!");
        _;
    }

    constructor() {
        // beneficiary는 contract 배포자
        beneficiary = payable(msg.sender);
        // event 발생
        emit AuctionInit();
    }

    function advancePhase() public OnlyBene { // phase 진행은 beneficiary만 가능
        // 이미 Done인 경우는 진행하지 않음
        require(currentPhase != Phase.Done, "Already done");
        if(currentPhase == Phase.Init){// Init의 경우 Bidding으로
            currentPhase = Phase.Bidding;
            emit BiddingStarted();
        }
        else if(currentPhase == Phase.Bidding){ // Bidding의 경우에는 Reveal로
            currentPhase = Phase.Reveal;
            emit RevealStarted();
        }
        else if(currentPhase == Phase.Reveal){ // Reveal의 경우에는 Done으로
            currentPhase = Phase.Done;
        }
    }

    function bid(bytes32 blindBid) public payable Check_Phase(Phase.Bidding) { // Bidding 단계에서만 실행가능
        require(bids[msg.sender].blindedBid == 0, "You already do it"); // 이미 등록한 id는 불가
        require(msg.value > 0, "Need to send it"); // value가 0 이상
        bids[msg.sender] = Bid(blindBid, msg.value); // bid 저장
    }

    function reveal(uint value, bytes32 secret) public Check_Phase(Phase.Reveal) { // 입찰가와 비밀번호 확인
        Bid storage Toreveal = bids[msg.sender]; // 저장소에 있는 입찰 정보를 가져옴
        require(Toreveal.blindedBid != 0, "You don't have bid"); // 입찰 내역의 존재여부 확인
        // 해시값 검증 단계
        bytes32 cal_hash = keccak256(abi.encodePacked(value, secret));
        require(cal_hash == Toreveal.blindedBid, "Don't match");
        uint valueInWei = value * 1 ether; // 이더 단위 수정
        require(Toreveal.deposit >= valueInWei, "Deposit is less than the revealed bid value.");
        // 검증 완료 후 입찰 정보를 초기화
        Toreveal.blindedBid = 0;
        uint re_trans = Toreveal.deposit - valueInWei;
        // 입찰가보다 많은 경우는 환불 리스트에 추가
        if(re_trans > 0){
            pendingReturns[msg.sender] += re_trans;
            totalPendingAmount += re_trans; // [추가기능] 환불금 총액에 추가
        }
        // 최고 입찰가보다 높은 경우에는 최고 입찰자 갱신
        if(valueInWei > highestBid){
            if(highestBidder != address(0)){
                pendingReturns[highestBidder] += highestBid;
                totalPendingAmount += highestBid; // [추가기능] 최고 입찰자 금액도 추가
            }
            highestBid = valueInWei;
            highestBidder = msg.sender;
        }
        else{
            // 최고 입찰자가 아니면 환불 리스트에 추가
            pendingReturns[msg.sender] += valueInWei;
            totalPendingAmount += valueInWei; // [추가기능] 탈락자의 입찰금도 환불금에 추가
        }
    }

    // Withdraw a non-winning bid
    function withdraw() public { // 환불하는 함수
        uint amount = pendingReturns[msg.sender];
        require(amount > 0, "No funds to withdraw"); // 환불할 금액이 없는 경우
        pendingReturns[msg.sender] = 0; // 먼저 초기화한 이후에 전송
        totalPendingAmount -= amount; // [추가기능] 환불이 된 금액은 차감
        payable(msg.sender).transfer(amount);
    }

    // Send the highest bid to the beneficiary and end the auction
    function auctionEnd() public OnlyBene Check_Phase(Phase.Done){ // 경매를 종료하는 함수
        require(highestBidder != address(0), "Auction ended with no valid bid"); 
        // 최종 낙찰 이벤트 발생
        emit AuctionEnded(highestBidder, highestBid);
        // 판매자에 낙찰 금액 전송
        payable(beneficiary).transfer(highestBid);
        // 남는 금액 계산(reveal에서 계산되지 않고 contract에 남은 것)
        uint currentBalance = address(this).balance;
        uint lostDeposits = currentBalance - totalPendingAmount;
        payable(beneficiary).transfer(lostDeposits); // 컨트랙트에 남은 비용은 배포자에 전달
    }
}
