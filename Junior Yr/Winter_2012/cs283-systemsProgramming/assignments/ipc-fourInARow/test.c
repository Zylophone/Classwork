#include "csapp.h"

typedef struct{
	int board[10][10]; // a 8x8 gameboard, we use 10x10 array to avoid out of bounds error
	int playerTurn; // represents whose turn it is next
	int winner;     // holds the PID of winning process.  hopefully of the parent
	int gameover;   // is the gameover? Just in case there are no possible moves left, in which case game would be considered a tie
} game;

typedef struct{
	int i; 
	int j;
} position;

// check in all directions around the position given and returns if there is
// a line in the given direction, 
// if direction is -1 then checks all direction
int checkAround(int board[10][10], position *pos, int *currentDirection, int *lineCount);

// checks if there is line
int checkPosLine(int board[10][10], position *pos, int *currentDirection,	position *newPos);

// game logic
int gameLogic(game *g1);
// create a new game
game createGame();
// isValidPos
int isValidPos();

int main(){
	char c='x';
	int m = 10; // number of children to make
	int fd[m][2];
	int i=0;	

	for(i=0; i<=2*m; i+2){
		pipe(fd[i]);
		int pid;
		// create fork or give error
		if((pid = fork()) == -1){
			perror("Error creating fork");
			exit(1);
		}
		if(pid==0){
			// child
			game g1 = createGame();			
			position availableMoves[8];

			int i,j;
			for(i=1; i<=8; i++){
				int skip=0;
				for(j=1;(j<=8 && skip==0); j++){
					if(g1.board[i][j] != 0){
						// the first open spot is [i][j-1] add it to availableMoves[i]
						position p1;
						p1.i = i;
						p1.j = j-1;
						availableMoves[i] = p1;

					} else{
						position p1;
						p1.i = -1;
						p1.j = -1;
						availableMoves[i] = p1;
						skip =1;
					}

				}
			}

			// randomly pick a position
			int moved = 0;
			while(!moved){
				int r = rand() % 9;
				if(isValidPos(availableMoves[r])){
					// make the move
					g1.board[availableMoves[r].i][availableMoves[r].j] = 2;
					moved = 1;
				}
			} // while(!moved)
			g1.playerTurn = 1; // parents turn to make a move

			//write
			write(fd[i+1][1], &g1, (sizeof(g1)));

			//	read
			int nbytes;
			int readi;
			nbytes = read( fd[i][0], &g1, sizeof(g1));
			//sprintf(childString, "%d", readi);
			//printf("Child ;  %s\n", childString);
		}
		else {
			// parent


			// read
			int nbytes;
			int readi;
			nbytes = read(fd[i+1][0], &readi, sizeof(readi));
			int new = readi + 10;
			char *string = Malloc(sizeof(new));	
			sprintf(string, "%d", new);
			printf("i = %d   ; %s\n",i,string);	

			// write 
			write( fd[i][1], &new, (sizeof(new)));

		}
	}

	return(0);
}

int gameLogic(game *g1){
	int highest, skip = 0;
	position bestNextMove;

	int i,j;
	for(i=0; i<=8;i++){
		for(j=0; (j<=8 && skip == 0); j++){
			if(g1->board[i][j] == 0){
				highest =0;
				int *lineCount;
				lineCount = 0;
				// create a new position struct for this spot
				position *p1 = Malloc(sizeof(position));
				p1->i = i;
				p1->j = j;
				int *currentDirection = Malloc(sizeof(int));
				*currentDirection = -1;
				if( checkAround(g1->board, p1, currentDirection, lineCount) ) { // ********* SUNNY LOOK HERE REWRTE THIS 
					if(*lineCount == 3){ // this is deff one of the best moves available
						g1->board[i][j] = 1;
						g1->winner = 1; // ***** SEMI CODE supposed to be PID
						skip = 1;
					} // lineCount if
					else if(*lineCount > highest){
						bestNextMove.i = i;
						bestNextMove.j = j;
						skip = 1;
						highest = *lineCount;
					} // else if(lineCount > highest)
				} // checkAround
			} // g1.board[i][j] if
		} // 2nd for
		if( highest > 0 ){ // we have found a move
			g1->board[bestNextMove.i][bestNextMove.j] = 1;
			return 1;
		} // highest > 0
		else {
			return 0;
		}
	} // 1st for
}


int checkAround(int board[10][10], position *pos, int *currentDirection,	int *lineCount){
	int direction = 0;
	for(direction = 0; direction <=7; direction++){
		if(*currentDirection != direction || *currentDirection == -1){
			position *newPos = Malloc(sizeof(position));
			int d;
			if((d = checkPosLine(board, pos, &direction, newPos)) >= 0){
				// recrusively loop to check for any other matches
				checkAround(board, newPos, &d, lineCount++);
			} else {
				if(lineCount > 0)
					return 1;
				else 
					return 0;
			}
		}
	}
}


int checkPosLine(int board[10][10], position *pos, int *currentDirection, position *newPos){
	int i = pos->i;
	int j = pos->j;

	switch(*currentDirection){
		// chk top
		case 0:
			// another match was found in the direction we're looking
			if(board[i][j-1] == 1){
				newPos->i = i;
				newPos->j = j-1;
				return 0;
			}
			break;
		case 1:
			if(board[i+1][j-1] == 1){
				newPos->i = i+1;
				newPos->j = j-1;
				return 1;
			}		
			break;
		case 2:
			if(board[i+1][j] == 1){
				newPos->i = i+1;
				newPos->j = j;
				return 2;
			}
			break;
		case 3:
			if(board[i+1][j+1] == 1){
				newPos->i = i+1;
				newPos->j = j+1;
				return 3;
			}
			break;

		case 4:
			if(board[i][j+1] == 1){
				newPos->i = i;
				newPos->j = j+1;
				return 4;
			}
			break;
		case 5:
			if(board[i-1][j+1] == 1){
				newPos->i = i-1;
				newPos->j = j+1;
				return 5;
			}
			break;

		case 6:
			if(board[i-1][j] == 1){
				newPos->i = i-1;
				newPos->j = j;
				return 6;
			}
			break;

		case 7:
			if(board[i-1][j-1] == 1){
				newPos->i = i-1;
				newPos->j = j-1;
			}
			break;

		default: return -1; // no match was found
	}
}


game createGame(){
	game g1;
	int i,j,k,w;

	for(i=0;i<10;i++){
		g1.board[i][0] = -1;
		g1.board[i][9] = -1;
	}	
	for(j=0; j<10; j++){
		g1.board[0][j] = -1;
		g1.board[9][j] = -1;
	}
	for(k=1; k<=8; k++){
		for(w=1; w<=8; w++){
			g1.board[k][w] = 0;
		}
	}
}

int isValidPos(position p1){
	if(p1.i == -1 || p1.j == -1)
		return 0;
	return 1;
}
