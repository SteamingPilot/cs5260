#include <math.h>
#include <stdlib.h>
#include <stdio.h>

void printBoard(int **board, int n){
    printf("\n");
    for(int i=0; i<n; i++){
        for(int j=0; j<n; j++){
            if(board[i][j] == 0) printf("_ ");
            else printf("%d ", board[i][j]);
        }
        printf("\n");
    }
}
int getNumberOfEmptyCells(int **board, int n){
    int count = 0;
    for(int i=0; i<n; i++){
        for(int j=0; j<n; j++){
            if(board[i][j] == 0) count++;
        }
    }
    return count;
}

// Returns 1 if no empty cell found which means board is solved
int findNextEmpty(int **board, int n, int *res_row, int *res_col){ 
    for(int i=0; i<n; i++){
        for(int j=0; j<n; j++){
            if (board[i][j] == 0) {
                *res_row = i;
                *res_col = j;
                return 0;
            }
        }
    }
    return 1;
}

int isValPossible(int **board, int n, int row, int col, int val){

    // Checking Column
    for(int i=0; i<n; i++){
        if(board[row][i] == val){
            return 0;
        }
    }

    // Checking Row
    for(int i=0; i<n; i++){
        if(board[i][col] == val) return 0;
    }


    int sqrt_n = sqrt(n);

    // Checking box/sub-grid
    int row_start = row - (row%sqrt_n);
    int col_start = col - (col%sqrt_n);
    for(int i=0; i<sqrt_n; i++){
        for(int j=0; j<sqrt_n; j++){
            if(board[row_start+i][col_start+j] == val) return 0;
        }
    }

    return 1;
    
}

int isBoardFilled(int **board, int n){
    for(int i=0; i<n; i++){
        for(int j=0; j<n; j++){
            if(board[i][j] == 0) return 0;
        }
    }
    return 1;
}

void createBoardCopy(int **board_main, int n, int **board_copy){
    for(int i=0; i<n; i++){
        for(int j=0; j<n; j++){
            board_copy[i][j] = board_main[i][j];
        }
    }
}