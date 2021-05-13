// TIN.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"
#include <iostream>
#include <string>
#include <fstream> 
#include <string>
#include <vector>
#include <sstream>  
using namespace std;

#define INF 1e9 

ofstream write, labelForward;
int adjMat[101][101], parent[101][101], dist[101][101];
vector <int> graph[101];
long double bandwidth[101][101];
vector <int> firstShortest[101][101], secondShortest[101][101];
vector <int> label;
vector <int> track;
int labelGenerator;
bool first = true;
char ch;
int firstDistance[101][101], secondDistance[101][101];
char f1[100], f2[100], f3[100], f4[100], f5[100];
int p;

void storePath(int V) 
{ 
    for (int v = 0; v < V; v++)
    {
        for (int u = 0; u < V; u++)
        {
            if (u != v && parent[v][u] != -1)
            {
               
				firstShortest[v][u].push_back(v);
				vector <int> vec;
				int temp = parent[v][u];
				
				while (temp != v){
					vec.push_back(temp);
					temp = parent[v][temp];
				}
                
				if (vec.size() > 0){
					for (int k = vec.size() - 1; k >= 0; k--){
						//cout << vec[k] << " ";
						firstShortest[v][u].push_back(vec[k]);
					}
				}
				firstShortest[v][u].push_back(u);
            }
        }
    } 

/*	for (int i = 0; i < V; i++){
		for (int j = 0; j < V; j++){
			cout << parent[i][j] << "  ";
		}
		cout << endl;
	}*/
} 


void floydWarshall (int V) 
{ 
  
    int i, j, k; 
 
   
    for (i = 0; i < V; i++) {
		for (j = 0; j < V; j++) {
			dist[i][j] = adjMat[i][j];

			if (i == j) {
                parent[i][i] = 0;
            }
            else if (dist[i][j] != INF) {
                parent[i][j] = i;
            }
            else {
                parent[i][j] = -1;
            }
		}
            
	}
         
	
    for (k = 0; k < V; k++) 
    { 
        for (i = 0; i < V; i++) 
        {           
            for (j = 0; j < V; j++) 
            { 
                 
                if (dist[i][k] + dist[k][j] < dist[i][j]) {
					dist[i][j] = dist[i][k] + dist[k][j]; 
					parent[i][j] = parent[k][j];
				}
                    
            } 
        } 
    } 

	for (int i = 0; i < V; i ++){
		for (int j = 0; j < V; j++){
			firstDistance[i][j] = dist[i][j];
		}
	}

	storePath(V); 
} 

void SecondShortest(int src, int dest, int V){

	int parP[101][101], disP[101][101];

	int i, j; 
   
    for (i = 0; i < V; i++) {
		for (j = 0; j < V; j++) {
			disP[i][j] = adjMat[i][j];
		}
            
	}

	vector <int> vec;
	int temp = parent[src][dest];

	while (temp != src){
		vec.push_back(temp);
		temp = parent[src][temp];
	}
    int x = src;           
	if (vec.size() > 0){
		for (int k = vec.size() - 1; k >= 0; k--){
			disP[x][vec[k]] = INF;
			disP[vec[k]][x] = INF;
			x = vec[k];
		}
	}
	disP[x][dest] = INF;
	

	for (i = 0; i < V; i++) {
		for (j = 0; j < V; j++) {
			if (i == j) {
                parP[i][i] = 0;
            }
            else if (dist[i][j] != INF) {
                parP[i][j] = i;
            }
            else {
                parP[i][j] = -1;
            }
		}          
	}



	

	for (int k = 0; k < V; k++) 
    { 
        for (int i = 0; i < V; i++) 
        {           
            for (int j = 0; j < V; j++) 
            { 
                 
                if (disP[i][k] + disP[k][j] < disP[i][j]) {
					disP[i][j] = disP[i][k] + disP[k][j]; 
					parP[i][j] = parP[k][j];
				}
                    
            } 
        } 
    } 
	secondDistance[src][dest] = disP[src][dest];
    secondShortest[src][dest].push_back(src);      
	vec.clear();
	temp = parP[src][dest];
				
	while (temp != src){
		vec.push_back(temp);
		temp = parP[src][temp];
	}
                
	if (vec.size() > 0){
		for (int k = vec.size() - 1; k >= 0; k--){
			//cout << vec[k] << " ";
			secondShortest[src][dest].push_back(vec[k]);
		}
	}
	secondShortest[src][dest].push_back(dest);

}
void PutValueInRoutingTable(int V){

	ofstream routingTable;
	routingTable.open (f3);
	
	if (!routingTable.is_open()) {
		cout << "ERROR!! NO FILE HAS BEEN OPENED" << endl;
	}

	for (int i = 0; i < V; i++){
		for (int j = 0; j < V; j++){

			if (i == j){
				continue;
			}
			routingTable << i << " " << j << endl;
			routingTable << "{ ";
			for (int k = 0; k < (int) firstShortest[i][j].size(); k++){
				routingTable << firstShortest[i][j][k] << " ";
			}
			routingTable << "} ";
			routingTable << firstDistance[i][j];
			routingTable << endl;
			routingTable << "{ ";
			for (int k = 0; k < (int) secondShortest[i][j].size(); k++){
				routingTable << secondShortest[i][j][k] << " ";
			}
			routingTable << "} ";
			routingTable << secondDistance[i][j];
			routingTable << endl << endl;
		}
	}
	routingTable.close();
}

bool isConnectionPossible(int v1, int v2, long double bmin, long double bav, long double bmax){
	
	long double beff;
	
	if (p == 0){
		beff = min (bmax, bav + 0.25 * (bmax - bmin));
	}
	
	else{
		beff = bmax;
	}
	
	// Checking for first shortest
	long double minBandwidthOfPath = INF;

	for (int i = 0; i < (int) firstShortest[v1][v2].size() - 1; i++){
	
		int n1 = firstShortest[v1][v2][i];
		int n2 = firstShortest[v1][v2][i + 1];

		minBandwidthOfPath = min (minBandwidthOfPath, bandwidth[n1][n2]);
	}

	if (minBandwidthOfPath < beff){
		// Checking for second shortest as first shortest connection not possible
		minBandwidthOfPath = INF;

		for (int i = 0; i < (int)secondShortest[v1][v2].size() - 1; i++){
	
			int n1 = secondShortest[v1][v2][i];
			int n2 = secondShortest[v1][v2][i + 1];

			minBandwidthOfPath = min (minBandwidthOfPath, bandwidth[n1][n2]);
		}

		if (minBandwidthOfPath < beff){
			// Neither first nor second shortest possible
			return false;
		}

		// Second shortest possible
		for (int i = 0; i < (int) secondShortest[v1][v2].size() - 1; i++){
			int n1 = secondShortest[v1][v2][i];
			int n2 = secondShortest[v1][v2][i + 1];

			bandwidth[n1][n2] -= beff;
			bandwidth[n2][n1] -= beff;
		}
		first = false;
		return true;

	}
	// First shortest possible
	for (int i = 0; i < (int) firstShortest[v1][v2].size() - 1; i++){
		int n1 = firstShortest[v1][v2][i];
		int n2 = firstShortest[v1][v2][i + 1];

		bandwidth[n1][n2] -= beff;
		bandwidth[n2][n1] -= beff;
	}
	return true;
}

int getOutInterface(int v1, int v2){
	for (int i = 0; i < (int) graph[v1].size(); i++){
		if (graph[v1][i] == v2){
			return i;
		}
	}
	return -1;

}

int getInInterface(int v1, int v2){
	for (int i = 0; i < (int) graph[v2].size(); i++){
		if (graph[v2][i] == v1){
			return i;
		}
	}
	return -1;

}

void discoverFirstShortestPath(int v1, int v2){
	int outInterface, inInterface = -1;
	
	labelGenerator = rand();
	
	for (int i = 0; i < (int) firstShortest[v1][v2].size() - 1; i++){
		int n1 = firstShortest[v1][v2][i];
		int n2 = firstShortest[v1][v2][i + 1];
		
		outInterface = getOutInterface(n1, n2);
		
		labelForward << inInterface << " ";
		
		if (i == 0){
			labelForward << -1 << " ";
		}
		else{
			labelForward << labelGenerator << " ";
		}

		labelForward << outInterface << " ";

		labelGenerator ++;

		labelForward << labelGenerator;

		labelForward << endl;
		label.push_back(labelGenerator);
		inInterface = getInInterface(n1, n2);

	}

	labelForward << inInterface << " ";
	labelForward << labelGenerator << " ";
	labelForward << -1 << " ";

	labelGenerator ++;

	labelForward << -1;
	labelForward << endl << endl;;

	for (int i = 0; i < (int) firstShortest[v1][v2].size(); i++){
		
		track.push_back(firstShortest[v1][v2][i]);
	}

}

void discoverSecondShortestPath(int v1, int v2){
	int outInterface, inInterface = -1;
	//labelGenerator = -1;
	
	for (int i = 0; i < (int) secondShortest[v1][v2].size() - 1; i++){
		int n1 = secondShortest[v1][v2][i];
		int n2 = secondShortest[v1][v2][i + 1];
		
		outInterface = getOutInterface(n1, n2);
		
		labelForward << inInterface << " ";
		
		if (i == 0){
			labelForward << -1 << " ";
		}
		else{
			labelForward << labelGenerator << " ";
		}

		labelForward << outInterface << " ";

		labelGenerator ++;

		labelForward << labelGenerator;

		labelForward << endl;
		label.push_back(labelGenerator);
		inInterface = getInInterface(n1, n2);

	}

	labelForward << inInterface << " ";
	labelForward << labelGenerator << " ";
	labelForward << -1 << " ";

	labelGenerator ++;

	labelForward << -1;
	labelForward << endl << endl;;

	for (int i = 0; i < (int) secondShortest[v1][v2].size(); i++){
		
		track.push_back(secondShortest[v1][v2][i]);
	}

}

void establishConnection(int v1, int v2, int id){
	label.clear();
	track.clear();

	if (first){
		discoverFirstShortestPath(v1, v2);
	}
	else{
		discoverSecondShortestPath(v1, v2);
	}
	write << id << " " << v1 << " " << v2 << " " << "{ ";
	for (int i = 0; i < (int) label.size(); i++){
		write << label[i] << " ";
	}
	write << "} ";
	/*for (int i = 0; i < (int) track.size(); i++){
		write << track[i] << " ";
	}*/
	if (first){
		write << firstDistance[v1][v2];
	}
	else{
		write << secondDistance[v1][v2];
	}
	write << endl;
}

int main(int argc, char* argv[])
{

	char hop_or_dist[100];
	p = atoi(argv[7]);
	strcpy(f1, argv[1]);
	strcpy(f2, argv[2]);
	strcpy(f3, argv[3]);
	strcpy(f4, argv[4]);
	strcpy(f5, argv[5]);
	strcat(f1, ".txt");
	strcat(f2, ".txt");
	strcat(f3, ".txt");
	strcat(f4, ".txt");
	strcat(f5, ".txt");
	strcpy(hop_or_dist, argv[6]);
	
	for (int i = 0; i < 101; i++){
		for (int j = 0; j < 101; j++){
			adjMat[i][j] = INF;
			bandwidth[i][j] = 0;
			if (i == j){
				adjMat[i][i] = 0;
			}
		}
	}
	ifstream fin;
	fin.open(f1);

	if (!fin.is_open()) {
		cout << "ERROR!! NO FILE HAS BEEN OPENED" << endl;
	}

	int vertices, edges;
	fin >> vertices;
	fin >> edges;
	
	for (int i = 0; i < edges; i++){
		int v1, v2, cost, bw;
		fin >> v1 >> v2 >> cost >> bw;
		graph[v1].push_back(v2);
		graph[v2].push_back(v1);
		if (strcmp(hop_or_dist, "hop") == 0){
			cost = 1;
		}
		adjMat[v1][v2] = cost;
		adjMat[v2][v1] = cost;
		bandwidth[v1][v2] = bw;
		bandwidth[v2][v1] = bw;
		//cout << v1 << " " << v2 << " " << cost << endl;
	}
   
	fin.close();
	floydWarshall(vertices);
	
	for (int i = 0; i < vertices; i++){
		for (int j = 0; j < vertices; j++){
			if (i != j){
				SecondShortest(i, j, vertices);
			}
			
		}
	}

	//SecondShortest(11, 5);
	PutValueInRoutingTable(vertices);

	fin.open(f2);
	if (!fin.is_open()) {
		cout << "ERROR!! NO FILE HAS BEEN OPENED" << endl;
	}

	write.open (f5);
	
	if (!write.is_open()) {
		cout << "ERROR!! NO FILE HAS BEEN OPENED" << endl;
	}
	
	labelForward.open(f4);
	
	if (!labelForward.is_open()) {
		cout << "ERROR!! NO FILE HAS BEEN OPENED" << endl;
	}
	
	int noOfConnections;
	fin >> noOfConnections;
	int count = 0;
	
	for (int i = 1; i <= noOfConnections; i++){
		
		int v1, v2;
		long double bmin, bav, bmax;
		fin >> v1 >> v2 >> bmin >> bav >> bmax;
		
		bool possible = isConnectionPossible(v1, v2, bmin, bav, bmax);
		
		if (!possible){
			continue;
		}
		cout << "Connection id = " << i << " admitted" << endl;
		establishConnection(v1, v2, i);
		count++;
	}
	cout << "Total Established Connection = " << count << " out of " << noOfConnections << " connections" << endl;
	write.close();
	labelForward.close();
	fin.close();
	system("pause");
	return 0;
}

