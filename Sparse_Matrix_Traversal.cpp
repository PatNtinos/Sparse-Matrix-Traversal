#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>


#define MAX_ROWS 30


struct StackNode {
    int row, col;
    struct StackNode *next;
};

void findCommonAncestorAndPrint(struct StackNode *stack, int parent[]);

// Stack functions
struct StackNode* createStack();
void push(struct StackNode **stack, int row, int col);
void pop(struct StackNode **stack);
bool isEmpty(struct StackNode *stack);
int isEdgeVisited(struct StackNode *stack, int x, int y);

// A structure for sparse matrix nodes
typedef struct smNode *pp;



struct smNode {
    int row, col, aux;  // Row and column index
    double value;  // Non-zero value
    bool visited;  // Visited flags for BFS	
    pp next;       // Pointer to the next element in the row or column
    pp prev;       // Pointer to the previous element in the row or column
    pp up;		   // Pointer to the previous element in the same column
    pp down;	   // Pointer to the next element in the same column
};

// A structure for queue node
struct QueueNode {
    pp matrixNode;
    struct QueueNode *next;
};

// A structure for the queue itself
struct Queue {
    struct QueueNode *front;
    struct QueueNode *rear;
};


// Queue functions
struct Queue* createQueue();
void enqueue(struct Queue *q, pp node, pp h);
pp dequeue(struct Queue *q);

// Matrix Functions
void insertElement(pp *h, int row, int col, double value);
void printMatrix(pp h);
void deleteElement(pp *h, int row, int col);
void BFS(pp h, int startrow, struct StackNode **stack, int parent[]);
void rowvisited(pp h, pp temp);

int main(void) {
    pp h = NULL;  // Head pointer 
    
    int row, col,startrow;
    double value;
    char answer = '_';

	struct StackNode *nonTreeEdges = NULL;   
    int parent[MAX_ROWS];  
    
    
    while(answer != 'Q' && answer != 'q') {
        printf("\n\n--------------------------------------------------------------\n");
        printf("Previous answer was: %c\nGive a new choice: (I/P/D/Q/B/C): ", answer);

        fflush(stdin);
        answer = getchar();
        getchar();

        printf("The new answer is: %c", answer);
        printf("\n--------------------------------------------------------------\n");

        if(answer != 'I' && answer != 'i' && answer != 'P' && answer != 'p' && answer != 'D' && answer != 'd' && answer != 'Q' &&
           answer != 'q' && answer != 'B' && answer != 'b' && answer != 'C' && answer != 'c') {
            printf("\n\nInvalid input, please try again.\n");
        }

        if(answer == 'I' || answer == 'i') {
            printf("\n\nInsert a value in the matrix");


            do {
       		printf("\nThe maximum row must be 30");
      		printf("\nProvide the row: ");
       		scanf("%d", &row);
       		getchar();
            }while (row < 0 || row > 30);
            


            do {
      	    printf("\nThe maximum column must be 30");
       	    printf("\nProvide the column: ");
     		scanf("%d", &col);
     		getchar();
  		    }while (col < 0 || col > 30);
            
			printf("\nProvide the value: ");
            scanf("%lf", &value);
            getchar();

            insertElement(&h, row, col, value);
            insertElement(&h, col, row, value);
            printf("\nValue inserted at (%d,%d)!\n",row,col);
        }

        if(answer == 'P' || answer == 'p') {
            printf("\n\nPrint the Sparse Matrix\n");
            printMatrix(h);
        }

        if(answer == 'D' || answer == 'd') {
            printf("\n\nDelete a value from the matrix");
            printf("\nProvide the row: ");
            scanf("%d", &row);
            getchar();

            printf("Provide the column: ");
            scanf("%d", &col);
            getchar();

            deleteElement(&h, row, col);
  			deleteElement(&h, col, row);      
		}
		    if(answer == 'B' || answer == 'b') {
            
			printf("\n\n Select your starting node for BFS");
			printf("\nProvide the node: ");
            scanf("%d", &startrow);
            getchar();

            struct StackNode *nonTreeEdges = createStack();
            int parent[MAX_ROWS];
            BFS(h, startrow, &nonTreeEdges, parent); 
            
            // Print the non-tree edges
    		/*printf("\nEdges not in the spanning tree:\n");
  			while (!isEmpty(nonTreeEdges)) {
      		printf("\nEdge (%d, %d)\n", nonTreeEdges->row, nonTreeEdges->col);
        	pop(&nonTreeEdges);
        	
        	
    }*/
    		// Process non-tree edges to find common ancestor and print paths
    		findCommonAncestorAndPrint(nonTreeEdges, parent);
		}
		
}
    

    printf("\n\nThe program has halted.");

    return 0;
}

	// Insert a value in the matrix at (row, col)
	void insertElement(pp *h, int row, int col, double value) {
    if (value == 0) return;  // Non zero values 

    pp newNode = (pp)malloc(sizeof(struct smNode));
    newNode->row = row;
    newNode->col = col;
    newNode->value = value;
    newNode->next = NULL;
    newNode->prev = NULL;
    newNode->up = NULL;
    newNode->down = NULL;

    pp tmp = *h;
    
    // Empty Matrix, First Node
    if (*h == NULL) {
        *h = newNode;
        return;
    }

    // Insert in sorted order by row and then by column
    while (tmp->next != NULL && (tmp->row < row || (tmp->row == row && tmp->col < col))) {
        tmp = tmp->next;
    }

    if (tmp->row == row && tmp->col == col) {
        // Same value
        tmp->value = value;
        free(newNode);
    } else if (tmp->row > row || (tmp->row == row && tmp->col > col)) {
        // Insert the new node before tmp
        newNode->next = tmp;
        newNode->prev = tmp->prev;
        if (tmp->prev != NULL) {
            tmp->prev->next = newNode;
        } else {
            *h = newNode;  // Update head 
        }
        tmp->prev = newNode;
    } else {
        // Insert at the end of the list
        tmp->next = newNode;
        newNode->prev = tmp;
    }
    
    
    pp vertical = *h;

	while (vertical != NULL && vertical->col != col) {
        vertical = vertical->next;  // Traverse horizontally to find the column
    }

    if (vertical == NULL) {
        // No nodes exist in this column yet, so no vertical linkage needed
        return;
    }

    // Traverse vertically within the column to find the correct row position
    pp prevVert = NULL;
    while (vertical != NULL && vertical->row < row) {
        prevVert = vertical;
        vertical = vertical->down;
    }

    if (vertical != NULL && vertical->row == row) {
    } else {
        // Insert the new node vertically between `prevVert` and `vertical`

        newNode->down = vertical;
        newNode->up = prevVert;

        if (prevVert != NULL) {
            prevVert->down = newNode;
        }

        if (vertical != NULL) {
            vertical->up = newNode;
        }
    }
}

// Print the non-zero elements of the sparse matrix
void printMatrix(pp h) {
   
    
    if (h == NULL) {
        printf("\nThe matrix is empty.\n");
        return;
    }
    pp tmp = h;
    /* IF we want to type only the existent graph intersections
    while (tmp != NULL) {
        printf("Value at (%d, %d): %.2f\n", tmp->row, tmp->col, tmp->value);
        tmp = tmp->next;
    } */
    
    //If we want to print all the sparse matrix
    
    double matrix[31][31]={0};
    while (tmp !=NULL)
    {
    	matrix[tmp->row][tmp->col]=tmp->value;
    	tmp=tmp->next;
	}
	
	//Printing
	for(int i=1;i<31;i++)
	{
		for(int j=1;j<31;j++)
		{
			printf("%5.1f", matrix[i][j]);
		}
		printf("\n");
	}
}

	// Delete the element at (row, col)
	void deleteElement(pp *h, int row, int col) {
    pp tmp = *h;

    // Find the node to delete
    while (tmp != NULL && (tmp->row != row || tmp->col != col)) {
        tmp = tmp->next;
    }

    if (tmp == NULL) {
        printf("\nElement not found at (%d, %d)\n", row, col);
        return;
    }

    // Unlink the node from the list
    if (tmp->prev != NULL) {
        tmp->prev->next = tmp->next;
    } else {
        *h = tmp->next;  // Update head if deleting the first element
    }

    if (tmp->next != NULL) {
        tmp->next->prev = tmp->prev;
    }
    //Unlink up down
	if (tmp->up!= NULL) {
        tmp->up->down = tmp->down;
    } 

    if (tmp->down != NULL) {
        tmp->down->up = tmp->up;
    }

    free(tmp);
    printf("\nElement at (%d, %d) deleted.\n", row, col);
}


	// Queue creation
	struct Queue* createQueue() {
    struct Queue *q = (struct Queue*)malloc(sizeof(struct Queue));
    q->front = NULL;
	q->rear = NULL;
    return q;
}
// Enqueue Operation
void enqueue(struct Queue *q, pp node, pp h) {
    struct QueueNode *temp = (struct QueueNode*)malloc(sizeof(struct QueueNode));
    temp->matrixNode = node;   
    temp->next = NULL;
    
    // If queue is empty, initialize front and rear
    if (q->rear == NULL) {
        q->front = q->rear = temp;
    } else {
        // Attach new node at the end of the queue and update rear
        q->rear->next = temp;
        q->rear = temp;
    }

    printf("\nThe (%d,%d) node got enqueued\n", node->row, node->col);

    // Call rowvisited after enqueueing
    rowvisited(h, node);
}

	// Dequeue operation
	pp dequeue(struct Queue *q) {
    if (q->front == NULL)
        return NULL;
    struct QueueNode *temp = q->front;
    pp node = temp->matrixNode;
    q->front = q->front->next;
    if (q->front == NULL)
        q->rear = NULL;
    free(temp);
    return node;
}


	void BFS(pp h,int startrow, struct StackNode **stack, int parent[]) 
	{
	
	if (h == NULL) 
	{
        printf("\nThe matrix is empty.\n");
        return;
    }
    
    // Initialize the parent array
    for (int i = 0; i < MAX_ROWS; i++) {
        parent[i] = -1;  // -1 indicates no parent assigned
    }
    
    // Create a queue for BFS
    struct Queue *q = createQueue();

    // Find the starting node
    pp rowhead = h;
    while (rowhead != NULL && rowhead->row != startrow) {
        rowhead = rowhead->next;
    
	}
	if (rowhead == NULL) {
        printf("\nStarting node not found\n");
        return;
    }
    
    // Initialize a list to track traversed edges
    struct StackNode *traversedEdges = NULL;
    
    // Enqueue the starting node
    pp temp = rowhead;
    
	parent[temp->row]=-1;
	
	while((temp != NULL) && (temp->row == startrow))
    {
    	if (!temp->visited)
    	{
    		temp->visited=true;
    		enqueue(q, temp, h);
    		
			// Record edge in traversedEdges
            push(&traversedEdges, temp->row, temp->col);
            
            // Set the parent for the starting node
        	parent[temp->col] = temp->row;  // No parent for the root
		}
	    temp=temp->next;
	}
    
    // BFS loop
    while (q->front != NULL) {
    	int aux, rowd ;
        pp node = dequeue(q);
		aux=node->col;
		rowd=node->row;
		
        // Process the node (print its value and position)
        printf("\nVisited node at (%d, %d): %.2f\n", node->row, node->col, node->value);
		
        // Enqueue adjacent nodes
        while (node->next != NULL &&  node->next->row==node->row)
		{
		    if(!node->next->visited)
			{
			node=node->next;
			node->visited=true;
			enqueue(q, node, h);
			
	        // Record edge in traversedEdges
            push(&traversedEdges, node->row, node->col);
            
            // Update the parent array
            parent[node->col] = node->row;
			}

			node=node->next;
		}
		
        while (node->prev != NULL &&  node->prev->row==node->row)
		{
			if(!node->prev->visited)
			{
			node=node->prev;
			node->visited=true;
			enqueue(q, node, h );
			
			// Record edge in traversedEdges
            push(&traversedEdges, node->row, node->col);
            
            // Update the parent array
            parent[node->col] = node->row;
			}

			node=node->prev;
		} 
		
		node = h;
		while (node != NULL) {
    		if (node->row == aux && !node->visited) {
     	    node->visited = true;
      	  	enqueue(q, node, h);
      	  	
      	  	// Record edge in traversedEdges
            push(&traversedEdges, node->row, node->col);
            
            // Update the parent array
        	parent[node->col] = aux;
      	  	
			break;  // Exit the loop once the required node is found and enqueued
  		    }
   		    node = node->next;
		}
	
    }
    
    // Find and push edges not in the BFS tree to the main stack
    temp = h;
    while (temp != NULL) {
        bool isEdgeTraversed = false;
        struct StackNode *edgeNode = traversedEdges;

        // Check if edge (temp->row, temp->col) is in traversedEdges
        while (edgeNode != NULL) {
            if ((edgeNode->row == temp->row && edgeNode->col == temp->col) || (edgeNode->row == temp->col && edgeNode->col == temp->row)) {
                isEdgeTraversed = true;
                break;
            }
            edgeNode = edgeNode->next;
        }

        if (!isEdgeTraversed) {
        	// Check if the edge is already marked as visited in the main stack
        	if (!isEdgeVisited(*stack, temp->row, temp->col)) 
			{
            // Push to the main stack
            push(stack, temp->row, temp->col);
        	}
        }

        temp = temp->next;
    }
    
     // Reset visited status for all nodes after BFS completes
    temp = h;
    while (temp != NULL) {
        temp->visited = false;
        temp = temp->next;
    }
    
    printf("\nBFS traversal complete.\n");
    printf("\nNon-Tree edge pushed to the stack.\n");
    
    // Print the parent array for debugging
    printf("\nParent array:\n");
    for (int i = 1; i < MAX_ROWS; i++) {
        if (parent[i] != -1) {
            printf("Node %d -> Parent %d\n", i, parent[i]);
}
}
}

// Rowvisited function, to delete the columns that were visited
void rowvisited(pp h, pp temp)
{
	int rowd;
	int cold;
	cold=temp->col;
	rowd=temp->row;
	temp=h;
	while( temp != NULL )
	{
		if (temp->col==rowd || temp->col==cold)
		{
			temp->visited=true;
		}
		temp=temp->next;

	}
}

struct StackNode* createStack() {
    return NULL;  // An empty stack
}

void push(struct StackNode **stack, int row, int col) {
    struct StackNode *newNode = (struct StackNode*)malloc(sizeof(struct StackNode));
    newNode->row = row;
    newNode->col = col;
    newNode->next = *stack;  // Link the new node to the top of the stack
    *stack = newNode;  // Update the top of the stack
}

void pop(struct StackNode **stack) {
    if (*stack == NULL) return;  // Stack is empty
    struct StackNode *temp = *stack;
    *stack = (*stack)->next;  // Move the top pointer to the next node
    free(temp);  // Free the memory of the removed node
}

// Function to check if an edge (x, y) is already in the stack
int isEdgeVisited(struct StackNode *stack, int x, int y) {
    struct StackNode *current = stack;
    while (current != NULL) {
        // Check if this edge (x, y) or (y, x) is already in the stack
        if ((current->row == x && current->col == y) || (current->row == y && current->col == x)) {
            return 1;  // Edge already exists in the stack
        }
        current = current->next;
    }
    return 0;  // Edge not found in the stack
}

// Check if the stack is empty
bool isEmpty(struct StackNode *stack) {
    return stack == NULL;
}

void findCommonAncestorAndPrint(struct StackNode *stack, int parent[]) {
	
	
    while (!isEmpty(stack)) {
        // Pop a non-tree edge
        int u = stack->row;
        int v = stack->col;
        pop(&stack);

        printf("\nProcessing non-tree edge: (%d, %d)\n", u, v);

        // Find the path to the root for u
        int pathU[MAX_ROWS], pathV[MAX_ROWS];
        int uCount = 0, vCount = 0;

        while (u != -1) {
            pathU[uCount++] = u;
            u = parent[u];
        }

        // Find the path to the root for v
        while (v != -1) {
            pathV[vCount++] = v;
            v = parent[v];
        }

        // Find the common ancestor
        int i = uCount - 1, j = vCount - 1;
        while (i >= 0 && j >= 0 && pathU[i] == pathV[j]) {
            i--;
            j--;
        }
        i++;
        j++;

        // Print the traversed edges
        printf("Edges traversed to find common ancestor:\n");
        for (int k = 0; k < uCount - 1; k++) {
            printf("(%d, %d)\n", pathU[k], pathU[k + 1]);
        }
        for (int k = 0; k < vCount - 1; k++) {
            printf("(%d, %d)\n", pathV[k], pathV[k + 1]);
        }
    }
}
