#include <stdio.h>
#include <ncurses.h>
#include <stdlib.h>

#define KEY_SPACE ' ' // not defined in ncurses.h
#define KEY_Enter 10
#define START_ROW 5
#define START_COL 5

int HEIGHT, WIDTH;



void saveGame(int **board, int row, int col, int turn, int players, WINDOW* win){
	/*
		Save current game with given name 
	*/
	// TODO
	char str[50]; // name that you want to save as a file
	echo(); // show keyboard input
	wmove(win, row, col); 
	wprintw(win, "ENTER FILE NAME : ");
	wgetstr(win, str); // input file name
	FILE* fp = fopen(str, "w"); // create new file
	if(fp == NULL){ // show error if fopen has a error
		wprintw(win, "file open error!\n");
		return;
	}
	// save HEIGHT, WIDTH, board array, current row, col and turn and players as a file
	fprintf(fp, "%d %d\n", HEIGHT, WIDTH);
	for(int i = 0; i<HEIGHT; i++){
		for(int j = 0; j<WIDTH; j++){
			fprintf(fp, "%d\n", board[i][j]);
		}
	}
	fprintf(fp, "%d %d %d %d\n", row, col, turn, players);
	fclose(fp);
}

int** readSavedGame(int **board, int *row, int *col, int *turn, int* players){
	/*
		Read the existing game
	*/
	// TODO 
	char str[50]; // name that you want to load game from
	echo(); // show keyboard input
	wprintw(stdscr, "ENTER FILE NAME : ");
	wgetstr(stdscr, str); // input file name
	for(int i = 0; str[i] != 0; i++){ // delete '\n' from input
		if(str[i] == '\n'){
			str[i] = 0;
			break;
		}
	}
	noecho(); // do not show keyboard input
	FILE* game = NULL;
	game = fopen(str, "r"); // open file as 'r' mode
	if(game == NULL){
		wprintw(stdscr, "file open error!\n");
		exit(1);
	}
	// load data from file
	fscanf(game, "%d %d", &HEIGHT, &WIDTH); // input HEIGHT, WIDTH from file
	// make board memory
	board = (int**)malloc(sizeof(int*)*HEIGHT);
	for(int i=0; i<HEIGHT; i++){
		board[i] = (int*)malloc(sizeof(int)*WIDTH);
	}
	// load board array from file
	for(int i = 0; i<HEIGHT; i++){
		for(int j = 0; j<WIDTH; j++){
			fscanf(game, "%d", &board[i][j]);
		}
	}
	//load row, col, turn, players from file
	fscanf(game, "%d %d %d %d", row, col, turn, players);
	fclose(game); // file close
	return board; // return board array
}

int** initBoard(int **board, int *row, int *col, int *turn, int load, int* players){
	// if player select to load the game
	if(load == 1){
		board = readSavedGame(board, row, col, turn, players); // load board from file
		return board;
	}
	board = (int**)malloc(sizeof(int*)*HEIGHT);
	for(int i=0; i<HEIGHT; i++){
		board[i] = (int*)malloc(sizeof(int)*WIDTH);
	}
	printf("%d %d\n", HEIGHT, WIDTH);
	board[0][0] = ACS_ULCORNER;//'┌'
	for (int i=1; i < WIDTH-1; i++)
		board[0][i] = ACS_TTEE;//'┬'
	board[0][WIDTH-1] = ACS_URCORNER; //'┐'

	for (int i=1; i<HEIGHT-1; i++){
		board[i][0] = ACS_LTEE; // '├'
		for (int j=1; j < WIDTH-1; j++)
			board[i][j] = ACS_PLUS; //'┼'
		board[i][WIDTH-1] = ACS_RTEE; //'┤'
	}

	board[HEIGHT-1][0] = ACS_LLCORNER; //'└'
	for (int i=1; i < WIDTH-1; i++)
		board[HEIGHT-1][i] = ACS_BTEE; //'┴'
	board[HEIGHT-1][WIDTH-1] = ACS_LRCORNER; // '┘'

	return board;
}

void paintBoard(int **board, WINDOW *win, int row, int col){
	/*
		Print the board to the given WINDOW 
		using functions of the ncurses library.
	*/
	// TODO
	for(int i = 0; i<HEIGHT; i++){
		for(int j = 0; j<WIDTH; j++){
			wmove(win, i + 1, j + 1); // move cursor to the place
			waddch(win, board[i][j]); // print board data
		}
		
	}
}

int checkWin(int** board, int players){ // parameters and return type can be modified with any form. 
	/*
		Check if the game is over. 
	*/
	// TODO

	// direction vector that need to check blocks
	int dx[4] = {1, 1, 0, -1};
	int dy[4] = {0, 1, 1, 1};
	int stan; // standard for confirm winning
	if(players == 2){ // when two players are playing
		stan = 5;
	}
	else{ // when three players are playing
		stan = 4;
	}
	int cnt; // count same block's streak
	for(int i = 0; i<HEIGHT; i++){
		for(int j = 0; j<WIDTH; j++){
			int color = board[i][j]; // standard for counting streak
			if(color == 'O' || color == 'X' || color == 'Y'){ // divide current board is a block
				for(int k = 0; k<4; k++){
					// define block aroud color
					int nx = j + dx[k];
					int ny = i + dy[k];
					if(nx >= 0 && nx < WIDTH && ny >= 0 && ny < HEIGHT && board[ny][nx] == color){// it's index in ranges and has same shape
						cnt = 2;
						int a = dx[k];
						int b = dy[k];
						// find the block while it has diffent shape
						while(nx + a >= 0 && nx + a < WIDTH && ny + b >= 0 && ny + b < HEIGHT && board[ny + b][nx + a] == color){
							cnt++;
							a += dx[k];
							b += dy[k];
						}
						// if cnt value has same value with stan and next block has diffent shape return 1
						if(cnt == stan){
							if((j - dx[k] < 0 || i - dy[k] < 0) || board[j - dy[k]][i - dx[k]] != color){
								return 1;
							}
						}
					}
				}
			}
		}
	}
	// if there is no cnt that has same value with stan return 0
	return 0;
}

int Action(WINDOW *win, int **board, int keyin, int *row, int *col, int *turn, int players){
	/*
		following right after the keyboard input,
		perform a corresponding action. 
	*/
	// TODO
	char block[3] = {'O', 'X', 'Y'}; // 3 type of block that players use
	// move the cursor when user click arrow keys
	if(keyin == KEY_UP){
		if(*row > 1) {
			wmove(win, --*row, *col);
			*row--;
		}
		
	}
	else if(keyin == KEY_DOWN){
		if(*row < HEIGHT) {
			wmove(win, ++*row, *col);
			*row++;
		}
	}
	else if(keyin == KEY_LEFT){
		if(*col > 1) {
			wmove(win, *row, --*col);
			*col--;
		}
	}
	else if(keyin == KEY_RIGHT){
		if(*col < WIDTH) {
			wmove(win, *row, ++*col);
			*col++;
		}
	}
	// place a block when user clicks 'Enter' or 'Space' and it is not the place that already dropped the block
	else if(board[*row - 1][*col - 1] != 'O' && board[*row - 1][*col - 1] != 'X' && board[*row - 1][*col - 1] != 'Y' && (keyin == KEY_Enter || keyin == KEY_SPACE)){
		board[*row - 1][*col - 1] = block[(*turn) % players]; // select block by using remainder
		int res = checkWin(board, players); // save the truth value that judge winning
		if(res == 1){ // when any player satisfied winning condition
			return 10;
		}
		return -1;
	}
	// when user want to save the game and when saving game is done, program ends
	else if(keyin == '1'){
		saveGame(board, *row, *col, *turn, players, win);
		return 0;
	}
	// end program with no save
	else if(keyin == '2'){
		return 0;
	}
}


void gameStart(WINDOW *win, int load, int* players){
	int **board;
	int row = 0;
	int col = 0;
	int keyin;
	int turn = 0; // save user's turn
	char block[3] = {'O', 'X', 'Y'}; // block array
	wmove(win, row, col); 
	board = initBoard(board, &row, &col, &turn, load, players); // Initiating the board
	if(load != 1){ // initiating the cursor if there is no loaded data
		row = 1;
		col = 1;
	}
	while(1){
		/* 
			This While loop constantly loops in order to 
			draw every frame of the WINDOW.
		*/

		// TODO LIST
		  // PAINT THE BOARD
		  // PAINT MENU
		  // MOVE CURSOR TO THE LAST POINT 
		  // GET KEYBOARD INPUT
		  // DO ACTION ACCORDING TO THAT INPUT
		  // update WINDOW 
		paintBoard(board, win, row, col); // paint the board
		// paint MENU
		wmove(win, HEIGHT + 3, 4);
		wprintw(win, "Current Turn : %c", block[turn % (*players)]);
		wmove(win, HEIGHT + 4, 4);
		wprintw(win, "1. Press 1 to save");
		wmove(win, HEIGHT + 5, 4);
		wprintw(win, "2. Exit without save");
		// move the cursor to it's right position
		wmove(win, row, col);
		wrefresh(win);
		// input from the keyboard and do action
		keyin = wgetch(win);
		int act = Action(win, board, keyin, &row, &col, &turn, *players);
		if(act == -1){ // calculate user's turn
			turn++; // turn + 1 to go to the next turn
		}
		if(act == 10){ // when checkwin return true
			wmove(win, HEIGHT + 2, 4);
			wprintw(win, "PLAYER%d WIN !! Press any button to terminate the program", turn % (*players) + 1); // player who drop the block is always win when checkWin() is true 
			paintBoard(board, win, row, col); // update Board's screeen
			wrefresh(win);
			wgetch(win); // show the last scene until press any key
			break;
		}
		if(act == 0) break;
	}
	// free board memory
	for(int i = 0; i<HEIGHT; i++){
		free(board[i]);
	}
	free(board);

	return;
}

int main(){
	char load;
	int players;
	
	// TODO 
	/*
		Prompts to ask options of the game
	*/
	int x, y; // window location
	int load_on = 0; // show data is loaded or not (1 : loaded, 0 : not loaded)
	while(1){
		printf("Want to load the game?[y/n] : ");
		scanf("%c", &load);
		getchar();
		// check load is 'y' or 'n'
		if(load == 'y' || load == 'n') break;
		else{
			printf("Enter 'y' or 'n'\n");
		}
	}
	if(load == 'y'){ // load data
		load_on = 1;
		// window location
		x = 0;
		y = 1;
	}
	else if (load == 'n'){
		// window location
		x = 0;
		y = 0;
		load_on = 0;
		// input HEIGHT, WIDTH
		printf("Enter the HEIGHT of the board : ");
		scanf("%d", &HEIGHT);
		printf("Enter the WIDTH of the board : ");
		scanf("%d", &WIDTH);
		// input number of players
		while(1){
			printf("Enter the number of players[2/3] : ");
			scanf("%d", &players);
			// check if players is not 2 or 3
			if(players == 2 || players == 3) break;
			else{
				printf("Wrong number of players!! input 2 or 3\n");
			}
		}
	}
	// TODO LIST
	  // define a window
	  // terminate the window safely so that the terminal settings can be restored safely as well. 
	initscr();
	//define a window
	WINDOW* win = newwin(HEIGHT + 30, WIDTH + 60, y, x);
	// activate keypad(Can use arrow key)
	intrflush(win, 0);
	keypad(win, 1);
	// hide keyboard input
	noecho();
	// start game
	gameStart(win, load_on, &players);
	wrefresh(win);
	// exit window
	endwin();
	return 0;
}
