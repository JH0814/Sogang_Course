#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "20211522.h"

int main() {
	//파일 열고 구조체 head 선언
	FILE* st_fp = fopen("student.txt", "r");
	FILE* m_fp = fopen("major.txt", "r");
	FILE* p_fp = fopen("postcode.txt", "r");
	FILE* a_fp = fopen("address.txt", "r");
	if (st_fp == NULL) {
		printf("file open error\n");
		return 1;
	}
	if (m_fp == NULL) {
		printf("file open error\n");
		return 1;
	}
	if (p_fp == NULL) {
		printf("file open error\n");
		return 1;
	}
	if (a_fp == NULL) {
		printf("file open error\n");
		return 1;
	}
	STUDENT* s_head = NULL;
	MAJOR* m_head = NULL;
	POSTCODE* p_head = NULL;
	ADDRESS* a_head = NULL;
	//각각의 구조체에 파일에서 입력받은 값 연결리스트로 만들기
	while (feof(m_fp)==0) {
		MAJOR* new = (MAJOR*)malloc(sizeof(MAJOR));//새로운 노드 선언
		fgets(new->major, 30, m_fp);//한 줄씩 입력받기
		for (int i = 0; *(new->major + i) != 0; i++){//마지막이 개행문자 시 제거
			if (*(new->major + i) == '\n') {
				*(new->major + i) = 0;
				break;
			}
		}
		if (m_head == NULL) {//리스트 비어있을 때 추가
			new->link = m_head;
			m_head = new;
		}
		else {//이외에는 리스트의 마지막에 추가
			MAJOR* ptr1 = m_head;
			while (ptr1->link != NULL) {
				ptr1 = ptr1->link;
			}
			new->link = ptr1->link;
			ptr1->link = new;
		}
	}
	while (feof(a_fp)==0) {
		ADDRESS* new = (ADDRESS*)malloc(sizeof(ADDRESS));//노드 선언
		fgets(new->address, 20, a_fp);//한줄씩 입력받기
		for (int i = 0; *(new->address + i) != 0; i++) {//마지막 개행문자 제거
			if (*(new->address + i) == '\n') {
				*(new->address + i) = 0;
				break;
			}
		}
		if (a_head == NULL) {//빈 리스트 시 추가
			new->link = a_head;
			a_head = new;
		}
		else {//이외에 리스트 마지막에 추가
			ADDRESS* ptr2 = a_head;
			while (ptr2->link != NULL) {
				ptr2 = ptr2->link;
			}
			new->link = ptr2->link;
			ptr2->link = new;
		}
	}
	while (feof(p_fp) == 0) {
		POSTCODE* new = (POSTCODE*)malloc(sizeof(POSTCODE));//노드 선언
		fscanf(p_fp, "%d", &(new->postcode));//정수로 한 줄씩 입력받기
		if (p_head == NULL) {//빈 리스트 시 추가
			new->link = p_head;
			p_head = new;
		}
		else {//이외에 리스트 마지막에 추가
			POSTCODE* ptr3 = p_head;
			while (ptr3->link != NULL) {
				ptr3 = ptr3->link;
			}
			new->link = ptr3->link;
			ptr3->link = new;
		}
	}
	while (feof(st_fp)==0) {
		STUDENT* new = (STUDENT*)malloc(sizeof(STUDENT));//노드 선언
		if (new == NULL) {
			printf("memory error!\n");
			return 1;
		}
		char* maj = (char*)calloc(30, sizeof(char));//값 비교시 사용할 maj와 add 동적할당
		char* add = (char*)calloc(20, sizeof(char));
		if (maj == NULL) {
			printf("memory error!\n");
			return 1;
		}
		if (add == NULL) {
			printf("memory error!\n");
			return 1;
		}
		int post;//값 비교시 사용할 post 선언
		fscanf(st_fp, "%d %s %d %s %s %d", &(new->std_id), new->name, &(new->year), maj, add, &post);
		for (int i = 0; *(add + i) != 0; i++) {//마지막 개행문자 제거
			if (*(add + i) == '\n') {
				*(add + i) = 0;
				break;
			}
		}
		for (int i = 0; *(maj + i) != 0; i++) {//마지막 개행문자 제거
			if (*(maj + i) == '\n') {
				*(maj + i) = 0;
				break;
			}
		}
		MAJOR* ptr1 = m_head;//구조체별로 포인터 설정
		ADDRESS* ptr2 = a_head;
		POSTCODE* ptr3 = p_head;
		while (ptr1 != NULL) {//m_link에 노드 할당
			if (strncmp(ptr1->major, maj, strlen(maj)) == 0) {
				new->m_link = ptr1;
				break;
			}
			ptr1 = ptr1->link;
		}
		while (ptr2 != NULL) {//a_link에 노드 할당
			if (strncmp(ptr2->address, add, strlen(add)) == 0) {
				new->a_link = ptr2;
				break;
			}
			ptr2 = ptr2->link;
		}
		while (ptr3 != NULL) {//p_link에 노드 할당
			if (post == ptr3->postcode) {
				new->p_link = ptr3;
				break;
			}
			ptr3 = ptr3->link;
		}
		if (s_head == NULL) {//빈 리스트 시에 추가
			new->link = s_head;
			s_head = new;
		}
		else if (s_head->year > new->year) {//리스트 첫 노드의 생년월일보다 작으면 헤드로 바꿈
			new->link = s_head;
			s_head = new;
		}
		else {
			STUDENT* ptr = s_head;
			while (1) {//이외에는 생년월일의 크기 비교하여 추가
				if (ptr->link == NULL) {
					ptr->link = new;
					new->link = NULL;
					break;
				}
				else if (ptr->link->year > new->year) {
					new->link = ptr->link;
					ptr->link = new;
					break;
				}
				else {
					ptr = ptr->link;
				}
			}
		}
		free(maj);
		free(add);
	}
	//선택 입력받아 그에 따른 함수 실행
	while (1) {
		int sel, id;
		STUDENT* new = (STUDENT*)malloc(sizeof(STUDENT));
		char* name = (char*)malloc(sizeof(char) * 20);
		char* major = (char*)malloc(sizeof(char) * 30);
		printf("select menu\n");
		printf("[1] : insert  [2] : delete  [3] : search neighbor [4] : search family\n");
		printf("[5] : search friend  [6] : search major  [7] : change major  [8] : exit\n");
		printf(">");
		scanf("%d", &sel);
		switch (sel) {
		case 1:
			if (new == NULL) {
				printf("memory error!\n");
				return 1;
			}
			insert(s_head, m_head, a_head, p_head, new);
			break;
		case 2:
			free(new);
			printf("Enter the student id : ");
			scanf("%d", &id);
			if (s_head !=NULL&&s_head->std_id == id) {
				STUDENT* ptr = s_head;
				s_head = s_head->link;
				free(ptr);
			}
			else {
				delete(s_head, id);
			}
			break;
		case 3:
			free(new);
			printf("Enter the student id : ");
			scanf("%d", &id);
			printf("Enter the student name : ");
			scanf("%s", name);
			for (int i = 0; *(name + i) != 0; i++) {//개행문자 제거
				if (*(name + i) == '\n') {
					*(name + i) = 0;
					break;
				}
			}
			search_neighbor(s_head, name, id);
			break;
		case 4:
			free(new);
			printf("Enter the student id : ");
			scanf("%d", &id);
			printf("Enter the student name : ");
			scanf("%s", name);
			for (int i = 0; *(name + i) != 0; i++) {//개행문자 제거
				if (*(name + i) == '\n') {
					*(name + i) = 0;
					break;
				}
			}
			search_family(s_head, name, id);
			break;
		case 5:
			free(new);
			printf("Enter the student id : ");
			scanf("%d", &id);
			printf("Enter the student name : ");
			scanf("%s", name);
			for (int i = 0; *(name + i) != 0; i++) {//개행문자 제거
				if (*(name + i) == '\n') {
					*(name + i) = 0;
					break;
				}
			}
			search_friend(s_head, name, id);
			break;
		case 6:
			free(new);
			printf("Enter the student id : ");
			scanf("%d", &id);
			printf("Enter the student name : ");
			scanf("%s", name);
			for (int i = 0; *(name + i) != 0; i++) {//개행문자 제거
				if (*(name + i) == '\n') {
					*(name + i) = 0;
					break;
				}
			}
			search_major(s_head, name, id);
			break;
		case 7:
			free(new);
			printf("Enter the student id : ");
			scanf("%d", &id);
			printf("Enter the student name : ");
			scanf("%s", name);
			printf("Enter the major : ");
			scanf("%s", major);
			for (int i = 0; *(name + i) != 0; i++) {//개행문자 제거
				if (*(name + i) == '\n') {
					*(name + i) = 0;
					break;
				}
			}
			for (int i = 0; *(major + i) != 0; i++) {//개행문자 제거
				if (*(major + i) == '\n') {
					*(major + i) = 0;
					break;
				}
			}
			change_major(s_head, m_head, name, id, major);
			break;
		case 8:
			free(new);
			printf("program exit\n");
			break;
		default:
			printf("Wrong menu!\n");
			break;
		}
		free(name);
		free(major);
		if (sel == 8) {
			break;
		}
	}
	//free 처리
	STUDENT* c;
	while (s_head->link != NULL) {
		c = s_head;
		s_head = s_head->link;
		free(c);
		c = NULL;
	}
	ADDRESS* c1;
	while (a_head->link != NULL) {
		c1 = a_head;
		a_head = a_head->link;
		free(c1);
		c1 = NULL;
	}
	POSTCODE* c2;
	while (p_head->link != NULL) {
		c2 = p_head;
		p_head = p_head->link;
		free(c2);
		c2 = NULL;
	}
	MAJOR* c3;
	while (m_head->link != NULL) {
		c3 = m_head;
		m_head = m_head->link;
		free(c3);
		c3 = NULL;
	}
	//파일 종료
	fclose(st_fp);
	fclose(m_fp);
	fclose(p_fp);
	fclose(a_fp);
}
void insert(STUDENT* s_head, MAJOR* m_head, ADDRESS* a_head, POSTCODE* p_head, STUDENT* new) {//리스트에 학생정보 삽입, 새로운 노드는 메인에서 할당하여 사용하기 위해 new추가
	MAJOR* ptr2 = m_head;//구조체의 각각의 포인터 선언
	ADDRESS* ptr3 = a_head;
	POSTCODE* ptr4 = p_head;
	int flag = 0;//올바른 정보인지 판단하는 정수
	int postcode;//비교시 임시로 저장할 변수
	char * major, * address;
	major = (char*)malloc(sizeof(char) * 30);
	address = (char*)malloc(sizeof(char) * 20);
	//학생정보 입력받기
	printf("Enter the id : ");
	scanf("%d", &(new->std_id));
	printf("Enter the name : ");
	scanf("%s", new->name);
	printf("Enter the year : ");
	scanf("%d", &(new->year));
	printf("Enter the major : ");
	scanf("%s", major);
	printf("Enter the postcode : ");
	scanf("%d", &postcode);
	printf("Enter the address : ");
	scanf("%s", address);
	for (int i = 0; *(address + i) != 0; i++) {//개행문자 제거
		if (*(address + i) == '\n') {
			*(address + i) = 0;
			break;
		}
	}
	for (int i = 0; *(major + i) != 0; i++) {//개행문자 제거
		if (*(major + i) == '\n') {
			*(major + i) = 0;
			break;
		}
	}
	for (int i = 0; *(new->name + i) != 0; i++) {//개행문자 제거
		if (*(new->name + i) == '\n') {
			*(new->name + i) = 0;
			break;
		}
	}
	//m_link, a_link, p_link에 노드를 할당하고 flag가 그대로 0이면 리스트에 연결하지 않고 끝냄
	while(1){
		if (strncmp(ptr2->major, major, strlen(major)) == 0) {
			new->m_link = ptr2;
			flag = 1;
			break;
		}
		if (ptr2->link == NULL) {
			break;
		}
		ptr2 = ptr2->link;
	}
	if (flag == 0) {
		printf("Wrong data!\n");
		free(new);
		return;
	}
	while(1) {
		if (strncmp(ptr3->address, address, strlen(address)) == 0) {
			new->a_link = ptr3;
			flag = 1;
			break;
		}
		if (ptr3->link == NULL) {
			break;
		}
		ptr3 = ptr3->link;
	}
	if (flag == 0) {
		printf("Wrong data!\n");
		free(new);
		return;
	}
	while(1) {
		if (ptr4->postcode == postcode) {
			new->p_link = ptr4;
			flag = 1;
			break;
		}
		if (ptr4->link == NULL) {
			break;
		}
		ptr4 = ptr4->link;
	}
	if (flag == 0) {
		printf("Wrong data!\n");
		free(new);
		return;
	}
	//달이 12가 넘거나 일이 31보다 크면 추가하지 않음
	int m, d;
	m = ((new->year) % 10000) / 100;
	d = (new->year) % 100;
	if (m > 12 || d > 31) {
		printf("Wrong data!\n");
		free(new);
		return;
	}
	if (s_head == NULL) {//빈 리스트 시 추가
		new->link = s_head;
		s_head = new;
	}
	else {//생년월일을 비교하여 나이 많은 순으로 정렬되도록 끼워넣음
		STUDENT* ptr = s_head;
		while (1) {
			if (ptr->link == NULL) {
				ptr->link = new;
				new->link = NULL;
				break;
			}
			else if (ptr->link->year > new->year) {
				new->link = ptr->link;
				ptr->link = new;
				break;
			}
			else {
				ptr = ptr->link;
			}
		}
	}
	free(major);
	free(address);
}
void delete(STUDENT* s_head, int n) {//학번을 받아 학생 삭제, 학생정보 있는 부분과 학번만 필요하여 나머지 매개인자는 제거
	STUDENT* ptr = s_head;
	STUDENT* pre, * cur;
	int flag = 0;//없는 학생인지 확인
	if (s_head == NULL) {//빈 리스트 시 멈춤
		printf("there is no data in list\n");
		return;
	}
	else {//학생의 번호가 맞으면 뒤의 노드와 연결시키고 해당 노드 free
		while (ptr->link != NULL) {
			if (ptr->link->std_id == n) {
				pre = ptr;
				cur = ptr->link;
				pre->link = cur->link;
				flag = 1;
				free(cur);
				return;
			}
			ptr = ptr->link;
		}
	}
	if (flag == 0) {//flag가 그대로면 없는 데이터로 확인하고 멈춤
		printf("Wrong data!\n");
		return;
	}
}
void search_neighbor(STUDENT* s_head, char name[], int n) {//우편번호는 같지만 주소 다른 경우 출력
	STUDENT* ptr = s_head;
	int post;
	int flag = 0;
	char* add = (char*)malloc(sizeof(char) * 20);
	while (ptr != NULL) {
		if (strcmp(name, ptr->name) == 0 && n == ptr->std_id) {//입력받은 n과 name을 ptr로 리스트에서 확인하며 해당학생 찾기
			post = ptr->p_link->postcode;//해당 학생의 정보로 변수에 저장
			strcpy(add, ptr->a_link->address);
			flag = 1;
			break;
		}
		ptr = ptr->link;
	}
	if (flag == 0) {//없으면 멈춤
		printf("Wrong data!\n");
		free(add);
		return;
	}
	STUDENT* ptr2 = s_head;
	while (ptr2 != NULL) {
		if (post == ptr2->p_link->postcode && strcmp(ptr2->a_link->address, add) != 0) {//리스트에서 이웃에 해당하는 관계 찾기, 자기 자신은 이미 주소가 같아 이미 제외됨
			printf("id : %d name : %s year : 0%d major : %s\n", ptr2->std_id, ptr2->name, ptr2->year, ptr2->m_link->major);
			printf("postcode : %d address : %s\n", ptr2->p_link->postcode, ptr2->a_link->address);
		}
		ptr2 = ptr2->link;
	}
	free(add);
}
void search_family(STUDENT* s_head, char name[], int n) {//우편번호와 주소 모두 같은 학생 출력
	STUDENT* ptr = s_head;
	int flag = 0;
	int post;
	char* add = (char*)malloc(sizeof(char) * 20);
	while (ptr != NULL) {
		if (strcmp(name, ptr->name) == 0 && n == ptr->std_id) {//해당 학생 찾아 정보 변수에 저장
			post = ptr->p_link->postcode;
			strcpy(add, ptr->a_link->address);
			flag = 1;
			break;
		}
		ptr = ptr->link;
	}
	if (flag == 0) {//없으면 멈춤
		printf("Wrong data!\n");
		free(add);
		return;
	}
	STUDENT* ptr2 = s_head;
	while (ptr2 != NULL) {
		if (post == ptr2->p_link->postcode && strcmp(ptr2->a_link->address, add) == 0) {//가족에 해당하는 학생들 찾기
			printf("id : %d name : %s year : 0%d major : %s\n", ptr2->std_id, ptr2->name, ptr2->year, ptr2->m_link->major);
			printf("postcode : %d address : %s\n", ptr2->p_link->postcode, ptr2->a_link->address);
		}
		ptr2 = ptr2->link;
	}
	free(add);
}
void search_friend(STUDENT* s_head, char name[], int n) {//출생년도 같은 학생들 출력
	STUDENT* ptr = s_head;
	int flag = 0;
	int age;
	while (ptr != NULL) {
		if (strcmp(name, ptr->name) == 0 && n == ptr->std_id) {//학번 이름으로 찾기
			age = (ptr->year) / 10000;//태어난 년도를 나이로 하여 구하기
			flag = 1;
			break;
		}
		ptr = ptr->link;
	}
	if (flag == 0) {//없으면 멈춤
		printf("Wrong data!\n");
		return;
	}
	STUDENT* ptr2 = s_head;
	while (ptr2 != NULL) {
		if ((ptr2->year)/10000 == age) {//같은 년도 출생인지 찾아 출력
			printf("id : %d name : %s year : 0%d major : %s\n", ptr2->std_id, ptr2->name, ptr2->year, ptr2->m_link->major);
			printf("postcode : %d address : %s\n", ptr2->p_link->postcode, ptr2->a_link->address);
		}
		ptr2 = ptr2->link;
	}
}
void search_major(STUDENT* s_head, char name[], int n) {//같은 전공 학생 찾아 출력
	STUDENT* ptr = s_head;
	char* maj = (char*)malloc(sizeof(char) * 30);
	int flag = 0;
	while (ptr != NULL) {
		if (strcmp(name, ptr->name) == 0 && n == ptr->std_id) {//해당학생 찾아 major 저장
			strcpy(maj, ptr->m_link->major);
			flag = 1;
			break;
		}
		ptr = ptr->link;
	}
	if (flag == 0) {//없으면 멈춤
		printf("Wrong data!\n");
		return;
	}
	STUDENT* ptr2 = s_head;
	while (ptr2 != NULL) {
		if (strcmp(ptr2->m_link->major, maj)==0) {//major같은 학생들 찾아 출력
			printf("id : %d name : %s year : 0%d major : %s\n", ptr2->std_id, ptr2->name, ptr2->year, ptr2->m_link->major);
			printf("postcode : %d address : %s\n", ptr2->p_link->postcode, ptr2->a_link->address);
		}
		ptr2 = ptr2->link;
	}
	free(maj);
}
void change_major(STUDENT* s_head, MAJOR* m_head, char name[], int n, char m[]) {//전공 입력받아 변경, 전공 리스트 사용해야 하므로 m_head 추가
	STUDENT* ptr = s_head;
	MAJOR* ptr2 = m_head;
	int flag = 0;
	while (ptr != NULL) {
		if (strcmp(name, ptr->name) == 0 && n == ptr->std_id) {//해당학생 노드 찾기
			break;
		}
		ptr = ptr->link;
	}

	while (ptr2 != NULL) {
		if (strncmp(m, ptr2->major,strlen(m)) == 0) {//입력받은 전공 전공 리스트에서 찾아 비교
			ptr->m_link = ptr2;//같으면 바꾸기
			flag = 1;
			break;
		}
		ptr2 = ptr2->link;
	}
	if (flag == 0) {//없으면 오류 출력
		printf("it is not included in the list\n");
		return;
	}
}
