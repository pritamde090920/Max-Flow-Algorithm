#include <stdio.h>

/*Basic definition*/
#define ZERO 0
#define ONE 1
#define TWO 2
#define MAXIMUM_ALLOWED_NODES 1000
#define increase 1000000000

/*Declarations*/
int nodes;  // number of nodes
int edges;  // number of edges
int capacity[MAXIMUM_ALLOWED_NODES][MAXIMUM_ALLOWED_NODES]; // capacity matrix
int flow[MAXIMUM_ALLOWED_NODES][MAXIMUM_ALLOWED_NODES];     // flow matrix
int num[MAXIMUM_ALLOWED_NODES]; // needed for breadth-first search               
int augumenting[MAXIMUM_ALLOWED_NODES];  // array to store augmenting path


int minimum (int x, int y) {
    return x<y ? x : y;  // returns the minimum of x and y
}

/*a queue for Breadth-First Search*/
int head,tail;
int q[MAXIMUM_ALLOWED_NODES+2];
void enqueue (int x) {
    q[tail] = x;
    tail++;
    num[x] = ONE;
}
int dequeue () {
    int x = q[head];
    head++;
    num[x] = TWO;
    return x;
}

/*Breadth-First Search for an augmenting path*/
int b_f_s (int start, int target) {
    int u,v;
    for (u=0; u<nodes; u++) {
	num[u] = ZERO;
    }   
    head = tail = 0;
    enqueue(start);
    augumenting[start] = -1;
    while (head!=tail) {
	u = dequeue();
        // Search all adjacent white nodes v. If the capacity
        // from u to v in the residual network is positive,
        // enqueue v.
	for (v=0; v<nodes; v++) {
	    if (num[v]==ZERO && capacity[u][v]-flow[u][v]>0) {
		enqueue(v);
		augumenting[v] = u;
	    }
	}
    }
    // If the num of the target node is black now,
    // it means that we reached it.
    return num[target]==TWO;
}
/* Using Ford-Fulkerson algorithm*/
int max_flow (int source, int sink) {
    int i,j,u;
    // Initialize empty flow.
    int max_flow = 0;
    for (i=0; i<nodes; i++) {
	for (j=0; j<nodes; j++) {
	    flow[i][j] = 0;
	}
    }
    // While there exists an augmenting path,
    // increment the flow along this path.
    while (b_f_s(source,sink)) {
        // Determine the amount by which we can increment the flow.
	int increment = increase;
	for (u=nodes-1; augumenting[u]>=0; u=augumenting[u]) {
	    increment = minimum(increment,capacity[augumenting[u]][u]-flow[augumenting[u]][u]);
	}
        // Now increment the flow.
	for (u=nodes-1; augumenting[u]>=0; u=augumenting[u]) {
	    flow[augumenting[u]][u] += increment;
	    flow[u][augumenting[u]] -= increment;
	}
	max_flow += increment;
    }
    // No augmenting path anymore. We are done.
    return max_flow;
}
/*Reading the input file*/
void read_input_file() {
    int a,b,c,i,j;
    FILE* input = fopen("mf.in","r");
    // read number of nodes and edges
    fscanf(input,"%d %d",&nodes,&edges);
    // initialize empty capacity matrix 
    for (i=0; i<nodes; i++) {
	for (j=0; j<nodes; j++) {
	    capacity[i][j] = 0;
	}
    }
    // read edge capacities
    for (i=0; i<edges; i++) {
	fscanf(input,"%d %d %d",&a,&b,&c);
	capacity[a][b] = c;
    }
    fclose(input);
}

/*The main function*/
int main () {
    read_input_file();
    printf("%d\n",max_flow(0,nodes-1));
    return 0;
}
