#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<omp.h>
#include<time.h>
#include<math.h>

#include "queue.h"
#include "sudoku.h"


#define max(x, y) ((x) > (y) ? (x) : (y))




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
            
            enqueue(q, board_copy, n);
        }
    }

    dequeue(q);
}

void solveParallel(Queue *q, int **board, int board_size, int *solved, int **board_original, int thread_count){

    // Push Current Board to the Queeu
    enqueue(q, board, board_size);


    int forty_percent_cells = (int) (0.4*board_size*board_size);
    int num_bootstraps = forty_percent_cells;
    for(int i=0; i<num_bootstraps; ++i){
        bootstrap(q, solved);
    }

    int boards_in_queue = q->size;
    int ***solved_boards;
    solved_boards = (int ***) malloc(sizeof(int **)*boards_in_queue);
    for(int i=0; i<boards_in_queue; ++i){
        solved_boards[i] = (int **) malloc(sizeof(int *)*boards_in_queue);
        for(int j=0; j<thread_count; ++j){
            solved_boards[i][j] = (int *) malloc(sizeof(int)*boards_in_queue);
        }
    }
    // #pragma omp parallel shared(q, boards_in_queue, solved, board_original)
    // while(q->size > 0){
    #pragma omp parallel num_threads (thread_count)
    {
        #pragma omp parallel for schedule(static) default(none) shared(q, boards_in_queue, solved, board_original, solved_boards)
        for(int i=0; i<boards_in_queue; ++i){
            if(*solved) {
                continue;
            }
            queue_node_t *local_board_node;
            #pragma omp critical
            local_board_node = dequeue(q);

            if(local_board_node == NULL) continue;
            int **local_board = local_board_node->board;
            int local_board_size = local_board_node->board_size;

            if(solveSerial(local_board, local_board_size)){
                solved_boards[i] = local_board;
                #pragma omp critical
                *solved = 1;
                createBoardCopy(local_board, local_board_size, board_original);
            }
        }
    }
}

// Usage: parallel_sudoku_solver <N> <MAX_THREADS>
int main(int argc, char*argv[]){
    double start_time, end_time;

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


    // Parallel Solver
    start_time = omp_get_wtime();
    omp_set_num_threads(max_threads);
    solveParallel(q, board_parallel, n, &solved, board, max_threads);
    end_time = omp_get_wtime();

    if(solved) {printBoard(board, n);}
    else printf("Board unsolvable\n");
    printf("Time taken by Parallel Solver: %lf\n", end_time-start_time);

    return 0;
}