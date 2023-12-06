#include<stdio.h>
#include<stdlib.h>
#include<time.h>

#include "queue.h"
#include "sudoku.h"

int solveSerial(int **board, int n){
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
    time_t serial_start_time, serial_end_time;

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


    // Serial Solver
    serial_start_time = clock();
    int has_solved = solveSerial(board, n);
    serial_end_time = clock();

    if(has_solved) {
        printBoard(board, n);
    }
    else printf("Board unsolvalble\n");
    printf("Time taken by Serial Solver: %lf\n", (double)(serial_end_time-serial_start_time)/CLOCKS_PER_SEC); 

    return 0;
}