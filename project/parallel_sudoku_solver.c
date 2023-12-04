#include<stdio.h>
#include<string.h>
#include<math.h>
#include<stdlib.h>
#include<omp.h>

void printBoard(int **board, int n){
    for(int i=0; i<n; i++){
        for(int j=0; j<n; j++){
            if(board[i][j] == 0) printf("_ ");
            else printf("%d ", board[i][j]);
        }
        printf("\n");
    }
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

int solveSerial(int **board, int n){
    printf("\n\n");
    printBoard(board, n);

    int emp_row, emp_col, is_solved;
    is_solved = findNextEmpty(board, n, &emp_row, &emp_col);

    if(is_solved) return 1;

    for(int i=1; i<=n; i++){
        if(isValPossible(board, n, emp_row, emp_col, i)){
            board[emp_row][emp_col] = i;
            if(solveSerial(board, n)) return 1;
            board[emp_row][emp_col] = 0;
        }
    } 
    return 0;
}



// Usage: parallel_sudoku_solver <N> <MAX_THREADS>
int main(int argc, char*argv[]){

    long n = strtol(argv[1], NULL, 10);
    
    // Board Allocation
    int **board;
    board = (int **) malloc(sizeof(int *)*n);
    for(int i=0; i<n; i++){
        board[i] = (int *) malloc(sizeof(int)*n);
    }

    // Board Input
    for(int i=0; i<n; i++){
        for(int j=0; j<n; j++) {
            scanf("%d", &board[i][j]);
        }
    }

    // Copy board to board_serial for the Serial Algorithm
    int **board_serial;
    board_serial = (int **) malloc(sizeof(int *)*n);
    for(int i=0; i<n; i++){
        board[i] = (int *) malloc(sizeof(int)*n);
    }
    // Serial Solver
    if(solveSerial(board, n)) printBoard(board, n);
    else printf("Board unsolvalble\n");



    // Parlallel Solver

    return 0;

}