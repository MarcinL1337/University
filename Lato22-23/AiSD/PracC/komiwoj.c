#include <stdlib.h>
#include <stdio.h>

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
    struct node* new_node = malloc(sizeof(struct node));
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

void Dijkstra(struct Graph* g, int source, int vert_count){
    int big_num = 1e7;
}

int main() {
    int cities_count, paths_count, dest_count;
    scanf("%d %d %d\n", &cities_count, &paths_count, &dest_count);
    // printf("%d %d %d \n", cities_count, paths_count, dest_count);

    struct Graph* g = malloc(sizeof(struct Graph));
    g->adjList = malloc(cities_count * sizeof(struct node));
    char line[6];
    int dest, source, weight;
    for(int i = 0; i < paths_count; i++){
        // fgets(line, 6, stdin);
        gets(line);
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
    
}