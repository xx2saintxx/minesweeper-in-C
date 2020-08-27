// Toussaint Turnier
// 333 Dr. Phipps
// Part 2 of Project: Minesweeper
// 03/23/2020
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

const int MAX_EDGE = 9; // Max length of rows and columns.
const int NUMBOMBS = 10; // Number of bombs on the board
const double ACCURACY = 0.001; // Accuracy for RNG and Proximity
void printboard(char[][MAX_EDGE]);
int flagMove(char[][MAX_EDGE],char[][MAX_EDGE],int, int);
int flagRemove(char[][MAX_EDGE],char[][MAX_EDGE],int, int);
int domove(char[][MAX_EDGE], char[][MAX_EDGE], int[][2], int, int, int*);
int valid(int,int);
int numberbombs(int,int,int[][2]);
int initialize();
int teardown();
void acceptInput(int*, int*);
void getbombs(int[][2]);
char decision; //Variable for switch statement

//ALGORITHIM FOR REVEALING SQUARES (doMOVE progressive case)
//USER ASSERTS SQUARE
//PROGRAM CHECKS FOR BOMB
//BOMB = GAME OVER
//NO BOMB = ASSERTED
//ASSERTED REVEAL ADJACENT BOMBS
//IF BOMB WITHIN 1-2 SPACE IS ASSERTED BUT WITH A 1 OR 2 
//IF NO BOMB WITHIN 1-2 SPACES, SPACE IS ASSERTED WITH A 0

//if(bomb == true) Print "Game over"
//if(bomb == false){
//Function BombProximity (its just called doMove's progressive case)
//for square Xaxis (i=-1;i<2;i++)
//for square Yaxis (i=-1;i<2;i++)
//if(BombProximity 1 || 2) Print  "1" or "2" doMove +i +j}
// else Print "0"
    

int main( int argc, char *argv[] ) {
int height = atoi(argv[1]);
int width = atoi(argv[2]); 

int i; // For loop variable for X
int j; // For loop variable for Y
int x; // Horizontal Axis
int y; // Verticle Axis
char normal_board[MAX_EDGE][MAX_EDGE]; //Stores a Normal board For asserted squares and unknowns.
char flag_board[MAX_EDGE][MAX_EDGE]; // Stores Flag locations on the board
char bomb_board[MAX_EDGE][MAX_EDGE]; //Stores BOMBS locations on the board
int totalmoves; // Stores the total moves left for the user before they win.
int gameover = 0; // used to check if the game is over or not

    int listbombs[NUMBOMBS][2]; // stores (x,y) coordinates of all bombs.
  
    srand(time(0)); //RNG SEED

    // Initialize the current board.
    for (i=0;i<MAX_EDGE;i++) {
        for (j=0;j<MAX_EDGE;j++) {
            normal_board[i][j] = '_';
            flag_board[i][j] = '_';
            bomb_board[i][j] = '_';
        }
    }
    getbombs(listbombs); //RNG bombplacement 1/4th chance
    for (i=0;i<NUMBOMBS;i++) 
    bomb_board[listbombs[i][0]][listbombs[i][1]]='*';
    totalmoves = MAX_EDGE*MAX_EDGE - NUMBOMBS;


// Initializes the game
do{
   switch(decision) {
    // Accept Input
   initialize();
   printf("What Will You Do?\nPress A for Flag\nPress B for Remove Flag\nPress C for Assert\nPress D to Quit\n");
   scanf("%s", &decision); 
      
      //PLACING A FLAG
         case 'A' : 
         printf("Where Would You like to Put Your Flag?\n" );
         acceptInput(&x,&y);
         flagMove(normal_board,flag_board,x,y);
         printf("What Will You Do?\nPress A for Flag\nPress B for Remove Flag\nPress C for Assert\nPress D to Quit\n");
         printboard(flag_board);
         scanf("%s", &decision); //space b/w %c and %c)
         break;

      
      //REMOVING A FLAG
      case 'B' : 
         printboard(flag_board);
         printf("Type in the coordinates of the Flag to be removed:\n" );
         acceptInput(&x,&y);
         flagRemove(normal_board,flag_board,x,y);
         printboard(flag_board);
         printf("What Will You Do?\nPress A for Flag\nPress B for Remove Flag\nPress C for Assert\nPress D to Quit");
         scanf("%s", &decision); //space b/w %c and %c)
         break;

        //ASSERTING 
      case 'C' : 
         printboard(flag_board);
         printf("Where Would You like to Assert?\n" );
         acceptInput(&x,&y);

        gameover = domove(normal_board,bomb_board,listbombs,x,y,&totalmoves);

        if ((!gameover) && (totalmoves==0)) {
            printf("Great! You win!!!\n");
            gameover = 1;
           }
         printf("What Will You Do?\nPress A for Flag\nPress B for Remove Flag\nPress C for Assert\nPress D to Quit");
         printboard(normal_board);
         scanf("%s", &decision);
        break;

     //QUIT OUT OF PROGRAM
      case 'D' : 
         teardown();
         printf("Goodbye. Thank you for Playing\n" );
         gameover = 1;
         return 0;
      
      //DEFAULT  
      default : 
         printf("Invalid input\n" );
         printf("What Will You Do?\nPress A for Flag\nPress B for Remove Flag\nPress C for Assert\nPress D to Quit");
         scanf("%s", &decision); //space b/w %c and %c)
   }
}while(!gameover); //ENDS LOOP 
} // End of Main

void printboard(char board[][MAX_EDGE]) {

    int i,j;

    // Print out side labels.
    printf("    ");
    for (i=0;i<MAX_EDGE;i++)
        printf("%d ", i);
    printf("\n\n");

    // Print out actual board.
    for (i=0;i<MAX_EDGE;i++) {
    
        printf("%d   ", i);
        for (j=0;j<MAX_EDGE;j++)
            printf("%c ",board[i][j]); 
        printf("\n");
    }
}

int initialize(){
printf("Initializing the Game...\n");
return 0;
}

int teardown(){
    printf("Destroying the Game...\n");
    return 0;
}

//ACCEPT INPUT FUNCTION
void acceptInput(int *x,int *y) {
// Ask for User input
printf("Enter the coordinates with a space in between: ");
scanf("%d%d", x, y);
// Re-read if invalid.
while (!valid(*x,*y)) {
printf("Those coordinates were invalid, please try a different one.\n");
printf("Enter the coordinates with a space in between, (row_column): ");
scanf("%d%d", x, y);
    }
}


//UPDATE FUNCTION: USED FOR FLAG "CHOICE A"
int flagMove(char board[][MAX_EDGE],char flag_board[][MAX_EDGE], int row, int column){
// ALREADY CHOSEN CASE: If a square has already been Asserted on
if (board[row][column]!='_'){
printf("You've already cleared that square. Try a different coordinate.\n");
        return 0;
    }
// ALREADY CHOSEN CASE: If a square has already been Flagged
else if (flag_board[row][column]=='F'){
printf("You've already flagged that square. Try a different coordinate.\n");
        return 0;
//PROGRESSIVE CASE: Place a Flag down
} if(valid(row,column) && (board[row][column]=='_'))
board[row][column] = 'F';
flag_board[row][column] = 'F';
printf("Flag has been placed.\n");
return 0;
}

//UPDATE FUNCTION: USED FOR REMOVE FLAG "CHOICE B"
int flagRemove(char board[][MAX_EDGE], char flag_board[][MAX_EDGE], int row, int column){
// ALREADY CHOSEN CASE: If a square has already been Asserted on
if (flag_board[row][column]=='_'){
printf("Either you haven't Flagged this square or it's been revealed. Try a different coordinate.\n");
        return 0;
    }
//PROGRESSIVE CASE: Remove the Flag
else if(valid(row,column) && (flag_board[row][column]=='F')){
board[row][column] = '_';
flag_board[row][column] = '_';
printf("Flag has been removed.\n");
return 0;
}
return 0;
}

//UPDATE FUNCTION: USED FOR ASSERT "CHOICE C"
int domove(char board[][MAX_EDGE], char bomb_board[][MAX_EDGE], int listbombs[][2], int row, int column, int *totalmoves) {
int i;
int j; 
int num;
//LOSING CASE: inputting a move where you lose.
if (bomb_board[row][column]=='*'){
    board[row][column]='*';
        for (i=0;i<NUMBOMBS;i++) 
            board[listbombs[i][0]][listbombs[i][1]]='*';
        printboard(bomb_board);
        printf("BOOM! GAMEOVER. BETTER LUCK NEXT TIME!\n");
        return 1;
    }
// ALREADY CHOSEN CASE: If a square has already been asserted on
else if (board[row][column]!='_'){
printf("You've already cleared that square. Try a different coordinate.\n");
        return 0;
    }
    // ALREADY CHOSEN CASE: If a square has been FLAGGED
else if (board[row][column]=='F'){
printf("You've Flagged that square. Remove the flag first, or try a different coordinate.\n");
        return 0;
    }
// PROGESSIVE CASE: If game goes on (Bomb proximity ALGORITHIM)
    else {
        num = numberbombs(row, column, listbombs); 
        (*totalmoves)--;
        board[row][column]=(char)(num+'0');
        if (num == 0) { // if no bombs nearby  
            for (i=-1;i<2;i++) {
	            for (j=-1;j<2;j++) {
	               if (valid(row+i,column+j) && (board[row+i][column+j]=='_'))
	                domove(board, bomb_board, listbombs, row+i, column+j, totalmoves);
	            }
            }
	      
        }
 return 0;
        } // end else
}


//Determines if user choice is within bounds
int valid(int row, int column) {
if (row < 0){
return 0;    
}else if(row >= MAX_EDGE){
return 0;
}else if(column < 0){
    return 0;
}else if (column >= MAX_EDGE){
    return 0;
}else return 1;
}


//Bomb Counter for proximity
int numberbombs(int row ,int column ,int listbombs[][2]) {
int i;
int count = 0;
for (i=0;i<NUMBOMBS;i++) {
// Check each bomb one by one against the square to uncover.
    if ((abs(row-listbombs[i][0])<=1+ACCURACY) && (abs(column-listbombs[i][1])<=1+ACCURACY)) {
            count++; 
        }  
    }   
    return count;
}

//Creates bombs
void getbombs(int bomblist[][2]) {
  
    int i;
    int bombrng;
    int curbombs[MAX_EDGE*MAX_EDGE];
  
    for (i=0; i<MAX_EDGE*MAX_EDGE; i++)
        curbombs[i] = 0;
        i = 0; //counter
    while(i<NUMBOMBS) {
    bombrng = rand()%(MAX_EDGE*MAX_EDGE);
    // Check for cloned bomb, then add to the bomblist
        if (curbombs[bombrng]==0) {
            bomblist[i][0] = bombrng/MAX_EDGE;
            bomblist[i][1] = bombrng%MAX_EDGE;
            i++;
            curbombs[bombrng] = 1;
        }
    }
} // END OF CODE