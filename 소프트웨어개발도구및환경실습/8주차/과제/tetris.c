#include "tetris.h"

static struct sigaction act, oact;

int main(){
	int exit=0;

	initscr();
	noecho();
	keypad(stdscr, TRUE);	

	srand((unsigned int)time(NULL));

	while(!exit){
		clear();
		switch(menu()){
		case MENU_PLAY: play(); break;
		case MENU_EXIT: exit=1; break;
		default: break;
		}
	}

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
			nextBlock[0] = nextBlock[1]; // nextBlock 한 칸 씩 당기기
			nextBlock[1] = nextBlock[2]; // nextBlock 한 칸 씩 당기기
			nextBlock[2] = rand() % 7;// 다음 칸에는 랜덤으로 배치
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
}

void createRankList(){
	// user code
}

void rank(){
	// user code
}

void writeRankFile(){
	// user code
}

void newRank(int score){
	// user code
}

void DrawRecommend(int y, int x, int blockID,int blockRotate){
	// user code
}

int recommend(RecNode *root){
	int max=0; // 미리 보이는 블럭의 추천 배치까지 고려했을 때 얻을 수 있는 최대 점수

	// user code

	return max;
}

void recommendedPlay(){
	// user code
}
