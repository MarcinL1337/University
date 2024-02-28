#include <cstdlib>
#include <iostream>
#include <cstdio>
#include <queue>
using namespace std;

struct node {
    struct node *next;
    int vertex;
    int weight;
};

struct Graph {
    struct node **adjList;
    int V;
};

struct node* createNode(int v, int weight){
    struct node* new_node = (struct node*)malloc(sizeof(struct node));
    new_node->next = NULL;
    new_node->weight = weight;
    new_node->vertex = v;
    return new_node;
}

void addEdge(struct Graph* g, int v, int u, int weight){
    struct node* new_node = createNode(v, weight);
    if(g->adjList[u] == NULL){
        g->adjList[u] = new_node;
    }
    else{
        struct node *temp = g->adjList[u];
        while(temp->next != NULL){
            temp = temp->next;
        }
        temp->next = new_node;
    }

    new_node = createNode(u, weight);
    if(g->adjList[v] == NULL){
        g->adjList[v] = new_node;
    }
    else{
        struct node *temp = g->adjList[v];
        while(temp->next != NULL){
            temp = temp->next;
        }
        temp->next = new_node;
    }
}

void printAdjList(struct Graph *g, int vert_count){
    struct node *cur_head; 
    for(int v = 0; v < vert_count; ++v){
        printf("Lista sąsiedztwa wierzchołka %d:\n", v+1);
        cur_head = g->adjList[v];
        while(cur_head){
            printf("%d (waga: %d) -> ", cur_head->vertex+1, cur_head->weight);
            cur_head = cur_head->next;
        }
        printf("NULL\n");
    }
}

struct compareWeight{
    bool operator()(struct node &node1, struct node &node2){
        return node1.weight < node2.weight;
    }
};

void Dijkstra(struct Graph* g, int source, int vert_count){
    int big_num = 1e9;

    priority_queue <struct node*, vector<struct node*>, compareWeight> min_heap;
    int distances[vert_count] = {big_num};
    bool visited[vert_count] = {false};
    distances[source] = 0;

    min_heap.push(g->adjList[source]);

    struct node* cur_min;
    while(!min_heap.empty()){
        cur_min = min_heap.top();
        min_heap.pop();
        int src_vertex = cur_min->vertex;
        struct node *temp = g->adjList[src_vertex]->next;
        while(temp != NULL){
            int cur_weight = temp->weight;
            if((cur_weight + distances[src_vertex]) > distances[temp->vertex]){
                distances[temp->vertex] = cur_weight + distances[src_vertex];
            }
            temp = temp->next;
        }
    }
}

int main() {
    int cities_count, paths_count, dest_count;
    scanf("%d %d %d\n", &cities_count, &paths_count, &dest_count);
    // printf("%d %d %d \n", cities_count, paths_count, dest_count);

    struct Graph* g = (struct Graph*)malloc(sizeof(struct Graph));
    g->adjList = (struct node**)malloc(cities_count * sizeof(struct node));
    char line[7];
    int dest, source, weight;
    for(int i = 0; i < paths_count; i++){
        fgets(line, 7, stdin);
        source = line[0] - 48;
        dest = line[2] - 48;
        weight = line[4] - 48;
        printf("%d %d %d \n", source, dest, weight);
        addEdge(g, source-1, dest-1, weight);
    }
    int dests[dest_count];
    for(int i = 0; i < dest_count; i++){
        scanf("%d", &dests[i]);
        // printf("%d ", dests[i]);
    }
    // printf("\n");
    printAdjList(g, cities_count);
    
    source = 0;
    Dijkstra(g, source, cities_count);
}