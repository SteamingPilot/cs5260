#include<stdio.h>
#include<string.h>
#include<math.h>
#include<stdlib.h>
#include<omp.h>
#include<time.h>

#include "queue.h"
#include "sudoku.h"

#define THRESHOLD 10

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


void bootstrap(Queue *q, int *solved){

    if(*solved) return;
    if(isQueueEmpty(q)) return;

    queue_node_t *head_node;
    #pragma omp critical
    head_node = getQueueHead(q);

    if(head_node == NULL) return;
    int **board = head_node->board;
    int n = head_node->board_size;

    if(isBoardFilled(board, n)) return;

    int emp_row, emp_col;
    findNextEmpty(board, n, &emp_row, &emp_col);

    for(int i=1; i<=n; i++){
        if(isValPossible(board, n, emp_row, emp_col, i)){
            int **board_copy;
            board_copy = (int **) malloc(sizeof(int *)*n);
            for(int i=0; i<n; i++){
                board_copy[i] = (int *) malloc(sizeof(int)*n);
            }
            createBoardCopy(board, n, board_copy);
            board_copy[emp_row][emp_col] = i;
            
            #pragma omp critical
            enqueue(q, board_copy, n);
        }
    }

    #pragma omp critical
    dequeue(q);
}

void solveHelper(Queue *q, int *solved, int **board_original){
    if(*solved) return;

    queue_node_t *head_node = getQueueHead(q);
    int **board = head_node->board;
    int n = head_node->board_size;

    if(getNumberOfEmptyCells(board, n) <= THRESHOLD){
        if(solveSerial(board, n)) {
            createBoardCopy(board, n, board_original);
            *solved = 1;
        }
        return;
    }
    bootstrap(q, solved);
}

void solveParallel(Queue *q, int **board, int board_size, int *solved, int **board_original, int thread_count){

    // Push Current Board to the Queeu
    enqueue(q, board, board_size);


    int num_bootstraps = thread_count;
    #pragma omp parallel for schedule(static) default(none) shared(q, num_bootstraps,solved)
    for(int i=0; i<num_bootstraps; ++i){
        bootstrap(q, solved);
    }

    int boards_in_queue = q->size;
    #pragma omp parallel shared(q, boards_in_queue, solved, board_original)
    while(q->size > 0){
    // #pragma omp parallel for schedule(static) default(none) shared(q, boards_in_queue, solved, board_original)
    // for(int i=0; i<boards_in_queue; ++i){
        if(*solved) {
                #pragma omp critical
                q->size = 0;
                continue;
        }
        solveHelper(q, solved, board_original);
        #pragma omp critical
        {
            boards_in_queue = q->size;
        }
    }
}


// Usage: parallel_sudoku_solver <N> <MAX_THREADS>
int main(int argc, char*argv[]){
    double start_time, end_time;
    double serial_start_time, serial_end_time;

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
        board_serial[i] = (int *) malloc(sizeof(int)*n);
    }
    createBoardCopy(board, n, board_serial);

    // Serial Solver
    serial_start_time = omp_get_wtime();
    int has_solved = solveSerial(board_serial, n);
    serial_end_time = omp_get_wtime();

    if(has_solved) {
        printBoard(board_serial, n);
    }
    else printf("Board unsolvalble\n");
    printf("Time taken by Serial Solver: %lf\n",  serial_end_time-serial_start_time); 

    // Parlallel Solver
    // Create a copy of the board to be used by the parallel solver
    int **board_parallel;
    board_parallel = (int **) malloc(sizeof(int *)*n);
    for(int i=0; i<n; i++){
        board_parallel[i] = (int *) malloc(sizeof(int)*n);
    }

    createBoardCopy(board, n, board_parallel);
    // Initialize the Shared Queue
    Queue *q = create_queue();
    unsigned max_threads = strtol(argv[2], NULL, 10);
    int solved = 0;
    omp_set_num_threads(max_threads); 


    // Parallel Solver
    start_time = omp_get_wtime();
    #pragma omp parallel num_threads(max_threads) shared(q, solved, board_parallel, board, max_threads)
    solveParallel(q, board_parallel, n, &solved, board, max_threads);
    end_time = omp_get_wtime();

    if(solved) {printBoard(board, n);}
    else printf("Board unsolvable\n");
    printf("Time taken by Parallel Solver: %lf\n", end_time-start_time);

    return 0;
}