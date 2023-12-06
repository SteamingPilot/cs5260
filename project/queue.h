#include <stdlib.h>

void freeBoard(int **board, int n){
    for(int i = 0; i < n; i++){
        free(board[i]);
    }
    free(board);
}

typedef struct queue_node{
    int **board;
    int board_size;
    struct queue_node *next;
} queue_node_t;

typedef struct {
    queue_node_t *head;
    queue_node_t *tail;
    int size;
} Queue;


Queue *create_queue(){
    Queue *queue = (Queue *)  malloc(sizeof(Queue));
    queue->head = NULL;
    queue->tail = NULL;
    queue->size = 0;
    return queue;
}

void enqueue(Queue *queue, int **board, int board_size){
    queue_node_t *node = (queue_node_t *) malloc(sizeof(queue_node_t));
    node->board = board;
    node->board_size = board_size;
    node->next = NULL;
    if(queue->size == 0){
        queue->head = node;
        queue->tail = node;
    } else {
        queue->tail->next = node;
        queue->tail = node;
    }
    queue->size++;
}

queue_node_t *dequeue(Queue *queue){
    if(queue->head == NULL){
        return NULL;
    }
    queue_node_t *node = queue->head;
    queue->head = queue->head->next;
    queue->size--;

    return node;

    // freeBoard(node->board, node->board_size);
}

int isQueueEmpty(Queue *queue){
    return queue->size == 0;
}

queue_node_t * getQueueHead(Queue *queue){
    if(queue->size == 0){
        return NULL;
    }
    return queue->head;
}

void freeQueue(Queue *queue){
    while(!isQueueEmpty(queue)){
        free(dequeue(queue));
    }
    free(queue);
}
