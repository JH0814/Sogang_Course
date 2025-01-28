#include "LinkedList.h"


//1. 템플릿 클래스로 확장해야함
//2. Stack형식으로 Delete 함수 재정의해야함
//주의: first, current_size는 class의 멤버 변수이기 때문에 this 포인터를 사용하여 가져와야함

//LinkedList class를 상속받음
template <typename T>
class Stack : public LinkedList<T>{
	public:
		bool Delete (T &element){
			//first가 0이면 false반환
			if(this -> first == 0){
				return false;
			}

			// LinkedList와 달리 Stack은 current가 가리키는 곳을 삭제
			Node<T>* cur = this -> first; // 첫번째 노드 변수에 저장
			this -> first = this -> first -> link; // 기존 스택의 첫 번째 노드는 기존의 두 번째 노드로 변경
			element = cur -> data; // 멤버 함수의 인자에 삭제할 노드의 data 값 저장

			delete cur; // 노드의 메모리 해제
			this -> current_size--; // 스택의 총 개수에서 1 빼주기
			


			return true; // 삭제 과정 진행이 끝나면 true 반환
			}
};
