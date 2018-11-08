//Author: Nate Glod
/*Date: November 11th 2018*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <string.h>
#include <getopt.h>

struct qNode{
        int key;
        struct qNode *link;
};

struct queue{
        struct qNode *front, *back;
};

struct qNode * deQueue(struct queue *q){
    if (q->front == NULL){
       return NULL;
    }
    struct qNode *temp = q->front;
    q->front = q->front->link;
    if (q->front == NULL){
        q->back = NULL;
    }
    return temp;
