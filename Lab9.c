/* 
 * File:   main.c
 * Author: Pranit
 *
 * Created on November 3, 2014, 6:16 PM
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

char board [21] [21];
int chosenRow, chosenCol;

void printBoard(char board[21] [21], int n);
int createBoard();
void getBlockedSquares(int);
bool getMove(bool color, int *row, int *col);
bool makeMove(bool color, int row, int col, int n);
int checkWinner(int n);
int longestRun(int n, int row, int col);
void bestPos(int n, int *row, int *col);
int longestSequenceBlack(int n, int row, int col);
int longestSequenceWhite(int n, int row, int col);
bool allOccupied(int n);
void AI(int n, int *row, int *col, char player);
int recursivePicker(int numIteration, int depth, char currentPlayer, int n, char myPlayer);
int getScore(int n, int row, int col);

int main(int argc, char** argv) {  
    //Main function
    int size, row, col;
    bool color=0, getMoveFlag, makeMoveFlag;
    
    char computerPlayer[5];
    
    //Calling the createBoard function to create the board and save the value of the size of the board it returns
    size= createBoard();
    
    //Printing the created board
    printBoard(board, size);

    //Asking the user if the computer will be playing as B or W
    printf("Computer playing B or W?: ");
    scanf("%s", computerPlayer);
    
    
    //Calling the getBlockedSquares function to get the blocked squares
    getBlockedSquares(size);
    
    //Do while loops to make the computer move and ask the user for moves
    
    int bestRow, bestCol;
    
    
       /*do{
            if(!allOccupied(size)){
            bestPos(size, &bestRow, &bestCol);
            printf("Lab8 moves B at %d %d\n", bestRow, bestCol);
            board [bestRow] [bestCol] = 'B';
            printBoard(board, size);
            if(checkWinner(size) == 0 && !allOccupied(size)){
                AI(size, &bestRow, &bestCol, 'W');
                printf("Genius moves W at %d %d\n", bestRow, bestCol);
                board [bestRow] [bestCol] = 'W';
                printBoard(board, size);
            }
            }
        }while (checkWinner(size) == 0 && !allOccupied(size));*/
        
        
        if(computerPlayer[0]== 'B'){
        do{
            if(!allOccupied(size)){
            AI(size, &bestRow, &bestCol, 'B');
            printf("Computer moves B at %d %d\n", bestRow, bestCol);
            board [bestRow] [bestCol] = 'B';
            printBoard(board, size);
            if(checkWinner(size) == 0 && !allOccupied(size)){
            getMoveFlag = getMove(1, &row, &col);
            if (getMoveFlag) {
                do {
                    makeMoveFlag = makeMove(1, row, col, size);
                    if (!makeMoveFlag) {
                        getMoveFlag= getMove(1, &row, &col);
                        if (!getMoveFlag)
                            makeMoveFlag = true;
                    }
                } while (!makeMoveFlag);
            }
            }
            }
        }while (checkWinner(size) == 0 && !allOccupied(size));
    }
    
    else {
        do {
            if(!allOccupied(size))
            {
            getMoveFlag = getMove(0, &row, &col);
            if (getMoveFlag && checkWinner(size) == 0 && !allOccupied(size)) {
                do {
                    makeMoveFlag = makeMove(0, row, col, size);
                    if (!makeMoveFlag) {
                        getMoveFlag= getMove(0, &row, &col);
                        if (!getMoveFlag)
                            makeMoveFlag = true;
                    }
                } while (!makeMoveFlag);
            }
            AI(size, &bestRow, &bestCol ,'W');
            printf("Computer moves W at %d %d\n", bestRow, bestCol);
            board [bestRow] [bestCol] = 'W';
            printBoard(board, size);
            }
        } while (checkWinner(size) == 0 && !allOccupied(size));
    }
    
    int winner= checkWinner(size);
    if(winner == 1)
        printf("\nBlack player wins.");
    else if(winner == 2)
        printf("\nWhite player wins.");
    else
        printf("\nDraw!");

    return (EXIT_SUCCESS);
}

int createBoard(){
    int i, j, size;
    
    //Creating a board of the given dimensions
    printf("Enter board dimensions (n), n>=6 and n<=21: ");
    scanf("%d", &size);
    
    //Populating the board with 'U's
    for(i=0; i<size; i++)
        for(j=0; j<size; j++)
            board[i] [j] = 'U';
    
    return size;
}

void printBoard(char board[21] [21], int n){
    //Function which loops through the board and prints each element
    int i, j;
    for(i=0; i<n; i++){
        for(j=0; j<n; j++)
            printf("%c", board[i][j]);
        printf("\n");
    }
}

void getBlockedSquares(int n){
    //Function which gets the position of blocked squares from the user until the user types -1 -1
    int row, col;
    do{
        printf("Enter position (ROW COL) of blocked square; (-1 -1) to finish: ");
        scanf("%d %d", &row, &col);
        if(board[row] [col] == 'R')
            printf("Position already occupied, try again\n");
        else if(row == -1 && col == -1);
        else if(row>=n || col>=n || row<0 || col <0)
            printf("Out of range row or column\n");
        else{
            board[row] [col]= 'R';
            printBoard(board, n);
        }
    } while(row != -1 && col != -1);
}

bool getMove(bool color, int *row, int *col){
    //Function which gets the moves from the player
    //It receives a boolean value called color which tells it whether it should ask for a white move or a black move
    int tempRow, tempCol;
    if(color == 0)
        printf("Enter Black Move (ROW COL): ");
    else
        printf("Enter White Move (ROW COL): ");
    scanf("%d %d", &tempRow, &tempCol);
    *row= tempRow;
    *col= tempCol;
    /*(if(tempRow == -1 && tempCol == -1){
        return false;
    }*/
    return true;
}

bool makeMove(bool color, int row, int col, int n){
    //Function which receives a move and translates that move into the board array
    
    //Gives an error if the position is already blocked or out of range
    if(row>=n || col>=n || row<0 || col <0){
        printf("Out of range row or column\n");
        return false;
    }
    else if(board[row] [col] == 'R' || board[row] [col] == 'B' || board[row] [col] == 'W'){
        printf("That square is already occupied or blocked\n");
        return false;
    }
    //Making changes to the array
    else{
        if(color ==0 ){
            board[row] [col]= 'B';
            printBoard(board, n);
        }
        else{
            board[row] [col]= 'W';
            printBoard(board, n);
        }
    }
    return true;
}

int checkWinner(int n){
    //Function which tries to find a winner on the board when called, if no winner is found it says so
    int i, j, k;
    bool found= false;
    //Looping through each position on the board
    for(i=0; i<n && !found; i++){
        for(j=0; j<n && !found; j++){
            int counter=0;
            //Checking for horizontal winners
            for(k=0; k< n-j && !found && counter <6; k++){
                if(board [i] [j] == board [i] [j+k])
                    counter++;
                else
                    counter = 0;
            }
            //If the counter reaches zero, it exits the whole loop and the function because a winner has been found
            if(counter==6){
                if(board[i] [j] == 'B'){
                    //a Black win
                    return 1;
                    found=true;
                }
                else if(board[i] [j] == 'W'){
                    //A White win
                    return 2;
                    found=true;
                }
            }
            counter=0;
            //Looking for vertical winners
            for(k=0; k< n-i && !found && counter <6; k++){
                if(board [i] [j] == board [i+k] [j])
                    counter++;
                else
                    counter = 0;
            }
            if(counter==6){
                if(board[i] [j] == 'B'){
                    return 1;
                    found=true;
                }
                else if(board[i] [j] == 'W'){
                    return 2;
                    found=true;
                }
            }
            
            counter=0;
            //Looking for diagonal winners
            for(k=0; k< fmin(n-i, n-j) && !found && counter <6; k++){
                if(board [i] [j] == board [i+k] [j+k])
                    counter++;
                else
                    counter = 0;
            }
            if(counter==6){
                if(board[i] [j] == 'B'){
                    return 1;
                    found=true;
                }
                else if(board[i] [j] == 'W'){
                    return 2;
                    found=true;
                }
            }
            
            counter=0;
            for(k=0; k < fmin(n-i, j+1) && !found && counter < 6; k++){
                if(board [i] [j] == board [i-k] [j-k])
                    counter++;
                else
                    counter = 0;
            }
            if(counter==6){
                if(board[i] [j] == 'B'){
                    return 1;
                    found=true;
                }
                else if(board[i] [j] == 'W'){
                    return 2;
                    found=true;
                }
            }
        }
    }
    
    /*for(i=0; i<n && !found; i++){
        for(j=0; j<n && !found; j++){
            if(longestRun(n, i, j) == 6){
                if(board[i] [j] == 'B'){
                    return 1;
                    found=true;
                }
                else if(board[i] [j] == 'W'){
                    return 2;
                    found=true;
                }
            }
        }
    }*/
    
    //If no winner has been found yet, it prints "No winner so far"
    if(found== false){
        return 0;
    }
}

int longestRun(int n, int row, int col){
    //Function which searches for the longest run for a given position
    //Here the longest run is stored in the variable max, the value of max is only changed if a run which is
    //greater than max is found
    int i, j, counter, max = 0;
    
    //Checking for erroneous input
    if (row >= n || col >= n)
        return -1;
    else if (board [row] [col] != 'B' && board [row] [col] != 'W')
        return -2;
    else {
        counter = 0;
        //Searching for a horizontal longest run
        for (i = 0; i < n - col && board [row] [col] == board[row] [col + i]; i++) {
            counter++;
        }
        if (counter >= max)
            max = counter;

        counter = 0;
        for (i = 0; i < col + 1 && board [row] [col] == board[row] [col - i]; i++) {
            counter++;
        }

        if (col != n - 1 && col != 0 && board[row] [col + 1] == board [row] [col - 1])
            max += counter - 1;

        else if (counter >= max)
            max = counter;

        //Searching for a vertical longest run
        counter = 0;
        for (i = 0; i < n - row && board [row] [col] == board[row + i] [col]; i++) {
            counter++;
        }
        if (counter >= max)
            max = counter;

        counter = 0;
        for (i = 0; i < row + 1 && board [row] [col] == board[row - i] [col]; i++) {
            counter++;
        }

        if (row != n - 1 && row != 0 && board[row + 1] [col] == board [row - 1] [col])
            max += counter - 1;
        else if (counter >= max)
            max = counter;

        //Searching for a left-diagonal longest run
        counter = 0;
        for (i = 0; i < fmin(n - row, n - col) && board [row] [col] == board[row + i] [col + i]; i++) {
            counter++;
        }
        if (counter >= max)
            max = counter;

        counter = 0;
        for (i = 0; i < fmin(row + 1, col + 1) && board [row] [col] == board[row - i] [col - i]; i++) {
            counter++;
        }

        if ((row != 0 && col != 0) && (row != n - 1 && col != n - 1) && board [row + 1] [col + 1] == board[row - 1] [col - 1])
            max += counter - 1;
        else if (counter >= max)
            max = counter;

        //Searching for a right-diagonal longest run
        counter = 0;
        for (i = 0; i < fmin(n - col, row + 1) && board [row] [col] == board[row - i] [col + i]; i++) {
            counter++;
        }
        if (counter >= max)
            max = counter;

        counter = 0;

        for (i = 0; i < fmin(n - row, col + 1) && board [row] [col] == board[row + i] [col - i]; i++) {
            counter++;
        }
        if ((row != 0 && col != n - 1) && (row != n - 1 && col != 0) && board [row - 1] [col + 1] == board[row + 1] [col - 1])
            max += counter - 1;

        else if (counter >= max)
            max = counter;

        //Returning the longest run which is stored in the variable max
        return max;
    }

}

void bestPos(int n, int *row, int *col){
    //Function to find the best unoccupied position in the table at a give point
    int i, j, tempScore, score =0, bestRow, bestCol;
    for(i=0; i< n; i++){
        for(j=0; j< n; j++){
            if(board [i] [j] == 'U'){
                tempScore= longestSequenceBlack(n, i, j) + longestSequenceWhite (n, i, j);
                if(tempScore > score){
                    score= tempScore;
                    bestRow = i;
                    bestCol= j;
                }
            }
        }
    }
    *row = bestRow;
    *col = bestCol;
}

void AI(int n, int *row, int *col, char player){
    bool found = false;
    int i, j, tempScore, score =0, bestRow, bestCol, longestSequence;
    if(!found){
        for(i=0; i<n; i++){
            for(j=0; j<n; j++){
                if(board [i] [j] == 'U'){
                    if(player == 'B')
                        longestSequence = longestSequenceBlack(n, i, j);
                    else
                        longestSequence = longestSequenceWhite(n, i, j);
                    if(longestSequence == 6){
                        found = true;
                        bestRow = i;
                        bestCol = j;
                    }
                }
            }
        }
    }
    if(!found){
        for(i=0; i<n; i++){
            for(j=0; j<n; j++){
                if(board [i] [j] == 'U'){
                    if(player == 'B')
                        longestSequence = longestSequenceWhite(n, i, j);
                    else
                        longestSequence = longestSequenceBlack(n, i, j);
                    if(longestSequence == 6){
                        found = true;
                        bestRow = i;
                        bestCol = j;
                    }
                }
            }
        }
    }
    if(!found){
        for(i=0; i<n; i++){
            for(j=0; j<n; j++){
                if(board [i] [j] == 'U'){
                    if(player == 'B')
                        longestSequence = longestSequenceBlack(n, i, j);
                    else
                        longestSequence = longestSequenceWhite(n, i, j);
                    if(longestSequence == 5){
                        found = true;
                        bestRow = i;
                        bestCol = j;
                    }
                }
            }
        }
    }
    if(!found){
        int score= recursivePicker(0, 2, player, n, player);
        if(chosenRow <n && chosenRow>=0 && chosenCol <n && chosenCol>= 0){
            found = true;
            bestRow= chosenRow;
            bestCol= chosenCol;
        }
    }
    if(!found){
        for(i=0; i< n; i++){
            for(j=0; j< n; j++){
                if(board [i] [j] == 'U'){
                    if(player == 'B')
                        tempScore= longestSequenceBlack(n, i, j) + longestSequenceWhite (n, i, j);
                    else
                        tempScore= longestSequenceBlack(n, i, j) + longestSequenceWhite (n, i, j);
                    if(tempScore > score){
                        //Theoretically the highest possible score is 2n
                        score= tempScore;
                        bestRow = i;
                        bestCol= j;
                    }
                }
            }
        }
    }
    *row = bestRow;
    *col = bestCol;
}

int recursivePicker(int numIteration, int depth, char currentPlayer, int n, char myPlayer){
    int i, j, tempScore, score=0, bestRow, bestCol;
    if (depth==0)
        return 0;
    
    for(i=0; i<n; i++){
        for(j=0; j<n; j++){
            tempScore= getScore(n, i, j);
            if(currentPlayer == 'B')
                    tempScore= recursivePicker(numIteration+1, depth-1, 'W', n, myPlayer);
                else
                    tempScore= recursivePicker(numIteration+1, depth-1, 'B', n, myPlayer); 
            if(currentPlayer== myPlayer && tempScore>score){
                score = tempScore;
                bestRow= i;
                bestCol=j;
            }
            else if(currentPlayer != myPlayer && tempScore<score){
                score = tempScore; 
                bestRow= i;
                bestCol=j;
            }
        }
    }
    if(numIteration==0){
        chosenRow= bestRow;
        chosenCol= bestCol;
    }
    return score;
}

int longestSequenceBlack(int n, int row, int col){
    //Function to find the longest run if a give unoccupied position were to become B
    int longest;
    char temp;
    temp = board [row] [col];
    board [row] [col] = 'B';
    longest = longestRun(n, row, col);
    board [row] [col]= temp;
    return longest;
}

int longestSequenceWhite(int n, int row, int col){
    //Function to find the longest run if a give unoccupied position were to become W
    int longest;
    char temp;
    temp = board [row] [col];
    board [row] [col] = 'W';
    longest = longestRun(n, row, col);
    board [row] [col]= temp;
    return longest;
}

bool allOccupied(int n){
    //Function which loops through the whole board to check if it is full or if it still has unoccupied spaces
    int i, j;
    for(i=0; i<n; i++){
        for(j=0; j<n; j++){
            if(board [i] [j] == 'U')
                return false;
        }
    }
    return true;
}

bool findPotentialHorizonalRight(int n, int row, int col, char color){
    int i;
    for (i = 0; i < n - col; i++) {
        if(i<6){
            if(board [row] [col+i] != 'U' && board [row] [col+i] != color)
                return false;
        }
    }
    if(i<6)
        return false;
    return true;
}

bool findPotentialHorizonalLeft(int n, int row, int col, char color){
    int i;
    for (i = 0; i < col + 1; i++) {
        if(i<6){
            if(board [row] [col-i] != 'U' && board [row] [col-i] != color)
                return false;
        }
    }
    if(i<6)
        return false;
    return true;
}

bool findPotentialVerticalDown(int n, int row, int col, char color){
    int i;
    for (i = 0; i < n - row; i++) {
        if(i<6){
            if(board [row+1] [col] != 'U' && board [row+i] [col] != color)
                return false;
        }
    }
    if(i<6)
        return false;
    return true;
}

bool findPotentialVerticalUp(int n, int row, int col, char color){
    int i;
    for (i = 0; i < row + 1; i++) {
        if(i<6){
            if(board [row-1] [col] != 'U' && board [row-i] [col] != color)
                return false;
        }
    }
    if(i<6)
        return false;
    return true;
}

bool findPotentialLeftDiagonalDown(int n, int row, int col, char color){
    int i;
    for (i=0; i< fmin(n - row, n - col); i++){
        if(i<6){
            if(board [row-1] [col] != 'U' && board [row+i] [col+i] != color)
                    return false;
        }
    }
    if(i<6)
        return false;
    return true;
}

bool findPotentialLeftDiagonalUp(int n, int row, int col, char color){
    int i;
    for(i= 0; i< fmin(row + 1, col + 1); i++){
        if(i<6){
            if(board [row-1] [col] != 'U' && board [row-i] [col-i] != color)
                    return false;
        }
    }
    if(i<6)
        return false;
    return true;
}

bool findPotentialRighDiagonalDown(int n, int row, int col, char color){
    int i;
    for(i= 0; i< fmin(n - row, col + 1); i++){
        if(i<6){
            if(board [row-1] [col] != 'U' && board [row+i] [col-i] != color)
                    return false;
        }
    }
    if(i<6)
        return false;
    return true;
}

bool findPotentialRightDiagonalUp(int n, int row, int col, char color){
    int i;
    for(i= 0; i< fmin(n - col, row + 1); i++){
        if(i<6){
            if(board [row-1] [col] != 'U' && board [row-i] [col+i] != color)
                    return false;
        }
    }
    if(i<6)
        return false;
    return true;
}

int getScore(int n, int row, int col){
    int score= longestSequenceBlack(n, row, col) + longestSequenceWhite (n, row, col);
    return score;
}