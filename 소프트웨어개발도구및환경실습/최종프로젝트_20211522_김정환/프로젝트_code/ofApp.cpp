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

// �̷� ������ linked list�� ����ü
struct node {
	int row;
	int col;
	bool up;
	bool left;
	bool right;
	bool down;
};

struct linked_list { // �̷� �����ϴ� linked list�� ���
	node* n;
	linked* link;
	linked* pre;
};

typedef struct { // BFS Ž�� �� ���� ����� �����͸� ������ �迭�� ����ü
	int row, col;
}temp_node;


linked* head;

temp_node before[31][31]; // BFS Ž�� �� ���� ��ġ ������ �迭
int* row_stack; // dfs �� ����� stack�� �迭
int* col_stack;
int top = -1; // top�� �迭 ������ ǥ���� index ����

int* row_q; // bfs �� ����� queue�� �迭
int* col_q;
int front = -1, rear = -1; // queue�� front�� rear�� ��Ÿ�� ����

// stack�� push, pop ����
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

// bfs �� ����� queue�� addq�� delq�� ����
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

void insert(linked* item) { // linked list�� ��带 ���� �� ���� �Լ�
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
linked* search(int row, int col) { // linked list ������ ��� ã���ִ� �Լ�
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

typedef struct linked_stack{ // ���� �̷��� �����͸� ������ �� �ֵ��� �ϴ� stack�� ���� ����ü
	int row, col; // �̷��� row�� col�� �����ϴ� ����
	linked* head; // ������ �̷��� data�� �����ϴ� linked list�� ������ ����
	struct linked_stack* link; // stack�� linked list�� �����ϵ��� �ϴ� link
}linked_stack;

linked_stack* before_top; // linked_stackd�� top�� �����ϴ� ����

void before_push(int row, int col) { // maze�� row�� col�� ���ڷ� ����
	linked_stack* temp = (linked_stack*)malloc(sizeof(linked_stack)); // ���ο� node�� �����Ҵ�
	temp->head = head; // head�� �Űܼ� temp�� ����
	// temp�� ���ڷ� ���� row�� col ����
	temp->row = row;
	temp->col = col;
	if (before_top == NULL) { // stack�� ����ִ� ���
		before_top = temp; // top�� temp�� �Ѵ�.
		temp->link = NULL;
	}
	else { // �̿ܿ��� ������ top�� temp�� link�� �ְ� top�� temp�� �ٲ۴�.
		temp->link = before_top;
		before_top = temp;
	}
}

linked_stack* before_pop() { // before ��ư�� ���� ���� �̷θ� �ҷ��� �� stack���� ������ �Լ�
	if (before_top == NULL) { // stack�� ����ִ� ���
		return NULL; // NULL�� return
	}
	else {
		linked_stack* temp = before_top; // temp�� ���� top�� ����
		before_top = temp->link; // top�� temp�� link�� ����
		return temp; // temp�� return ����
	}
}

typedef struct user_list { // user ���� �����͸� ���� ���� ����ü
	char name[21]; // user�� �̸�
	linked_stack* user_top; // user�� ������ �ִ� �̷� �������� stack
	struct user_list* link; // user�� linked list�� �����ϱ� ���� ����
}user_list;

user_list* user = NULL;
int logined = 0; // �α��� ���� flag
char cur_user[21]; // ���� user�� �̸� ����

user_list* user_search(char* name) { // user list���� user�� ã�� �Լ�
	user_list* ptr = user;
	while (ptr) {
		if (strcmp(ptr->name, name) == 0) return ptr; // �̸��� ���� ���� ����
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
	menu->AddPopupItem(hPopup, "Login", false, false); // ����� �α����� ���� ��ư
	menu->AddPopupItem(hPopup, "Logout", false, false); // ����� �α׾ƿ��� ���� ��ư
	
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
	menu->AddPopupItem(hPopup, "Before", false, false); // ���� �̷θ� ������� �� ��ư

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
		if(isOpen) before_push(maze_row, maze_col); // open �ÿ��� ������ �̷θ� stack�� �־��ش�.(�̹� ������ �� ���� �ִ� ��츸)
		freeMemory(); // head�� NULL�� �ٲٰ� bfs�� dfs���� ����� stack�̳� queue�� �ʱ�ȭ
		readFile();
	}
	if (title == "Login") { // �α��� ��ư ������ ��
		char name[21]; // ������� �̸��� ������ ����
		// �̸� �Է� �ޱ�
		cout << "Enter user name(max : 20 character) : ";
		cin >> name;
		user_list* temp_user = user_search(name); // �켱 ������ list�� �����ϴ��� ã�ƺ���
		if (!temp_user) { // list�� ���� ���
			// ���ο� ��带 �����Ѵ�.
			temp_user = (user_list*)malloc(sizeof(user_list));
			temp_user->user_top = NULL;
			strcpy(temp_user->name, name);
			temp_user->link = user; // ���� ��ũ�� head�� link�� ��
			user = temp_user; // head�� �ٲ���
		}
		if (logined) { // ������ �α����� �Ǿ� �ִ� ������ ���
			user_list* cur = user_search(cur_user); // �α��� �Ǿ� �ִ� user�� ��� ã��
			cur->user_top = before_top; // ������ stack�� �Ű���
			freeMemory(); // �̿��� �κ��� �� �޸� ���� �� �ʱ�ȭ����
		}
		else { // �α����� �Ǿ� ���� �ʴ� ���
			freeMemory_all(); // ������ stack������ ��� �����ϸ� �ʱ�ȭ
		}
		// row�� col �ʱ�ȭ
		maze_col = 0;
		maze_row = 0;
		before_top = temp_user->user_top; // ���� stack�� �ҷ��� stack���� ����
		strcpy(cur_user, temp_user->name); // ���� user�� �̸� update
		logined = 1; // �α��� flag ����
		isOpen = 0; // ���� �ٽ� ������� �ϹǷ� 0���� �ʱ�ȭ
	}
	if (title == "Logout") { // �α� �ƿ� ��ư�� ������ ��
		if (logined) { // logined�� 1�� ��, �� �α��� �� �� ���� ��
			user_list* temp = user_search(cur_user); // cur_user�� ��� ã�Ƽ�
			if (temp) temp->user_top = before_top; // stack ����
		}
		freeMemory_list(); // �̿ܿ� �޸� ���� �� ������ �ʱ�ȭ
		before_top = NULL; // top�� NULL�� ����
		isOpen = 0; // ���� �ٽ� ������ ����
		// row�� col �ʱ�ȭ
		maze_col = 0;
		maze_row = 0;
		logined = 0; // �α��� flag ����
	}
	
	if(title == "Exit") {
		freeMemory_all_data(); // ��� �޸� ����
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
	if (title == "Before") { // Before ��ư�� ������ ��
		if (before_top) { // stack�� ��Ұ� �ִ� ���
			freeMemory_list(); // ������ ��� �̷θ� �ʱ�ȭ
			linked_stack* temp = before_pop(); // temp�� �����͸� �޾ƿ´�.
			head = temp->head; // head�� temp�� �ִ� linked list�� �־��ش�
			// row�� col�� temp�� ����Ǿ� �ִ� ������ ��ü
			maze_col = temp->col;
			maze_row = temp->row;
			free(temp); // temp�� �޸� ��������
			isOpen = 1; // ���� ������ ��� �ִ� ������ ����
		}
		else { // ���� ���� file�� ����� �Ѵٰ� ǥ��
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
	// ����� �ڷᱸ���� �̿��� �̷θ� �׸���.
	// add code here
	// �� ĭ�� ũ��� 10 * 10����
	if(maze_col != 0)
		ofDrawLine(0, 0, 15 * maze_col, 0);
	linked* ptr = head;
	while (ptr) { // ������ Ž���ϸ�
		// ������ �ʿ��� ��ǥ ����Ͽ� �׸���
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
		ptr = ptr->link; // ���� ����
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
	if (logined) { // �α��� ���� ���� user�� �̸� ���
		ofDrawBitmapString("cur user : " + ofToString(cur_user), ofGetWidth() - 250, ofGetHeight() - 20);
	}
	else { // �ƴ� ���� guest�� ���
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
			// .maz ������ input���� �޾Ƽ� ������ �ڷᱸ���� �ִ´�
			int i = 0;
			char input_map[100][100];
			// string type���� ���� �� char �迭�� ����
			for (auto line : buffer.getLines()) {
				int j = 0;
				for (; j < line.length(); j++) {
					input_map[i][j] = line[j];
				}
				input_map[i][j] = '\0';
				i++;
			}
			// maze�� row�� col ����
			maze_row = (i-1)/2;
			maze_col = (strlen(input_map[0]) - 1) / 2;
			for (int p = 1; p < i; p+=2) {
				for (int q = 1; q < strlen(input_map[p]); q+=2) {
					// ���ο� node �Ҵ�
					linked* newnode = (linked*)malloc(sizeof(linked));
					newnode->n = (node*)malloc(sizeof(node));
					newnode->n->row = (p + 1) / 2;
					newnode->n->col = (q + 1) / 2;
					newnode->link = NULL;
					newnode->pre = NULL;
					// ���� node�� �� ���ο� ���� �� �ο�
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
					insert(newnode); // linked list�� �߰�
				}
			}
		}
		else {
			printf("  Needs a '.maz' extension\n");
			return false;
		}
	}
}
void ofApp::freeMemory() { // open�� ������ ������ �̷δ� stack�� �־��ָ� �ٸ� �迭�鸸 �޸� �ʱ�ȭ�ϴ� �Լ�

	//TO DO
	// malloc�� memory�� free���ִ� �Լ�
	head = NULL; // ������ �̷δ� stack�� �־��־����Ƿ� head�� NULL�� �ٲ�
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

void ofApp::freeMemory_list() { // before�� ���� �̷� �ҷ��� �� ���� �̷δ� �޸� �����ϴ� �Լ�

	//TO DO
	// malloc�� memory�� free���ִ� �Լ�
	linked* ptr = head;
	linked* pre;
	while (ptr) { // head���� Ž���ϸ鼭 pre�� �޸𸮴� ����
		pre = ptr;
		ptr = ptr->link;
		free(pre->n); // pre ���� n�� �����Ѵ�.
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
void ofApp::freeMemory_all() { // login or logout �ÿ� ��� ������ �� ����ϴ� �Լ�

	//TO DO
	// malloc�� memory�� free���ִ� �Լ�
	linked* ptr = head;
	linked* pre;
	while (ptr) { // ������ �̷��� �޸𸮸� �����ϴ� �κ�
		pre = ptr;
		ptr = ptr->link;
		free(pre->n);
		free(pre);
	}
	head = NULL;
	linked_stack* pre_ptr, * search = before_top;
	while (search) { // stack�� �����ִ� �����͵鵵 ��� ����
		// stack�� Ž���ϴ� �κ�
		pre_ptr = search;
		search = search->link;
		ptr = pre_ptr->head;
		while (ptr) { // stack�� ��� �� �ϳ��� linked list ��ü�� �����ϴ� �Լ�
			pre = ptr;
			ptr = ptr->link;
			// linked list�� ��� ��� ����
			free(pre->n);
			free(pre);
		}
		free(pre_ptr); // stack���� ��� �ϳ� ����
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

void ofApp::freeMemory_all_data() { // exit�� ������ ��� ������ �� ����ϴ� �Լ�

	//TO DO
	// malloc�� memory�� free���ִ� �Լ�
	linked* ptr = head;
	linked* pre;
	while (ptr) { // ������ �̷��� �޸𸮸� �����ϴ� �κ�
		pre = ptr;
		ptr = ptr->link;
		free(pre->n);
		free(pre);
	}
	head = NULL;
	linked_stack* pre_ptr, * search = before_top;
	while (search) { // stack�� �����ִ� �����͵鵵 ��� ����
		// stack�� Ž���ϴ� �κ�
		pre_ptr = search;
		search = search->link;
		ptr = pre_ptr->head;
		while (ptr) { // stack�� ��� �� �ϳ��� linked list ��ü�� �����ϴ� �Լ�
			pre = ptr;
			ptr = ptr->link;
			// linked list�� ��� ��� ����
			free(pre->n);
			free(pre);
		}
		free(pre_ptr); // stack���� ��� �ϳ� ����
	}
	user_list* se = user; // user list Ž�� ���� ����
	user_list* pre_se;
	while (se) { // user list�� �޸� ������ ���� �ݺ���
		pre_se = se;
		se = se->link;
		search = pre_se->user_top;
		while (search) { // stack�� �����ִ� �����͵鵵 ��� ����
			// stack�� Ž���ϴ� �κ�
			pre_ptr = search;
			search = search->link;
			ptr = pre_ptr->head;
			while (ptr) { // stack�� ��� �� �ϳ��� linked list ��ü�� �����ϴ� �Լ�
				pre = ptr;
				ptr = ptr->link;
				// linked list�� ��� ��� ����
				free(pre->n);
				free(pre);
			}
			free(pre_ptr); // stack���� ��� �ϳ� ����
		}
		free(pre_se); // �޸� ����
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

bool ofApp::DFS()//DFSŽ���� �ϴ� �Լ�
{
	//TO DO
	//DFSŽ���� �ϴ� �Լ� ( 3����)
	for (int i = 0; i < 31; i++) { // �湮 ǥ�� array �ʱ�ȭ
		for (int j = 0; j < 31; j++) {
			visited[i][j] = 0;
		}
	}
	// row, col ������ stack array �Ҵ�
	row_stack = (int*)malloc(sizeof(int) * 31 * 31);
	col_stack = (int*)malloc(sizeof(int) * 31 * 31);
	// ���� ���� push �� �湮 ǥ��
	push(1, 1);
	visited[1][1] = 1;
	while (top >= 0) {
		int row = row_stack[top];
		int col = col_stack[top];
		visited[row][col] = 1;
		if (row == maze_row && col == maze_col) { // ���� ���� ���� �� isdfs �������ְ� true ��ȯ
			isdfs = 1;
			return true;
		}
		linked* temp = search(row, col);
		int flag = 0;
		// �� ���⿡ ���ؼ� �湮���� ������ push�ϰ� flag�� ���� ǥ��
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
		// �ٸ� ���� �̵� �� ���� �� pop�ϱ�
		if (flag == 0) {
			pop(&row, &col);
		}
	}
	return false;
}
void ofApp::dfsdraw()
{
	//TO DO 
	//DFS�� ������ ����� �׸���. (3���� ����)
	ofSetColor(200); // �ִ� ��� �ƴ� ���� ���ϰ� ���
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
	ofSetColor(50); // �ִ� ��δ� ���ϰ� ���
	for (int i = 0; i < top; i++) {
		int x1 = col_stack[i];
		int y1 = row_stack[i];
		int x2 = col_stack[i + 1];
		int y2 = row_stack[i + 1];
		ofDrawLine(x1 * 5 + 10 * (x1 - 1) + 2.5, y1 * 5 + 10 * (y1 - 1) + 2.5, x2 * 5 + 10 * (x2 - 1) + 2.5, y2 * 5 + 10 * (y2-1) + 2.5);
	}
}

bool ofApp::BFS() {
	for (int i = 0; i < 31; i++) { // �湮 ǥ�� array �ʱ�ȭ
		for (int j = 0; j < 31; j++) {
			visited[i][j] = 0;
		}
	}
	row_q = (int*)malloc(sizeof(int) * 31 * 31);
	col_q = (int*)malloc(sizeof(int) * 31 * 31);
	// �������� ť�� �ְ� �湮 ǥ�� �� ���� ���� 0���� ���ϱ�
	addq(1, 1);
	visited[1][1] = 1;
	before[1][1].row = 0;
	before[1][1].col = 0;
	int row, col;
	while (front != rear) {
		delq(&row, &col); // ť���� ����
		// ���� ���� ���� �� ����
		if (row == maze_row && col == maze_col) {
			isBFS = 1;
			return true;
		}
		// �� ���⺰�� �� �� �ִ��� �Ǵ� �� ť�� �ְ� ���� ������ ���� ���� temp�� ��ġ �־��ش�.�湮 ǥ��
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
	//BFS�� ������ ����� �׸���. (3���� ����)
	ofSetColor(200); // �ִ� ��� �ƴ� ���� ���ϰ� ���
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
	ofSetColor(50); // �ִ� ��δ� ���ϰ� ���
	int x1 = maze_col;
	int y1 = maze_row;
	for (;;) {
		int x2 = before[y1][x1].col;
		int y2 = before[y1][x1].row; // ���� ������ ���� ���� �����Ͽ� ��� �ϼ�
		if (x2 == 0 && y2 == 0) break;
		ofDrawLine(x1 * 5 + 10 * (x1 - 1) + 2.5, y1 * 5 + 10 * (y1 - 1) + 2.5, x2 * 5 + 10 * (x2 - 1) + 2.5, y2 * 5 + 10 * (y2 - 1) + 2.5);
		x1 = x2;
		y1 = y2;
	}
}

