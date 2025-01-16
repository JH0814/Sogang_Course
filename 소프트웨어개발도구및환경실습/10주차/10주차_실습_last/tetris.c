#include "tetris.h"
static struct sigaction act, oact;
int B,count;

int main(){
	int exit=0;
	createRankList(); // 시작하고 rank에 대한 LinkedList 만듦

	initscr();
	noecho();
	keypad(stdscr, TRUE);	

	srand((unsigned int)time(NULL));

	while(!exit){
		clear();
		switch(menu()){
		case MENU_PLAY: play(); break;
		case MENU_RANK: rank(); break;
		case MENU_EXIT: exit=1; break;
		default: break;
		}
	}
	// 생성한 LinkedList 메모리 해제
	Node* pre = head, *ptr = head->link;
	while(1){
		free(pre);
		pre = ptr;
		if(ptr == NULL) break;
		ptr = ptr->link;
	}

	for(int i = 0; i<CHILDREN_MAX; i++){
		free(Rec_root->child[i]);
	}
	free(Rec_root);

	endwin();
	system("clear");
	return 0;
}

void InitTetris(){
	int i,j;

	for(j=0;j<HEIGHT;j++)
		for(i=0;i<WIDTH;i++)
			field[j][i]=0;

	nextBlock[0]=rand()%7;
	nextBlock[1]=rand()%7;
	nextBlock[2]=rand()%7; // 새로 추가한 nextBlock
	blockRotate=0;
	blockY=-1;
	blockX=WIDTH/2-2;
	score=0;	
	gameOver=0;
	timed_out=0;

	Rec_root = (RecNode*)malloc(sizeof(RecNode));
	Rec_root->level = 0;
	init_Tree(Rec_root);
	recommend(Rec_root);

	DrawOutline();
	DrawField();
	DrawBlockWithFeatures(blockY, blockX, nextBlock[0], blockRotate); // shadow와 block 동시에 그리는 함수
	DrawNextBlock(nextBlock);
	PrintScore(score);
}

void DrawOutline(){	
	int i,j;
	/* 블럭이 떨어지는 공간의 태두리를 그린다.*/
	DrawBox(0,0,HEIGHT,WIDTH);

	/* next block을 보여주는 공간의 태두리를 그린다.*/
	move(2,WIDTH+10);
	printw("NEXT BLOCK");
	DrawBox(3,WIDTH+10,4,8);

	//두 번째 다음 블록 표시 테두리
	DrawBox(9, WIDTH+10, 4, 8);

	/* score를 보여주는 공간의 태두리를 그린다.*/
	move(15,WIDTH+10);
	printw("SCORE");
	DrawBox(16,WIDTH+10,1,8);
}

int GetCommand(){
	int command;
	command = wgetch(stdscr);
	switch(command){
	case KEY_UP:
		break;
	case KEY_DOWN:
		break;
	case KEY_LEFT:
		break;
	case KEY_RIGHT:
		break;
	case ' ':	/* space key*/
		/*fall block*/
		break;
	case 'q':
	case 'Q':
		command = QUIT;
		break;
	default:
		command = NOTHING;
		break;
	}
	return command;
}

int ProcessCommand(int command){
	int ret=1;
	int drawFlag=0;
	switch(command){
	case QUIT:
		ret = QUIT;
		break;
	case KEY_UP:
		if((drawFlag = CheckToMove(field,nextBlock[0],(blockRotate+1)%4,blockY,blockX)))
			blockRotate=(blockRotate+1)%4;
		break;
	case KEY_DOWN:
		if((drawFlag = CheckToMove(field,nextBlock[0],blockRotate,blockY+1,blockX)))
			blockY++;
		break;
	case KEY_RIGHT:
		if((drawFlag = CheckToMove(field,nextBlock[0],blockRotate,blockY,blockX+1)))
			blockX++;
		break;
	case KEY_LEFT:
		if((drawFlag = CheckToMove(field,nextBlock[0],blockRotate,blockY,blockX-1)))
			blockX--;
		break;
	default:
		break;
	}
	if(drawFlag) DrawChange(field,command,nextBlock[0],blockRotate,blockY,blockX);
	return ret;	
}

void DrawField(){
	int i,j;
	for(j=0;j<HEIGHT;j++){
		move(j+1,1);
		for(i=0;i<WIDTH;i++){
			if(field[j][i]==1){
				attron(A_REVERSE);
				printw(" ");
				attroff(A_REVERSE);
			}
			else printw(".");
		}
	}
}


void PrintScore(int score){
	move(17,WIDTH+11); // 위치 수정
	printw("%8d",score);
}

void DrawNextBlock(int *nextBlock){
	int i, j;
	for( i = 0; i < 4; i++ ){
		move(4+i,WIDTH+13);
		for( j = 0; j < 4; j++ ){
			if( block[nextBlock[1]][0][i][j] == 1 ){
				attron(A_REVERSE);
				printw(" ");
				attroff(A_REVERSE);
			}
			else printw(" ");
		}
	}
	for( i = 0; i < 4; i++ ){
		move(10+i,WIDTH+13);
		for( j = 0; j < 4; j++ ){
			if( block[nextBlock[2]][0][i][j] == 1 ){
				attron(A_REVERSE);
				printw(" ");
				attroff(A_REVERSE);
			}
			else printw(" ");
		}
	}
}

void DrawBlock(int y, int x, int blockID,int blockRotate,char tile){
	int i,j;
	for(i=0;i<4;i++)
		for(j=0;j<4;j++){
			if(block[blockID][blockRotate][i][j]==1 && i+y>=0){
				move(i+y+1,j+x+1);
				attron(A_REVERSE);
				printw("%c",tile);
				attroff(A_REVERSE);
			}
		}

	move(HEIGHT,WIDTH+10);
}

void DrawBox(int y,int x, int height, int width){
	int i,j;
	move(y,x);
	addch(ACS_ULCORNER);
	for(i=0;i<width;i++)
		addch(ACS_HLINE);
	addch(ACS_URCORNER);
	for(j=0;j<height;j++){
		move(y+j+1,x);
		addch(ACS_VLINE);
		move(y+j+1,x+width+1);
		addch(ACS_VLINE);
	}
	move(y+j+1,x);
	addch(ACS_LLCORNER);
	for(i=0;i<width;i++)
		addch(ACS_HLINE);
	addch(ACS_LRCORNER);
}

void play(){
	int command;
	clear();
	act.sa_handler = BlockDown;
	sigaction(SIGALRM,&act,&oact);
	InitTetris();
	do{
		if(timed_out==0){
			alarm(1);
			timed_out=1;
		}

		command = GetCommand();
		if(ProcessCommand(command)==QUIT){
			alarm(0);
			DrawBox(HEIGHT/2-1,WIDTH/2-5,1,10);
			move(HEIGHT/2,WIDTH/2-4);
			printw("Good-bye!!");
			refresh();
			getch();

			return;
		}
	}while(!gameOver);

	alarm(0);
	getch();
	DrawBox(HEIGHT/2-1,WIDTH/2-5,1,10);
	move(HEIGHT/2,WIDTH/2-4);
	printw("GameOver!!");
	refresh();
	getch();
	newRank(score);
}

char menu(){
	printw("1. play\n");
	printw("2. rank\n");
	printw("3. recommended play\n");
	printw("4. exit\n");
	return wgetch(stdscr);
}

/////////////////////////첫주차 실습에서 구현해야 할 함수/////////////////////////

int CheckToMove(char f[HEIGHT][WIDTH],int currentBlock,int blockRotate, int blockY, int blockX){
	// user code
	for(int i = 0; i<4; i++){ // 이중 for loop
		for(int j = 0; j<4; j++){
			// block이 칠해지는 칸일 때, 전체 field의 범위를 벗어나거나 field가 이미 채워져 있으면 0 return
			if((f[blockY + i][blockX + j] == 1 || blockY + i >= HEIGHT || blockX + j < 0 || blockX + j >= WIDTH)&& block[currentBlock][blockRotate][i][j] == 1) return 0;
		}
	}
	// for loop 전부 통과시 1 return
	return 1;
}

void DrawChange(char f[HEIGHT][WIDTH],int command,int currentBlock,int blockRotate, int blockY, int blockX){
	// user code
	//1. 이전 블록 정보를 찾는다. ProcessCommand의 switch문을 참조할 것
	int x = blockX, y = blockY, z = blockRotate; // 이전 block의 값을 저장할 variable
	int temp_y = y; // shadow의 y좌표 저장할 변수
	switch(command){ // switch문으로 command의 종류에 따라 나눔
		case KEY_UP: // 위쪽 화살표가 눌렸을 경우
			z = (z + 3) % 4; // 이전의 회전 상태이므로 3 더해줘서 이전 상태 구함
			break;
		case KEY_DOWN: // 아래 화살표의 경우
			y -= 1; // y가 증가했었으므로 이전은 y가 1 작음
			break;
		case KEY_LEFT: // 왼쪽 화살표의 경우
			x += 1; // x가 감소했었으므로 이전은 x가 1 큼
			break;
		case KEY_RIGHT: // 오른쪽 화살표의 경우
			x -= 1; // x가 증가했으므로 이전은 x가 1 작음
			break;
		default:
			break;
	}
	//2. 이전 블록 정보를 지운다. DrawBlock함수 참조할 것.
	for(int i = 0; i<4; i++){ // 이중 for loop
		for(int j = 0; j<4; j++){
			if(block[currentBlock][z][i][j] == 1 && i + y >= 0){ // 이전 상태에서 block이 1이면서 y좌표가 0 이상일 때, 
				move(i + y + 1, j + x + 1); // 해당 위치로 이동
				printw("."); // field의 기본 상태로 출력해서 블록을 지움
			}
		}
	}
	while(CheckToMove(f, nextBlock[0], z, temp_y + 1, x)){ // shadow의 y좌표 획득 위한 while loop
		temp_y++;
	}
	// 이전 shadow를 지운다.
	for(int i = 0; i<4; i++){
		for(int j = 0; j<4; j++){
			if(block[currentBlock][z][i][j] == 1 && i + temp_y >= 0){ // 이전 상태에서 block이 1이면서 y좌표가 0 이상일 때, 
				move(i + temp_y + 1, j + x + 1); // 해당 위치로 이동
				printw("."); // field의 기본 상태로 출력해서 블록을 지움
			}
		}
	}
	// 다시 원래 커서로
	move(HEIGHT, WIDTH + 10);
	//3. 새로운 블록 정보를 그린다. 
	DrawBlockWithFeatures(blockY, blockX, currentBlock, blockRotate); // shadow도 같이 그리도록 추가
	move(HEIGHT, WIDTH + 10); // 다시 원래 커서 위치로
}

void BlockDown(int sig){
	// user code

	//강의자료 p26-27의 플로우차트를 참고한다.
	if(CheckToMove(field, nextBlock[0], blockRotate, blockY + 1, blockX)){ // CheckToMove에서 1 return 시
		blockY++; // 블록의 Y좌표 증가
		DrawChange(field, KEY_DOWN, nextBlock[0], blockRotate, blockY, blockX); // 블록의 위치 화면에서 갱신
	}
	else{
		score += AddBlockToField(field, nextBlock[0], blockRotate, blockY, blockX); // 못 움직일 때는 field에 추가
		if(blockY == -1){ // -1이면 제일 위이므로 gameover
			gameOver = 1;
		}
		else{ // gameover 조건이 아니면
			score += DeleteLine(field); // 제거되는 line 판단하고 점수 더하기
			Rec_ID = nextBlock[0];
			nextBlock[0] = nextBlock[1]; // nextBlock 한 칸 씩 당기기
			nextBlock[1] = nextBlock[2]; // nextBlock 한 칸 씩 당기기
			nextBlock[2] = rand() % 7;// 다음 칸에는 랜덤으로 배치
			recommend(Rec_root);
			// 블록 생성에 필요한 값들 초기화
			blockY = -1;
			blockX = WIDTH/2 - 2;
			blockRotate = 0;
			DrawNextBlock(nextBlock); // 다음 블록 그려놓기
			PrintScore(score); // 화면에 score 갱신
		}
		DrawField(); // field 화면에 갱신
	}
	timed_out = 0; // DrawBlock이 호출될 수 있도록함
}

int AddBlockToField(char f[HEIGHT][WIDTH],int currentBlock,int blockRotate, int blockY, int blockX){
	// user code
	int cnt = 0; // 닿은 면적 저장할 변수

	//Block이 추가된 영역의 필드값을 바꾼다.
	for(int i = 0; i<4; i++){ // 2중 for loop
		for(int j = 0; j<4; j++){
			// 현재 block이 1이고 field의 범위 내에 있을 때
			if(block[currentBlock][blockRotate][i][j] && (blockY + i < HEIGHT && blockY + i >= 0 && blockX + j < WIDTH && blockX + j >= 0)){
				f[blockY + i][blockX + j] = 1; // field를  1로 채워줌
				if(blockY + i == HEIGHT - 1){
					cnt++;
				}
			}
		}
	}
	return cnt * 10;
}

int DeleteLine(char f[HEIGHT][WIDTH]){
	// user code
	int cnt = 0; // 지울 line의 개수
	int flag = 0; // 줄이 꽉 찬 여부 판단 flag
	//1. 필드를 탐색하여, 꽉 찬 구간이 있는지 탐색한다.
	for(int i = 0; i<HEIGHT; i++){ // 가로 한 줄씩 탐색하도록
		flag = 1; // 기본 1로 하고 채워지지 않은 부분 있을 때 0으로
		for(int j = 0; j<WIDTH; j++){ // 한 칸씩 탐색
			if(!f[i][j]){ // f[i][j] == 0일 때(채워지지 않은)
				flag = 0; // flag를 0으로
				break; // 이 후 해당 줄 탐색 필요 없으므로 break
			}
		}
		//2. 꽉 찬 구간이 있으면 해당 구간을 지운다. 즉, 해당 구간으로 필드값을 한칸씩 내린다.
		if(flag){
			cnt++; // flag가 1이면 cnt 증가
			for(int p = i; p>=0; p--){ // 지워질 line부터 한 줄씩 당기기
				for(int q = 0; q<WIDTH; q++){
					f[p][q] = f[p-1][q];
				}
			}
			for(int q = 0; q<WIDTH; q++){ // 제일 윗 줄은 0으로 채워준다.
				f[0][q] = 0;
			}
			i--; // 한 줄씩 당겼으므로 해당 줄부터 또 탐색하도록 1 줄임
		}
	}
	
	return cnt * cnt * 100; // 점수 공식에 따른 return
}

///////////////////////////////////////////////////////////////////////////

void DrawShadow(int y, int x, int blockID,int blockRotate){
	// user code
	int temp_y = y;
	while(CheckToMove(field, blockID, blockRotate, temp_y + 1, x)){ // shadow의 y좌표를 구함
		temp_y++;
	}
	DrawBlock(temp_y, x, blockID, blockRotate, '/'); // '/' 문자를 이용해서 shadow 표시
}
void DrawBlockWithFeatures(int y, int x, int blockID, int blockRotate){ // shadow와 block 동시에 그리기 위한 함수
	DrawShadow(y, x, blockID, blockRotate);
	DrawBlock(y, x, blockID, blockRotate, ' ');
	DrawRecommend(answer->recBlockY, answer->recBlockX, answer->curBlockID, answer->recBlockRotate);
}


void createRankList(){
	// 목적: Input파일인 "rank.txt"에서 랭킹 정보를 읽어들임, 읽어들인 정보로 랭킹 목록 생성
	// 1. "rank.txt"열기
	// 2. 파일에서 랭킹정보 읽어오기
	// 3. LinkedList로 저장
	// 4. 파일 닫기
	FILE *fp;
	char name[NAMELEN + 1]; // 이름 입력 변수
	int input; // 점수 입력 변수
	//1. 파일 열기
	fp = fopen("rank.txt", "r");
	if(fp == NULL){
		printf("no rank.txt\n");
		return;
	}

	// 2. 정보읽어오기
	/* int fscanf(FILE* stream, const char* format, ...);
	stream:데이터를 읽어올 스트림의 FILE 객체를 가리키는 파일포인터
	format: 형식지정자 등등
	변수의 주소: 포인터
	return: 성공할 경우, fscanf 함수는 읽어들인 데이터의 수를 리턴, 실패하면 EOF리턴 */
	fscanf(fp, "%d", &score_number); // rank의 개수를 저장함
	for(int i = 0; i<score_number; i++){ // 입력된 rank의 개수만큼 반복
		fscanf(fp, "%s %d", name, &input); // name과 score 입력 받음
		Node* ptr = head; // 탐색할 Node pointer 변수
		Node* newnode = (Node*)malloc(sizeof(Node)); // 새로운 노드 선언
		newnode->score = input; // 새 노드에 점수 저장
		newnode->name[0] = '\0'; // 새 노드에 name 복사가 잘 이루어지도록 null 값 추가
		strcpy(newnode->name, name); // name 복사
		if(!head) { // head가 null이면
			head = newnode; //head를 newnode로
			head->link = NULL;
		}
		else{//이외에는
			while(ptr->link != NULL){ //제일 뒤까지 이동
				ptr = ptr->link;
			}
			ptr->link = newnode; // 제일 뒤에 newnode 붙이기
		}
	}
	// 4. 파일닫기
	fclose(fp);
}

void rank(){
	//목적: rank 메뉴를 출력하고 점수 순으로 X부터~Y까지 출력함
	//1. 문자열 초기화
	int X=1, Y=score_number, ch;
	clear();

	//2. printw()로 3개의 메뉴출력
	//3. wgetch()를 사용하여 변수 ch에 입력받은 메뉴번호 저장
	while(1){
		printw("1. list ranks from X to Y\n");
		printw("2. list ranks by a specific name\n");
		printw("3. delete a specific rank\n");
		ch = wgetch(stdscr);
		if(ch >= '1' && ch <= '3') break;
	}
	Node* ptr = head;
	echo();
	//4. 각 메뉴에 따라 입력받을 값을 변수에 저장
	//4-1. 메뉴1: X, Y를 입력받고 적절한 input인지 확인 후(X<=Y), X와 Y사이의 rank 출력
	if (ch == '1') {
		// X, Y 입력받음
		printw("X: ");
		scanw("%d", &X);
		printw("Y: ");
		scanw("%d", &Y);
		// 입력이 없을 경우 X = 1, Y = score_number
		if(X < 0) X = 1;
		if(Y < 0) Y = score_number;
		noecho();
		int idx = 1; // 몇 개의 노드를 갔는지 표시
		Node* ptr = head; // 노드 탐색을 위한 ptr
		printw("       name       |     score\n");
		printw("---------------------------\n");
		if(X > Y){ // 예외 처리
			printw("search failure: no rank in the list\n");
			getch();
			return;
		}
		// 개수가 X가 될 때까지 탐색
		while(idx != X && ptr != NULL){
			idx++;
			ptr = ptr->link;
		}
		// X부터 Y-X번 탐색하지만 그전에 ptr이 NULL이 되면 중지
		for(int i = 0; i<=Y-X && ptr != NULL; i++){
			printw("%-18s| %-17d\n", ptr->name, ptr->score);
			ptr = ptr->link;
		}
	}

	//4-2. 메뉴2: 문자열을 받아 저장된 이름과 비교하고 이름에 해당하는 리스트를 출력
	else if ( ch == '2') {
		char str[NAMELEN+1];
		int check = 0; // 찾았는지 여부 판단 flag
		Node* ptr = head; // 노드 탐색용 변수
		printw("input the name: "); 
		scanw("%s", str); // 이름 입력받기
		noecho();
		printw("       name       |     score\n");
		printw("---------------------------\n");
		while(1){ // 탐색 위한 loop
			if(strcmp(ptr->name, str) == 0){ // 이름이 같을 때 출력 후 flag 변경해주기
				printw("%-18s| %-17d\n", ptr->name, ptr->score);
				check = 1;
			}
			ptr = ptr->link;
			if(ptr == NULL) break;
		}
		if(check == 0){ // flag에 변동 없었으면 오류 출력
			printw("search failure: no rank in the list\n");
			getch();
			return;
		}
	}

	//4-3. 메뉴3: rank번호를 입력받아 리스트에서 삭제
	else if ( ch == '3') {
		int num;
		printw("input the rank: ");
		scanw("%d", &num); // rank 입력받기
		noecho();
		if(num > score_number || num <= 0){ // 범위 밖일 때 예외 처리
			printw("\nsearch failure: the rank not in the list\n");
			getch();
			return;
		}
		int idx = 1; // 맞는 순위 찾기 위한 index
		Node* ptr = head, *pre;
		while(idx != num && ptr != NULL){ // loop로 탐색하면서 맞는 num 또는 
			idx++;
			pre = ptr;
			ptr = ptr->link;
		}
		pre->link = ptr->link; // 이전 노드의 link에 ptr의 link를 연결
		free(ptr); // ptr 메모리 해제
		score_number--; // score_number 1 줄이기
		printw("\nresult: rank deleted\n");
		writeRankFile(); // rank.txt 파일에 업데이트
	}
	getch();

}

void writeRankFile(){
	// 목적: 추가된 랭킹 정보가 있으면 새로운 정보를 "rank.txt"에 쓰고 없으면 종료
	//1. "rank.txt" 연다
	FILE *fp = fopen("rank.txt", "w");
	Node* ptr = head;

	//2. 랭킹 정보들의 수를 "rank.txt"에 기록
	fprintf(fp, "%d\n", score_number);

	//3. 탐색할 노드가 더 있는지 체크하고 있으면 다음 노드로 이동, 없으면 종료
	while(1){
		fprintf(fp, "%s %d\n", ptr->name, ptr->score);
		ptr = ptr->link;
		if(ptr == NULL) break;
	}
	
	fclose(fp);
}

void newRank(int score){
	// 목적: GameOver시 호출되어 사용자 이름을 입력받고 score와 함께 리스트의 적절한 위치에 저장
	char str[NAMELEN+1];
	clear();
	echo();
	//1. 사용자 이름을 입력받음
	printw("your name: ");
	scanw("%s", str);
	noecho();
	//2. 새로운 노드를 생성해 이름과 점수를 저장, score_number가 순위의 개수
	Node* newnode = (Node*)malloc(sizeof(Node));
	newnode->score = score;
	newnode->name[0] = '\0';
	newnode->link = NULL;
	strcpy(newnode->name, str);
	score_number++; // 순위의 개수 증가
	if(!head){ // head가 비어있는 경우
		head = newnode;
	}
	else if(!head->link){ // head에만 있는 경우
		if(newnode->score > head->score){
			newnode->link = head;
			head = newnode;
		}
		else{
			head->link = newnode;
		}
	}
	else{
		if(newnode->score > head->score){ //head의 score보다 클 때 예외 처리
			newnode->link = head;
			head = newnode;
			return;
		}
		Node* pre, *ptr = head;
		while(newnode->score <= ptr->score && ptr->link){
			pre = ptr;
			ptr = ptr->link;
		}
		if(pre->score > newnode->score && newnode->score > ptr->score){
			pre->link = newnode;
			newnode->link = ptr;
		}
		else{
			ptr->link = newnode;
		}
	}
	
	writeRankFile();
}

void DrawRecommend(int y, int x, int blockID, int blockRotate) { // 전역 변수에 추천 블록의 정보를 저장하면서 DrawBlock 호출
	Rec_x = answer->recBlockX, Rec_y = answer->recBlockY;
	Rec_ID = answer->curBlockID, Rec_rot = answer->recBlockRotate;
	DrawBlock(y, x, blockID, blockRotate, 'R');
}

int getscore(RecNode* root) { // 노드에서 저장할 추가될 점수 계산
	int score_new = 0;
	score_new =  AddBlockToField(root->recField,root->curBlockID,root->recBlockRotate,root->recBlockY,root->recBlockX)+ DeleteLine(root->recField) ;
	return score_new;
}

int recommend(RecNode* root) {
	answer = Rec_root; // answer를 초기화
	make_Tree(Rec_root); // Tree에서 모든 경우의 수 계산하면서 answer 업데이트
	if(answer->parent != NULL){
		while (answer->parent->parent != NULL) { // 트리에서 구하는 level과 맞도록 조정
			answer = answer->parent;
		}
	}
	else{ // answer->parent == NULL인 경우가 answer로 되어 있을 경우 예외처리
		int index, max = 0;
		for(int i = 0; i<answer->index; i++){
			if(answer->child[i]->score > max) index = i;
		}
		answer = answer->child[index];
	}
	return answer->score; // 점수 반환
}

void init_Tree(RecNode* root) {
	if (root->level == 0) {// 첫 노드 생성 시
		// 기존의 recommend 삭제
		int y = Rec_y, x = Rec_x;
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				if (block[Rec_ID][Rec_rot][i][j] == 1 && i + y >= 0) {
					move(i + y + 1, j + x + 1);
					printw(".");
				}
			}
		}
		// 값들 초기화
		root->level = 0;
		root->parent = NULL;
		root->score = 0;
		root->index = 1;
		root->recBlockRotate = 0;
		for (int i = 0; i < HEIGHT; i++) {
			for (int j = 0; j < WIDTH; j++) {
				root->recField[i][j] = field[i][j];
			}
		}
	}
	// child 노드는 VISIBLE_BLOCKS level까지 CHILDREN_MAX만큼 생성
	if (root->level <= VISIBLE_BLOCKS) {
		for (int i = 0; i < CHILDREN_MAX; i++) {
			root->child[i] = (RecNode*)malloc(sizeof(RecNode));
			root->child[i]->level = root->level + 1;
			root->child[i]->score = 0;
			root->child[i]->parent = root;
			init_Tree(root->child[i]); // 재귀 호출로 그 다음 층 초기화
		}
	}
}
void make_Tree(RecNode* root) { //트리 만들기
	int idx = 0, y = 0;
	if (root->level == 0) { // 루트 노드일 때
		for (int i = 0; i < VISIBLE_BLOCKS; i++) {
			arr[i] = nextBlock[i]; // nextBlock 배열 임시 저장
		}
		for (int i = 0; i < HEIGHT; i++) { // field 값 받아오기
			for (int j = 0; j < WIDTH; j++) {
				root->recField[i][j] = field[i][j];
			}
		}
	}
	for (int rot = 0; rot < 4; rot++) { // 회전 모두 판별
		y = 0;
		for (int x = -1; x < WIDTH; x++) { // x 좌표 모두 구분
			if (CheckToMove(root->recField, arr[root->level], rot, y, x) == 1) { // 일단 제일 위에 둘 수 있을 때
				for (int i = 0; i < HEIGHT;i++) { // 부모 노드로부터 필드 받아옴
					for (int j = 0; j < WIDTH; j++) {
						root->child[idx]->recField[i][j] = root->recField[i][j];
					}
				}
				// 자식 노드 값에 맞게 초기화
				root->child[idx]->recBlockY = 0;
				root->child[idx]->recBlockX = x;
				root->child[idx]->curBlockID = arr[root->level];
				// 해당 x좌표에서 y좌표 어디까지 내려가는지 판별
				while (CheckToMove(root->recField, arr[root->level], rot, root->child[idx]->recBlockY, x) == 1 && root->child[idx]->recBlockY < HEIGHT) {
					root->child[idx]->recBlockY++;
				}
				root->child[idx]->recBlockY--;
				// 회전수와 score도 반영
				root->child[idx]->recBlockRotate = rot;
				root->child[idx]->score = root->score + getscore(root->child[idx]);
				//최대 값 비교 후 최대이면 answer 변경(단 노드의 level이 VISIBLE_BLOCKS-1 이상인 경우만)
				if (answer->score < root->child[idx]->score && root->child[idx]->level >= VISIBLE_BLOCKS - 1) {
					answer = root->child[idx];
				}
				idx++;
			}
		}
	}
	// 자식 노드 개수 저장
	root->index = idx;
	// 정해진 레벨 도착 전에는 재귀로 반복
	if (root->level < VISIBLE_BLOCKS-1) {
		for (int i = 0; i < idx; i++) {
				make_Tree(root->child[i]);
			}
	}
	return ;
}


void recommendedPlay(){
	// user code
}
