#include "DefinitionTypes.h"
#include "CornerTable.h"
#include <fstream>
#include <iostream>
#include <vector>
#include <set>
#include <chrono>
#include <list>

using namespace std;
using namespace chrono;

struct Triangle{
    unsigned int vertex1, vertex2, vertex3;
};

int __d_bfs;

#define INF 9999

bool isConected(Triangle A, Triangle B){

    set<unsigned int> sa;
    sa.insert(A.vertex1);
    sa.insert(A.vertex2);
    sa.insert(A.vertex3);

    int cont = 0;
    if (sa.find(B.vertex1) != sa.end()) cont++;
    if (sa.find(B.vertex2) != sa.end()) cont++;
    if (sa.find(B.vertex3) != sa.end()) cont++;

    return cont>1;
}

/* Algoritmo Floyd Warshall */

// Valor arbitario (PARAMETRIZAR DESPUES)
//#define verticesGrafo 4
//
//// Imprime los caminos encontrados en el grafo
//void imprimirMatrizDistancias(int distancias[][verticesGrafo]){
//	for(int i=0; i<verticesGrafo; i++){
//	        cout << "Nodo " << i << ": ";
//	        for(int j=0; j<verticesGrafo; j++){
//	            if(distancias[i][j] == INF){
//	                cout << "-" << "->";
//	            }
//	            else{
//	                cout << distancias[i][j] << "->";
//	            }
//	        }
//	        cout << "FIN" << endl;
//	    }
//}

// Calcula los caminos encontrados en el grafo
//void floydWarshall(int grafo[][verticesGrafo]){
//	// matriz de salida donde se guardan las distancias mas cortas entre 2 vertices
//	    int distancias[verticesGrafo][verticesGrafo];
//
//	    // definimos 3 iteradores
//	    int i,j,k;
//
//	    // inicializar matriz de salida
//	    for(i=0; i<verticesGrafo; i++){
//	        for(j=0; j<verticesGrafo; j++){
//	            distancias[i][j] = grafo[i][j];
//	        }
//	    }
//
//	    // se agnaden uno a uno los vertices a un set de vertices intermedios.
//	    for(k=0; k<verticesGrafo; k++){
//	        // se toma a los vertices como entrada
//	        for(i=0; i<verticesGrafo; i++){
//	            // se toma a los vertices como salida
//	            for(j=0; j<verticesGrafo; j++){
//	                // si el vertice k esta en el camino mas corto desde i hacia j
//	                // se guardan en la matriz distancias (se actualiza la matriz)
//	                if(distancias[i][k] + distancias[k][j] < distancias[i][j]){
//	                    distancias[i][j] = distancias[i][k] + distancias[k][j];
//	                }
//	            }
//	        }
//	    }
//	    imprimirMatrizDistancias(distancias);
//}

/*  ------------ ALGORITMO BFS ------------ */

void add_edge(vector<int> adj[], int src, int dest)
{
    adj[src].push_back(dest);
    adj[dest].push_back(src);
}

void buildAdjMatBFS(CornerTable* CT, vector<int> adjMat[]){
    const CornerType* triangleList = CT->getTriangleList();
    unsigned int numTriangles = CT->getNumTriangles();

    for (unsigned int i=0; i<numTriangles; i++){
        Triangle v{triangleList[3*i],
                triangleList[3*i + 1],
                triangleList[3*i + 2]
                };
        for (unsigned int j=i+1; j<numTriangles; j++){
            Triangle comp{triangleList[3*j],
                        triangleList[3*j + 1],
                        triangleList[3*j + 2]
                        };
            if (isConected(v, comp)){
                add_edge(adjMat, i, j);
            }
        }
    }
}

bool BFS(vector<int> adj[], int src, int dest, int v, int pred[], int dist[]){

    list<int> queue; // a queue of vertices
    bool visited[v];

    for (int i = 0; i < v; i++) {
        visited[i] = false; // initially all vertices are unvisited
        dist[i] = INT_MAX; //set to infinity
        pred[i] = -1;
    }

    visited[src] = true; // source is first to be visited
    dist[src] = 0; // distance from source to itself should be 0
    queue.push_back(src);

    // BFS algorithm
    while (!queue.empty()) {
        int u = queue.front();
        queue.pop_front();
        for (unsigned int i = 0; i < adj[u].size(); i++) {
            if (!visited[adj[u][i]]) {
                visited[adj[u][i]] = true;
                dist[adj[u][i]] = dist[u] + 1;
                pred[adj[u][i]] = u;
                queue.push_back(adj[u][i]);

                //Destination found
                if (adj[u][i] == dest)
                    return true;
            }
        }
    }
    return false;
}

void bfsPath(vector<unsigned int> &path, vector<int> adj[], int s,
                           int dest, int v){
    int pred[v], dist[v];
    if (!BFS(adj, s, dest, v, pred, dist)) {
        cout << "El origen y el destino no estan conectados.";
        return;
    }

    int crawl = dest;
    path.push_back(crawl);
    while (pred[crawl] != -1) {
        path.push_back(pred[crawl]);
        crawl = pred[crawl];
    }
    __d_bfs = dist[dest];
}

void MinPathBFS(vector<unsigned int> &path, CornerTable *CT, int o, int d) {

	auto start = high_resolution_clock::now();

    vector<int> adjmat[CT->getNumTriangles()];
    buildAdjMatBFS(CT, adjmat);

    auto stop1 = high_resolution_clock::now();

    duration<double> diff1 = duration_cast<microseconds>(stop1 - start);

    bfsPath(path, adjmat, o, d, CT->getNumTriangles());

    auto stop2 = high_resolution_clock::now();

    duration<double> diff2 = duration_cast<microseconds>(stop2 - stop1);

    cout << "__________________[CAMINO ENCONTRADO]__________________"<<endl;
    cout << "\t(*) Cantidad de triangulos en el camino: "<< __d_bfs<<endl;
    cout << "\t(*) Tiempo en calculo de Mat. Adyacencia: "<<diff1.count()<<" seg."<<endl;
    cout << "\t(*) Tiempo en Algoritmo BFS: "<<diff2.count()<<" seg."<<endl;
    cout << "\t(*) Camino mas corto encontrado: "<<endl;
    for (int i=path.size()-1; i>0; i--)
        cout << path[i] << " -> ";
    cout<<path[0]<<endl;
}


/*  ------------ ALGORITMO DIJKSTRA ------------ */

int minCost(vector<unsigned int> cost, vector<bool> marked){
    int min = INF;
    int i_min = -1;
    for (unsigned int i=0; i<cost.size(); i++){
        if (!marked[i] && cost[i]<(unsigned int)min){
            min = cost[i];
            i_min = i;
        }
    }
    return i_min;
}

void DijkstraPath(vector<unsigned int> &pred, vector<vector<unsigned int>> adjmat, int i, int d){

    vector<bool> marked(adjmat.size(), 0);
    vector<unsigned int> cost(adjmat.size(), INF);
    pred.resize(adjmat.size());
    fill(pred.begin(), pred.end(), -1);

    cost[i] = 0;

    for (unsigned int i=0; i<adjmat.size(); i++){
        int i_node = minCost(cost, marked);

        if (i_node == -1) // all is marked
            break;

        marked[i_node] = true;

        for (unsigned int j=0; j<adjmat.size(); j++){
            if (marked[j] || !adjmat[i_node][j]) continue;
            if (cost[i_node] + adjmat[i_node][j] < cost[j]){
                cost[j] =  cost[i_node] + adjmat[i_node][j];
                pred[j] = i_node;
            }
        }
    }
}

void buildAdjMat(CornerTable* CT, vector<vector<unsigned int>> &adjMat){
    const CornerType* triangleList = CT->getTriangleList();
    unsigned int numTriangles = CT->getNumTriangles();
    adjMat.resize(numTriangles);
    fill(adjMat.begin(), adjMat.end(), vector<unsigned int>(numTriangles, 0));
    vector<int> count(numTriangles, 0);

    for (unsigned int i=0; i<numTriangles; i++){
    	if (count[i]==3) continue;
        Triangle v{triangleList[3*i],
                triangleList[3*i + 1],
                triangleList[3*i + 2]
                };
        for (unsigned int j=i+1; j<numTriangles; j++){
        	if (count[j]==3) continue;
        	Triangle comp{triangleList[3*j],
                        triangleList[3*j + 1],
                        triangleList[3*j + 2]
                        };
            if (isConected(v, comp)){
                adjMat[i][j] = 1;
                adjMat[j][i] = 1;
                count[i]++; count[j]++;
            }
        }
    }
}

void MinPathDijkstra(vector<unsigned int> &path, CornerTable *CT, int o, int d) {

	auto start = high_resolution_clock::now();

	vector<vector<unsigned int>> adjmat; buildAdjMat(CT, adjmat);

    auto stop1 = high_resolution_clock::now();
    duration<double> diff1 = duration_cast<microseconds>(stop1 - start);

    if (CT->getNumTriangles() < 20){ // imprime la matriz de adyacencia solo si no es tan grande
        cout<<"\nMatriz de adyacencia: "<<endl;
        for(unsigned int i =0; i<adjmat.size(); i++){
            for (unsigned int j=0; j<adjmat.size(); j++){
                cout<<adjmat[i][j]<<" ";
            }
            cout<<endl;
        }
    }
    vector<unsigned int> pred;
    DijkstraPath(pred, adjmat, o, d);
    int r = pred[d];
    path.push_back(d);
    while(r > -1 && r != o){
        path.push_back(r);
        r = pred[r];
    }
    path.push_back(o);

    auto stop2 = high_resolution_clock::now();
    duration<double> diff2 = duration_cast<microseconds>(stop2 - stop1);

    cout << "__________________[CAMINO ENCONTRADO]__________________"<<endl;
    cout << "\t(*) Cantidad de triangulos en el camino: "<<path.size()-1<<endl;
    cout << "\t(*) Tiempo en calculo de Mat. Adyacencia: "<<diff1.count()<<" seg."<<endl;
    cout << "\t(*) Tiempo en Algoritmo de Dijkstra: "<<diff2.count()<<" seg."<<endl;
    cout << "\t(*) Camino mas corto encontrado: \n";
    for (int i=path.size()-1; i>0; i--)
        cout << path[i] << " -> ";
    cout<<path[0]<<endl;
}

