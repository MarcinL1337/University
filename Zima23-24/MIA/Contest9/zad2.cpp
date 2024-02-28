#include <iostream>
#include <bits/stdc++.h>
#include <string>
#include <cstring>

using namespace std;

#define ll long long int


int main(){
    int no_of_edges, no_of_vertices;
    cin >> no_of_vertices >> no_of_edges;

    int degrees[200000] = {0};

    int v1, v2;
    for(int i = 0; i < no_of_edges; i++){
        cin >> v1 >> v2;
        degrees[v1-1]++;
        degrees[v2-1]++;
    }

    int no_of_one_degrees = 0;
    int no_of_more_degrees = 0;
    for(int i = 0; i < no_of_vertices; i++){
        if(degrees[i] == 1) no_of_one_degrees++;
        if(degrees[i] > 2) no_of_more_degrees++;
    }

    if(no_of_more_degrees > 1){
        printf("unknown topology\n");
        return 0;
    }

    if((no_of_one_degrees > 2 && no_of_one_degrees < (no_of_vertices - 1)) || no_of_one_degrees == 1){
        printf("unknown topology\n");
        return 0;
    }

    if(no_of_one_degrees == 0) printf("ring topology\n");
    else if(no_of_one_degrees == 2) printf("bus topology\n");
    else if(no_of_one_degrees == no_of_vertices-1) printf("star topology\n");
    else printf("unknown topology\n");
}