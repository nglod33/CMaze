//Author: Nate Glod
//Class: Mechanics of Programming
//Professor: Adam Purtee
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
}

int deQueueHelper(struct queue *q){
        struct qNode *temp = deQueue(q);
        if(temp == NULL){
                return -1;
        }
        int value = temp->key;
        return value;
}

void freeQueue(struct queue *q){
	while(deQueueHelper(q) != -1){}
	free(q->back);
	free(q->front);
	free(q);
}

struct qNode * newNode(int i){
	struct qNode *temp = (struct qNode*)malloc(sizeof(struct qNode)); 
	temp->key = i; 
	temp->link = NULL; 
	return temp;  
}

struct queue * createQueue(){
    struct queue *q = (struct queue*)malloc(sizeof(struct queue));
    q->front = q->back = NULL;
    return q;
}

void enQueue(struct queue *q, int k){
    struct qNode *temp = newNode(k);
    if (q->back == NULL){
       q->front = q->back = temp;
       return;
    }
    	q->back->link = temp;
    	q->back = temp;
}

//Uses addresses as nodes in the graph to implement a
//breadth-first search. Outputs an array of ints representing
//the position of the nodes in the shortest possible route
//to the exit. Returns a pointer to '\0' if there is no route
//ASSUMPTIONS: the input file has at least one row, and one column,and each row has
//the same number of columns, and there is no newline at the end of the last row
void solve(char *maze, int *row, int *col){
	
	int end = (((*row) * (*col)) - 2);
	while(maze[end] == '\n'){
		--end;
	}
        //any visited node will be an x
        //If the starting node is blocked return false
        if((maze[0] == '1') || (maze[end] == '1')){
                return;
        }

        //bfs finds the path, (if there is one) and then works backwards to find the path using
        //the wasd system
        int currCheck;
        struct queue *q = createQueue();
        enQueue(q, 0);
        while((((currCheck = deQueueHelper(q)) != -1)) && (maze[end] == '0')){
                //check above
                if((currCheck > (*col)) && (maze[currCheck - (*col)] == '0')){
                        enQueue(q, currCheck - (*col));
                        maze[currCheck - (*col)] = 's';
                }
                //check right
                if(maze[currCheck + 1] == '0'){
                        enQueue(q, currCheck + 1);
                        maze[currCheck + 1] = 'a';
                }
                //check left
                if((currCheck > 0) && (maze[currCheck - 1] == '0')){
                        enQueue(q, currCheck - 1);
                        maze[currCheck - 1] = 'd';
                }
                //check below
                if(((currCheck + (*col)) <= end) && (maze[currCheck + (*col)] == '0')){
                        enQueue(q, currCheck + (*col));
                        maze[currCheck + (*col)] = 'w';
                }
        }
	deQueueHelper(q);
	free(q->front);
	free(q->back);
	free(q);
}

//Prints out a helpful message
void helpfulMessage(FILE *out){
	fprintf(out, "This is a helpful message\n");
}

//Prints the given maze in an aesthetically pleasing fashion
void prettyPrint( char *maze, FILE *out, int *row, int *col, int print){
	
	//Finds the number or rows and columns, used in other functions as well
        for(; (maze[*col]) != '\n'; ++(*col)){}
        ++(*col);
        for(int i = 0; (maze[i]) != '\0'; ++i){
                if(maze[i] == '\n'){
                        ++(*row);
                }
        }
	if(print){
		//Prints the full maze
		fprintf(out,"|");
		for(int i = 0; i < ((*col) - 1); ++i){
			fprintf(out, "--");
		}
		fprintf(out,"-|\n ");
		for(int i = 0; maze[i] != '\0'; ++i){
			if(maze[i] == '1'){
				fprintf(out, " #");
			}
			else if(maze[i] == '0'){
				fprintf(out, " .");
			}
			else if(maze[i] == '\n'){
				if(maze[i + 1] == '\0'){
					fprintf(out, " \n|");
				}
				else{
					fprintf(out, " |\n|");
				}
			}
		}
	        for(int i = 0; i < ((*col) - 1); ++i){
	                fprintf(out, "--");
	        }
		fprintf(out,"-|\n");
	}
		
}

//Prints the optimal path
void optimalPrint(char *maze, FILE *out, int *row, int *col, int *steps, int print, int solPrint){
	//This function calculates the number of steps in the solution, so it runs always, but does not
	//print always
	unsigned int end = ((*row)*(*col)) - 2;
	while(maze[end] == '\n'){
		end = end - 1;
	}
	if((maze[end] == '1') || (maze[end] == '0')){
		*row = 0;
		*col = 0;
		if(solPrint){fprintf(out, "No solution.\n");}
		prettyPrint(maze, out, row, col, print);
	}
	else{
	//Puts the correct path into the maze
	for(; end != 0; ++(*steps)){
		if(maze[end] == 'w'){
			maze[end] = '+';
			end = (end - (*col));
		}
		else if(maze[end] == 'a'){
			if(maze[end - 1] == '\n'){
				maze[end] = '+';
				end = (end - 2);
			}
			else{
				maze[end] = '+';
				end = end - 1;
			}
		}
		else if(maze[end] == 's'){
			maze[end] = '+';
			end = end + (*col);
		}
		else{
			if(maze[end + 1] == '\n'){
				maze[end] = '+';
				end = (end + 2);
			}
			else{
				maze[end] = '+';
				++end;
			}
		}
	}
	maze[end] = '+';

	//prints the number of steps in the solution
	if(solPrint){
		fprintf(out, "Solution in %i steps.\n", (*steps) + 1);
	}
	
	//transforms the maze into a more readable form
	if(print){
		fprintf(out,"|");
                for(int i = 0; i < ((*col) - 1); ++i){
                        fprintf(out, "--");
                }
                fprintf(out,"-|\n ");
		for(int i = 0; maze[i] != '\0'; ++i){
			if(maze[i] != '\n'){
				if(maze[i] != '+'){
					if(maze[i] != '1'){
						fprintf(out," .");
					}
					else{
						fprintf(out, " #");
					}
				}
				else{
					fprintf(out, " +");
				}
			}
			else{
				if(maze[i + 1] == '\0'){
                                	fprintf(out, " \n|");
                        	}
				else{
					fprintf(out, " |\n|");
				}
			}
		}	
        	for(int i = 0; i < ((*col) - 1); ++i){
			fprintf(out, "--");
        	}
        	fprintf(out,"-|\n\n");
	}	
	}
}

//Reads all the flags and then goes through and solves everythign
int main(int argc, char **argv){

//flag vars
int h = 0, d = 0, s = 0, p = 0;
char *i = NULL, *o = NULL;

//var for loop
int c;

//Checks to see what compiler flags the user put up
	while ((c = getopt (argc, argv, "hdspi:o:")) != -1){
		switch(c){
		
			//prints help message
			case 'h':
			h = 1;
			break;
		
			//pretty print statement
			case 'd':
			d = 1;
			break;
		
			//print shortest solution
			case 's':
			s = 1;
			break;

			//print optimal path
			case 'p':
			p = 1;
			break;
		
			//Set an input file
			case 'i':
			i = optarg;
			break;

			//set up an output file;
			case 'o':
			o = optarg;
			break;

			//Ignores unrecognized symbols;
			default:
			printf("Unrecognized flag: %c", c);
			break;
		}
	}

	//Prints help message and yeets outta here
	if(h){	
		helpfulMessage(stdout);
		exit(0);
	}

	FILE *mazeFile = NULL, *outFile = NULL;
	
	//Processing input
	if(i != NULL){
		mazeFile = fopen(i, "r");
	}
	else{
		mazeFile = stdin;
	}

	//processing output file
	if(o != NULL){
		outFile = fopen(o, "w");
	}
	else{
		outFile = stdout;
	}

	//Finding the size of the mazefile and creating a string for it
	unsigned int mazeSize = 0;
	char ch;
	char *maze = malloc(250005001);
	for(mazeSize = 0; (ch = getc(mazeFile)) != -1; ++mazeSize){
		if(ch != ' '){
			maze[mazeSize] = ((char)ch);
		}
		else{
			mazeSize = (mazeSize - 1);
		}
	}
	maze = realloc(maze, mazeSize + 2);
	maze[mazeSize] = '\0';
	int *row = malloc(4);
	*row = 0;
	int *col = malloc(4);
	*col = 0;
	int *steps = malloc(4);
	*steps = 0;

        //Pretty-print the core maze
        if(d){prettyPrint(maze, outFile, row, col, 1);}
	else{prettyPrint(maze, outFile, row, col, 0);}

	//Implements a bfs to solve the maze
	solve(maze, row, col);
	
	//prints helpful message to stdout
	if(h){helpfulMessage(outFile);}

	//Prints an optimal path
	if(p){optimalPrint(maze, outFile, row, col, steps, 1, s);}
	else{optimalPrint(maze, outFile, row, col, steps, 0, s);}

	//tying up loose ends
	fclose(mazeFile);
	free(maze);
	free(row);
	free(col);
	free(steps);
	return(0);
}
