/*

	ofxWinMenu basic example - ofApp.cpp

	Example of using ofxWinMenu addon to create a menu for a Microsoft Windows application.
	
	Copyright (C) 2016-2017 Lynn Jarvis.

	https://github.com/leadedge

	http://www.spout.zeal.co

    =========================================================================
    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
    =========================================================================

	03.11.16 - minor comment cleanup
	21.02.17 - rebuild for OF 0.9.8

*/
#include "ofApp.h"
#include <iostream>
#include<cstring>
#include<vector>
using namespace std;
//--------------------------------------------------------------
typedef struct node node;
typedef struct linked_list linked;

// 미로 저장할 linked list의 구조체
struct node {
	int row;
	int col;
	bool up;
	bool left;
	bool right;
	bool down;
};

struct linked_list { // 미로 저장하는 linked list의 노드
	node* n;
	linked* link;
	linked* pre;
};

typedef struct { // BFS 탐색 시 이전 노드의 데이터를 저장할 배열의 구조체
	int row, col;
}temp_node;


linked* head;

temp_node before[31][31]; // BFS 탐색 시 이전 위치 저장할 배열
int* row_stack; // dfs 시 사용할 stack의 배열
int* col_stack;
int top = -1; // top을 배열 내에서 표시할 index 변수

int* row_q; // bfs 시 사용할 queue의 배열
int* col_q;
int front = -1, rear = -1; // queue의 front와 rear를 나타낼 변수

// stack의 push, pop 구현
void push(int row, int col) {
	row_stack[++top] = row;
	col_stack[top] = col;
}
void pop(int* row, int* col) {
	if (top < 0) {
		*row = -1;
		*col = -1;
	}
	else {
		*row = row_stack[top];
		*col = col_stack[top];
		top--;
	}
}

// bfs 시 사용할 queue의 addq와 delq를 구현
void addq(int row, int col) {
	row_q[++rear] = row;
	col_q[rear] = col;
}
void delq(int* row, int* col) {
	if (front == rear) {
		*row = -1;
		*col = -1;
	}
	else {
		*row = row_q[++front];
		*col = col_q[front];
	}
}

void insert(linked* item) { // linked list에 노드를 넣을 때 쓰는 함수
	if (head == NULL) {
		head = item;
	}
	else {
		linked* ptr = head;
		linked* pre;
		while (ptr->link != NULL) {
			pre = ptr;
			ptr = ptr->link;
		}
		item->pre = ptr;
		ptr->link = item;
	}
}
linked* search(int row, int col) { // linked list 내에서 노드 찾아주는 함수
	if (head == NULL) {
		return NULL;
	}
	else {
		linked* ptr = head;
		while (ptr->link != NULL) {
			if (ptr->n->row == row && ptr->n->col == col) {
				return ptr;
			}
			ptr = ptr->link;
		}
	}
	return NULL;
}

typedef struct linked_stack{ // 이전 미로의 데이터를 저장할 수 있도록 하는 stack을 위한 구조체
	int row, col; // 미로의 row와 col을 저장하는 변수
	linked* head; // 기존에 미로의 data를 저장하던 linked list를 저장할 변수
	struct linked_stack* link; // stack을 linked list로 구현하도록 하는 link
}linked_stack;

linked_stack* before_top; // linked_stackd의 top을 저장하는 변수

void before_push(int row, int col) { // maze의 row와 col을 인자로 받음
	linked_stack* temp = (linked_stack*)malloc(sizeof(linked_stack)); // 새로운 node를 동적할당
	temp->head = head; // head를 옮겨서 temp에 저장
	// temp에 인자로 받은 row와 col 저장
	temp->row = row;
	temp->col = col;
	if (before_top == NULL) { // stack이 비어있는 경우
		before_top = temp; // top을 temp로 한다.
		temp->link = NULL;
	}
	else { // 이외에는 기존의 top을 temp의 link에 넣고 top을 temp로 바꾼다.
		temp->link = before_top;
		before_top = temp;
	}
}

linked_stack* before_pop() { // before 버튼을 눌러 이전 미로를 불러올 때 stack에서 빼내는 함수
	if (before_top == NULL) { // stack이 비어있는 경우
		return NULL; // NULL을 return
	}
	else {
		linked_stack* temp = before_top; // temp에 기존 top을 저장
		before_top = temp->link; // top을 temp의 link로 변경
		return temp; // temp를 return 해줌
	}
}

typedef struct user_list { // user 별로 데이터를 저장 위한 구조체
	char name[21]; // user의 이름
	linked_stack* user_top; // user별 가지고 있던 미로 데이터의 stack
	struct user_list* link; // user를 linked list로 연결하기 위한 변수
}user_list;

user_list* user = NULL;
int logined = 0; // 로그인 여부 flag
char cur_user[21]; // 현재 user의 이름 저장

user_list* user_search(char* name) { // user list에서 user를 찾는 함수
	user_list* ptr = user;
	while (ptr) {
		if (strcmp(ptr->name, name) == 0) return ptr; // 이름이 같은 경우로 구분
		ptr = ptr->link;
	}
	return NULL;
}


void ofApp::setup() {

	ofSetWindowTitle("Maze Example"); // Set the app name on the title bar
	ofSetFrameRate(15);
	ofBackground(255, 255, 255);
	// Get the window size for image loading
	windowWidth = ofGetWidth();
	windowHeight = ofGetHeight();
	isdfs = false;
	isOpen = 0;
	isBFS = 0;
	// Centre on the screen
	ofSetWindowPosition((ofGetScreenWidth()-windowWidth)/2, (ofGetScreenHeight()-windowHeight)/2);

	// Load a font rather than the default
	myFont.loadFont("verdana.ttf", 12, true, true);

	// Load an image for the example
	//myImage.loadImage("lighthouse.jpg");

	// Window handle used for topmost function
	hWnd = WindowFromDC(wglGetCurrentDC());

	// Disable escape key exit so we can exit fullscreen with Escape (see keyPressed)
	ofSetEscapeQuitsApp(false);

	//
	// Create a menu using ofxWinMenu
	//

	// A new menu object with a pointer to this class
	menu = new ofxWinMenu(this, hWnd);

	// Register an ofApp function that is called when a menu item is selected.
	// The function can be called anything but must exist. 
	// See the example "appMenuFunction".
	menu->CreateMenuFunction(&ofApp::appMenuFunction);

	// Create a window menu
	HMENU hMenu = menu->CreateWindowMenu();

	//
	// Create a "File" popup menu
	//
	HMENU hPopup = menu->AddPopupMenu(hMenu, "File");

	//
	// Add popup items to the File menu
	//

	// Open an maze file
	menu->AddPopupItem(hPopup, "Open", false, false); // Not checked and not auto-checked
	menu->AddPopupItem(hPopup, "Login", false, false); // 사용자 로그인을 위한 버튼
	menu->AddPopupItem(hPopup, "Logout", false, false); // 사용자 로그아웃을 위한 버튼
	
	// Final File popup menu item is "Exit" - add a separator before it
	menu->AddPopupSeparator(hPopup);
	menu->AddPopupItem(hPopup, "Exit", false, false);

	//
	// View popup menu
	//
	hPopup = menu->AddPopupMenu(hMenu, "View");

	bShowInfo = true;  // screen info display on
	menu->AddPopupItem(hPopup, "Show DFS",false,false); // Checked
	bTopmost = false; // app is topmost
	menu->AddPopupItem(hPopup, "Show BFS", false, false); // Not checked (default)
	bFullscreen = false; // not fullscreen yet
	menu->AddPopupItem(hPopup, "Full screen", false, false); // Not checked and not auto-check
	menu->AddPopupItem(hPopup, "Before", false, false); // 이전 미로를 실행시켜 줄 버튼

	//
	// Help popup menu
	//
	hPopup = menu->AddPopupMenu(hMenu, "Help");
	menu->AddPopupItem(hPopup, "About", false, false); // No auto check

	// Set the menu to the window
	menu->SetWindowMenu();

} // end Setup


//
// Menu function
//
// This function is called by ofxWinMenu when an item is selected.
// The the title and state can be checked for required action.
// 
void ofApp::appMenuFunction(string title, bool bChecked) {

	ofFileDialogResult result;
	string filePath;
	size_t pos;

	//
	// File menu
	//
	if(title == "Open") {
		if(isOpen) before_push(maze_row, maze_col); // open 시에는 현재의 미로를 stack에 넣어준다.(이미 파일을 연 적이 있는 경우만)
		freeMemory(); // head를 NULL로 바꾸고 bfs나 dfs에서 사용한 stack이나 queue는 초기화
		readFile();
	}
	if (title == "Login") { // 로그인 버튼 눌렀을 시
		char name[21]; // 사용자의 이름을 저장할 변수
		// 이름 입력 받기
		cout << "Enter user name(max : 20 character) : ";
		cin >> name;
		user_list* temp_user = user_search(name); // 우선 기존의 list에 존재하는지 찾아보기
		if (!temp_user) { // list에 없는 경우
			// 새로운 노드를 생성한다.
			temp_user = (user_list*)malloc(sizeof(user_list));
			temp_user->user_top = NULL;
			strcpy(temp_user->name, name);
			temp_user->link = user; // 기존 링크의 head를 link로 함
			user = temp_user; // head를 바꿔줌
		}
		if (logined) { // 이전에 로그인이 되어 있던 상태인 경우
			user_list* cur = user_search(cur_user); // 로그인 되어 있던 user의 노드 찾기
			cur->user_top = before_top; // 기존의 stack을 옮겨줌
			freeMemory(); // 이외의 부분은 다 메모리 해제 및 초기화해줌
		}
		else { // 로그인이 되어 있지 않던 경우
			freeMemory_all(); // 기존의 stack까지도 모두 해제하며 초기화
		}
		// row와 col 초기화
		maze_col = 0;
		maze_row = 0;
		before_top = temp_user->user_top; // 현재 stack을 불러온 stack으로 변경
		strcpy(cur_user, temp_user->name); // 현재 user의 이름 update
		logined = 1; // 로그인 flag 변경
		isOpen = 0; // 파일 다시 열어줘야 하므로 0으로 초기화
	}
	if (title == "Logout") { // 로그 아웃 버튼을 눌렀을 때
		if (logined) { // logined가 1일 때, 즉 로그인 한 적 있을 때
			user_list* temp = user_search(cur_user); // cur_user의 노드 찾아서
			if (temp) temp->user_top = before_top; // stack 저장
		}
		freeMemory_list(); // 이외에 메모리 해제 및 변수들 초기화
		before_top = NULL; // top은 NULL로 변경
		isOpen = 0; // 파일 다시 열도록 수정
		// row와 col 초기화
		maze_col = 0;
		maze_row = 0;
		logined = 0; // 로그인 flag 변경
	}
	
	if(title == "Exit") {
		freeMemory_all_data(); // 모든 메모리 해제
		ofExit(); // Quit the application
	}

	//
	// Window menu
	//
	if(title == "Show DFS") {
		//bShowInfo = bChecked;  // Flag is used elsewhere in Draw()
		if (isOpen)
		{
			DFS();
			bShowInfo = bChecked;
		}
		else
			cout << "you must open file first" << endl;
		
	}

	if(title == "Show BFS") {
		if (isOpen)
		{
			BFS();
			bShowInfo = bChecked;
		}
		else
			cout << "you must open file first" << endl;
	}
	if (title == "Before") { // Before 버튼을 눌렀을 때
		if (before_top) { // stack에 요소가 있는 경우
			freeMemory_list(); // 기존에 띄운 미로를 초기화
			linked_stack* temp = before_pop(); // temp에 데이터를 받아온다.
			head = temp->head; // head에 temp에 있는 linked list를 넣어준다
			// row와 col을 temp에 저장되어 있는 것으로 교체
			maze_col = temp->col;
			maze_row = temp->row;
			free(temp); // temp는 메모리 해제해줌
			isOpen = 1; // 파일 존재할 경우 있는 것으로 변경
		}
		else { // 없을 때는 file을 열어야 한다고 표시
			cout << "you must open file first" << endl;
		}
	}

	if(title == "Full screen") {
		bFullscreen = !bFullscreen; // Not auto-checked and also used in the keyPressed function
		doFullScreen(bFullscreen); // But als take action immediately
	}

	//
	// Help menu
	//
	if(title == "About") {
		ofSystemAlertDialog("ofxWinMenu\nbasic example\n\nhttp://spout.zeal.co");
	}

} // end appMenuFunction


//--------------------------------------------------------------
void ofApp::update() {

}


//--------------------------------------------------------------
void ofApp::draw() {

	char str[256];
	//ofBackground(0, 0, 0, 0);
	ofSetColor(100);
	ofSetLineWidth(5);
	int i, j;
	
	// TO DO : DRAW MAZE; 
	// 저장된 자료구조를 이용해 미로를 그린다.
	// add code here
	// 한 칸의 크기는 10 * 10으로
	if(maze_col != 0)
		ofDrawLine(0, 0, 15 * maze_col, 0);
	linked* ptr = head;
	while (ptr) { // 끝까지 탐색하며
		// 각각의 필요한 좌표 계산하여 그리기
		int y = 5 * ptr->n->row + 10 * (ptr->n->row - 1);
		if (ptr->n->col == 1) {
			ofDrawLine(0, y - 2.5, 0, y + 12.5);
		}
		int x = 15 * ptr->n->col;
		if (!ptr->n->right) {
			ofDrawLine(x, y-2.5, x, y + 12.5);
		}
		if (!ptr->n->down) {
			ofDrawLine(x - 12.5, y + 10, x + 2.5, y + 10);
		}
		ptr = ptr->link; // 다음 노드로
	}

	if (isdfs)
	{
		ofSetColor(200);
		ofSetLineWidth(5);
		if (isOpen)
			dfsdraw();
		else
			cout << "You must open file first" << endl;
	}
	if (isBFS)
	{
		ofSetColor(200);
		ofSetLineWidth(5);
		if (isOpen)
			bfsdraw();
		else
			cout << "You must open file first" << endl;
	}
	if (logined) { // 로그인 했을 때는 user의 이름 출력
		ofDrawBitmapString("cur user : " + ofToString(cur_user), ofGetWidth() - 250, ofGetHeight() - 20);
	}
	else { // 아닐 때는 guest로 출력
		ofDrawBitmapString("cur user : guest", ofGetWidth() - 250, ofGetHeight() - 20);
	}
	if(bShowInfo) {
		// Show keyboard duplicates of menu functions
		sprintf(str, " comsil project");
		myFont.drawString(str, 15, ofGetHeight()-20);
	}

} // end Draw


void ofApp::doFullScreen(bool bFull)
{
	// Enter full screen
	if(bFull) {
		// Remove the menu but don't destroy it
		menu->RemoveWindowMenu();
		// hide the cursor
		ofHideCursor();
		// Set full screen
		ofSetFullscreen(true);
	}
	else { 
		// return from full screen
		ofSetFullscreen(false);
		// Restore the menu
		menu->SetWindowMenu();
		// Restore the window size allowing for the menu
		ofSetWindowShape(windowWidth, windowHeight + GetSystemMetrics(SM_CYMENU)); 
		// Centre on the screen
		ofSetWindowPosition((ofGetScreenWidth()-ofGetWidth())/2, (ofGetScreenHeight()-ofGetHeight())/2);
		// Show the cursor again
		ofShowCursor();
		// Restore topmost state
		if(bTopmost) doTopmost(true);
	}

} // end doFullScreen


void ofApp::doTopmost(bool bTop)
{
	if(bTop) {
		// get the current top window for return
		hWndForeground = GetForegroundWindow();
		// Set this window topmost
		SetWindowPos(hWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE); 
		ShowWindow(hWnd, SW_SHOW);
	}
	else {
		SetWindowPos(hWnd, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
		ShowWindow(hWnd, SW_SHOW);
		// Reset the window that was topmost before
		if(GetWindowLong(hWndForeground, GWL_EXSTYLE) & WS_EX_TOPMOST)
			SetWindowPos(hWndForeground, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE); 
		else
			SetWindowPos(hWndForeground, HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE); 
	}
} // end doTopmost


//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
	
	// Escape key exit has been disabled but it can be checked here
	if(key == VK_ESCAPE) {
		// Disable fullscreen set, otherwise quit the application as usual
		if(bFullscreen) {
			bFullscreen = false;
			doFullScreen(false);
		}
		else {
			ofExit();
		}
	}

	// Remove or show screen info
	if(key == ' ') {
		bShowInfo = !bShowInfo;
		// Update the menu check mark because the item state has been changed here
		menu->SetPopupItem("Show DFS", bShowInfo);
	}

	if(key == 'f') {
		bFullscreen = !bFullscreen;	
		doFullScreen(bFullscreen);
		// Do not check this menu item
		// If there is no menu when you call the SetPopupItem function it will crash
	}

} // end keyPressed

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
bool ofApp::readFile()
{
	ofFileDialogResult openFileResult = ofSystemLoadDialog("Select .maz file");
	string filePath;
	size_t pos;
	// Check whether the user opened a file
	if (openFileResult.bSuccess) {
		ofLogVerbose("User selected a file");

		//We have a file, check it and process it
		string fileName = openFileResult.getName();
		//string fileName = "maze0.maz";
		cout << "file name is " << fileName << endl;
		filePath = openFileResult.getPath();
		printf("Open\n");
		pos = filePath.find_last_of(".");
		if (pos != string::npos && pos != 0 && filePath.substr(pos + 1) == "maz") {

			ofFile file(fileName);

			if (!file.exists()) {
				cout << "Target file does not exists." << endl;
				return false;
			}
			else {
				cout << "We found the target file." << endl;
				isOpen = 1;
			}

			ofBuffer buffer(file);

			// Input_flag is a variable for indication the type of input.
			// If input_flag is zero, then work of line input is progress.
			// If input_flag is one, then work of dot input is progress.
			int input_flag = 0;

			// Idx is a variable for index of array.
			int idx = 0;

			// Read file line by line
			int cnt = 0;
			
			
			// TO DO
			// .maz 파일을 input으로 받아서 적절히 자료구조에 넣는다
			int i = 0;
			char input_map[100][100];
			// string type으로 받은 값 char 배열에 저장
			for (auto line : buffer.getLines()) {
				int j = 0;
				for (; j < line.length(); j++) {
					input_map[i][j] = line[j];
				}
				input_map[i][j] = '\0';
				i++;
			}
			// maze의 row와 col 저장
			maze_row = (i-1)/2;
			maze_col = (strlen(input_map[0]) - 1) / 2;
			for (int p = 1; p < i; p+=2) {
				for (int q = 1; q < strlen(input_map[p]); q+=2) {
					// 새로운 node 할당
					linked* newnode = (linked*)malloc(sizeof(linked));
					newnode->n = (node*)malloc(sizeof(node));
					newnode->n->row = (p + 1) / 2;
					newnode->n->col = (q + 1) / 2;
					newnode->link = NULL;
					newnode->pre = NULL;
					// 각각 node의 벽 여부에 따른 값 부여
					if (input_map[p + 1][q] == '-') {
						newnode->n->down = false;
					}
					else {
						newnode->n->down = true;
					}
					if (input_map[p - 1][q] == '-') {
						newnode->n->up = false;
					}
					else {
						newnode->n->up = true;
					}
					if (input_map[p][q + 1] == '|') {
						newnode->n->right = false;
					}
					else {
						newnode->n->right = true;
					}
					if (input_map[p][q - 1] == '|') {
						newnode->n->left = false;
					}
					else {
						newnode->n->left = true;
					}
					insert(newnode); // linked list에 추가
				}
			}
		}
		else {
			printf("  Needs a '.maz' extension\n");
			return false;
		}
	}
}
void ofApp::freeMemory() { // open을 눌러서 기존의 미로는 stack에 넣어주며 다른 배열들만 메모리 초기화하는 함수

	//TO DO
	// malloc한 memory를 free해주는 함수
	head = NULL; // 기존의 미로는 stack에 넣어주었으므로 head만 NULL로 바꿈
	if(!row_stack) free(row_stack);
	if(!col_stack) free(col_stack);
	if(!row_q) free(row_q);
	if(!col_q) free(col_q);
	front = -1;
	rear = -1;
	isBFS = 0;
	isdfs = 0;
	top = -1;
}

void ofApp::freeMemory_list() { // before로 이전 미로 불러올 때 현재 미로는 메모리 해제하는 함수

	//TO DO
	// malloc한 memory를 free해주는 함수
	linked* ptr = head;
	linked* pre;
	while (ptr) { // head부터 탐색하면서 pre의 메모리는 해제
		pre = ptr;
		ptr = ptr->link;
		free(pre->n); // pre 내의 n도 해제한다.
		free(pre);
	}
	head = NULL;
	if (!row_stack) free(row_stack);
	if (!col_stack) free(col_stack);
	if (!row_q) free(row_q);
	if (!col_q) free(col_q);
	
	front = -1;
	rear = -1;
	isBFS = 0;
	isdfs = 0;
	top = -1;
}
void ofApp::freeMemory_all() { // login or logout 시에 모두 해제할 때 사용하는 함수

	//TO DO
	// malloc한 memory를 free해주는 함수
	linked* ptr = head;
	linked* pre;
	while (ptr) { // 현재의 미로의 메모리를 해제하는 부분
		pre = ptr;
		ptr = ptr->link;
		free(pre->n);
		free(pre);
	}
	head = NULL;
	linked_stack* pre_ptr, * search = before_top;
	while (search) { // stack에 남아있는 데이터들도 모두 해제
		// stack을 탐색하는 부분
		pre_ptr = search;
		search = search->link;
		ptr = pre_ptr->head;
		while (ptr) { // stack의 요소 중 하나의 linked list 전체를 해제하는 함수
			pre = ptr;
			ptr = ptr->link;
			// linked list의 요소 모두 해제
			free(pre->n);
			free(pre);
		}
		free(pre_ptr); // stack에서 요소 하나 해제
	}
	if (!row_stack) free(row_stack);
	if (!col_stack) free(col_stack);
	if (!row_q) free(row_q);
	if (!col_q) free(col_q);

	front = -1;
	rear = -1;
	isBFS = 0;
	isdfs = 0;
	top = -1;
}

void ofApp::freeMemory_all_data() { // exit을 눌러서 모두 해제할 때 사용하는 함수

	//TO DO
	// malloc한 memory를 free해주는 함수
	linked* ptr = head;
	linked* pre;
	while (ptr) { // 현재의 미로의 메모리를 해제하는 부분
		pre = ptr;
		ptr = ptr->link;
		free(pre->n);
		free(pre);
	}
	head = NULL;
	linked_stack* pre_ptr, * search = before_top;
	while (search) { // stack에 남아있는 데이터들도 모두 해제
		// stack을 탐색하는 부분
		pre_ptr = search;
		search = search->link;
		ptr = pre_ptr->head;
		while (ptr) { // stack의 요소 중 하나의 linked list 전체를 해제하는 함수
			pre = ptr;
			ptr = ptr->link;
			// linked list의 요소 모두 해제
			free(pre->n);
			free(pre);
		}
		free(pre_ptr); // stack에서 요소 하나 해제
	}
	user_list* se = user; // user list 탐색 위한 변수
	user_list* pre_se;
	while (se) { // user list의 메모리 해제를 위한 반복문
		pre_se = se;
		se = se->link;
		search = pre_se->user_top;
		while (search) { // stack에 남아있는 데이터들도 모두 해제
			// stack을 탐색하는 부분
			pre_ptr = search;
			search = search->link;
			ptr = pre_ptr->head;
			while (ptr) { // stack의 요소 중 하나의 linked list 전체를 해제하는 함수
				pre = ptr;
				ptr = ptr->link;
				// linked list의 요소 모두 해제
				free(pre->n);
				free(pre);
			}
			free(pre_ptr); // stack에서 요소 하나 해제
		}
		free(pre_se); // 메모리 해제
	}
	if (!row_stack) free(row_stack);
	if (!col_stack) free(col_stack);
	if (!row_q) free(row_q);
	if (!col_q) free(col_q);

	front = -1;
	rear = -1;
	isBFS = 0;
	isdfs = 0;
	top = -1;
}

bool ofApp::DFS()//DFS탐색을 하는 함수
{
	//TO DO
	//DFS탐색을 하는 함수 ( 3주차)
	for (int i = 0; i < 31; i++) { // 방문 표시 array 초기화
		for (int j = 0; j < 31; j++) {
			visited[i][j] = 0;
		}
	}
	// row, col 저장할 stack array 할당
	row_stack = (int*)malloc(sizeof(int) * 31 * 31);
	col_stack = (int*)malloc(sizeof(int) * 31 * 31);
	// 시작 지점 push 및 방문 표시
	push(1, 1);
	visited[1][1] = 1;
	while (top >= 0) {
		int row = row_stack[top];
		int col = col_stack[top];
		visited[row][col] = 1;
		if (row == maze_row && col == maze_col) { // 도착 지점 도달 시 isdfs 변경해주고 true 반환
			isdfs = 1;
			return true;
		}
		linked* temp = search(row, col);
		int flag = 0;
		// 각 방향에 대해서 방문하지 않으면 push하고 flag로 변경 표시
		if (temp->n->right) {
			if (!visited[row][col + 1]) {
				push(row, col + 1);
				flag = 1;
				visited[row][col + 1] = 1;
				col++;
				continue;
			}
		}
		if (temp->n->down) {
			if (!visited[row + 1][col]) {
				push(row + 1, col);
				flag = 1;
				visited[row + 1][col] = 1;
				row++;
				continue;
			}
		}
		if (temp->n->left) {
			if (!visited[row][col - 1]) {
				push(row, col - 1);
				flag = 1;
				visited[row][col - 1] = 1;
				col--;
				continue;
			}
		}
		if (temp->n->up) {
			if (!visited[row - 1][col]) {
				push(row - 1, col);
				flag = 1;
				visited[row - 1][col] = 1;
				row--;
				continue;
			}
		}
		// 다른 노드로 이동 안 했을 시 pop하기
		if (flag == 0) {
			pop(&row, &col);
		}
	}
	return false;
}
void ofApp::dfsdraw()
{
	//TO DO 
	//DFS를 수행한 결과를 그린다. (3주차 내용)
	ofSetColor(200); // 최단 경로 아닌 곳은 연하게 출력
	for (int i = 1; i < maze_row; i++) {
		for (int j = 1; j < maze_col; j++) {
			linked* temp = search(i, j);
			if (visited[i][j]) {
				if (visited[i + 1][j] && temp->n->down) {
					ofDrawLine(j * 5 + 10 * (j - 1) + 2.5, i * 5 + 10 * (i - 1) + 2.5, j * 5 + 10 * (j - 1) + 2.5, (i + 1) * 5 + 10 * i + 2.5);
				}
				if (visited[i][j + 1] && temp->n->right) {
					ofDrawLine(j * 5 + 10 * (j - 1) + 2.5, i * 5 + 10 * (i - 1) + 2.5, (j + 1) * 5 + 10 * j + 2.5, i * 5 + 10 * (i - 1) + 2.5);
				}
			}
		}
	}
	ofSetColor(50); // 최단 경로는 진하게 출력
	for (int i = 0; i < top; i++) {
		int x1 = col_stack[i];
		int y1 = row_stack[i];
		int x2 = col_stack[i + 1];
		int y2 = row_stack[i + 1];
		ofDrawLine(x1 * 5 + 10 * (x1 - 1) + 2.5, y1 * 5 + 10 * (y1 - 1) + 2.5, x2 * 5 + 10 * (x2 - 1) + 2.5, y2 * 5 + 10 * (y2-1) + 2.5);
	}
}

bool ofApp::BFS() {
	for (int i = 0; i < 31; i++) { // 방문 표시 array 초기화
		for (int j = 0; j < 31; j++) {
			visited[i][j] = 0;
		}
	}
	row_q = (int*)malloc(sizeof(int) * 31 * 31);
	col_q = (int*)malloc(sizeof(int) * 31 * 31);
	// 시작지점 큐에 넣고 방문 표시 및 이전 지점 0으로 정하기
	addq(1, 1);
	visited[1][1] = 1;
	before[1][1].row = 0;
	before[1][1].col = 0;
	int row, col;
	while (front != rear) {
		delq(&row, &col); // 큐에서 제거
		// 도착 지점 도달 시 정지
		if (row == maze_row && col == maze_col) {
			isBFS = 1;
			return true;
		}
		// 각 방향별로 갈 수 있는지 판단 후 큐에 넣고 이전 지점을 현재 가진 temp의 위치 넣어준다.방문 표시
		linked* temp = search(row, col);
		if (temp->n->right) {
			if (!visited[row][col + 1]) {
				addq(row, col + 1);
				visited[row][col + 1] = 1;
				before[row][col + 1].row = temp->n->row;
				before[row][col + 1].col = temp->n->col;
			}
		}
		if (temp->n->down) {
			if (!visited[row + 1][col]) {
				addq(row + 1, col);
				visited[row + 1][col] = 1;
				before[row + 1][col].row = temp->n->row;
				before[row + 1][col].col = temp->n->col;
			}
		}
		if (temp->n->left) {
			if (!visited[row][col - 1]) {
				addq(row, col - 1);
				visited[row][col - 1] = 1;
				before[row][col - 1].row = temp->n->row;
				before[row][col - 1].col = temp->n->col;
			}
		}
		if (temp->n->up) {
			if (!visited[row - 1][col]) {
				addq(row - 1, col);
				visited[row - 1][col] = 1;
				before[row - 1][col].row = temp->n->row;
				before[row - 1][col].col = temp->n->col;
			}
		}
	}
}
void ofApp::bfsdraw() {
	//TO DO 
	//BFS를 수행한 결과를 그린다. (3주차 내용)
	ofSetColor(200); // 최단 경로 아닌 곳은 연하게 출력
	for (int i = 1; i < maze_row; i++) {
		for (int j = 1; j < maze_col; j++) {
			linked* temp = search(i, j);
			if (visited[i][j]) {
				if (visited[i + 1][j] && temp->n->down) {
					ofDrawLine(j * 5 + 10 * (j - 1) + 2.5, i * 5 + 10 * (i - 1) + 2.5, j * 5 + 10 * (j - 1) + 2.5, (i + 1) * 5 + 10 * i + 2.5);
				}
				if (visited[i][j + 1] && temp->n->right) {
					ofDrawLine(j * 5 + 10 * (j - 1) + 2.5, i * 5 + 10 * (i - 1) + 2.5, (j + 1) * 5 + 10 * j + 2.5, i * 5 + 10 * (i - 1) + 2.5);
				}
			}
		}
	}
	ofSetColor(50); // 최단 경로는 진하게 출력
	int x1 = maze_col;
	int y1 = maze_row;
	for (;;) {
		int x2 = before[y1][x1].col;
		int y2 = before[y1][x1].row; // 이전 지점과 현재 지점 연결하여 경로 완성
		if (x2 == 0 && y2 == 0) break;
		ofDrawLine(x1 * 5 + 10 * (x1 - 1) + 2.5, y1 * 5 + 10 * (y1 - 1) + 2.5, x2 * 5 + 10 * (x2 - 1) + 2.5, y2 * 5 + 10 * (y2 - 1) + 2.5);
		x1 = x2;
		y1 = y2;
	}
}

