/* 	The goal of this project is to create a system for creating generic graph types,
	complete with methods for constructing graphs and navigating them.				 */

#include <iostream>
#include <vector>
#include <unordered_map>
#include <stdio.h>
#include "lib.h"
#include "graph.inl"

#define TYPES int,string
#define PATHTYPES template <typename _id, typename _dist>
using namespace std;


PATHTYPES
unordered_map<_id, vertex<TYPES>*> dijkstra(graph<TYPES>& graph, vertex<TYPES>& source, vertex<TYPES>& target) {
	unordered_map<_id, _dist> dist;
	unordered_map<_id, vertex<TYPES>*> prev;
	vector<vertex<TYPES>> queue;

	dist[*source] = 0;
	prev[*source] = NULL;

	for(auto& v : graph.getVerticies()) {
		if(*v.second != *source) {
			dist[*v.second] = 9999999;
			prev[*v.second] = NULL;
		}
		queue.push_back(v.second);
	}

	while(!queue.empty()) {
		vertex<TYPES> curr = minDistVertex(queue,dist);
		if(curr == target) {
			// cout << "Found!" << endl;
			return prev;
		}

		queue.erase(remove(queue.begin(), queue.end(), curr), queue.end());

		for(auto& e : curr.getEdges()) {
			_dist total = dist[*curr] + e.getDistance();
			if(total < dist[*e.getDest()]) {
				dist[*e.getDest()] = total;
				prev[*e.getDest()] = &graph.searchVertex(*curr);
			}
		}
	}

	return prev;

}

PATHTYPES
vertex<TYPES> minDistVertex(vector<vertex<TYPES>>& queue, unordered_map<_id, _dist>& dist) {
	int min = 9999999;
	vertex<TYPES> minvert;
	for(auto& v : queue) {
		if(dist[v.getId()] < min) {
			minvert = v;
			min = dist[*v];
		}
	}

	return minvert;
}

vector<vertex<TYPES>> createPath(unordered_map<string, vertex<TYPES>*>& path, vertex<TYPES> target) {
	vector<vertex<TYPES>> unwind;
	vertex<TYPES>* walk = path[*target];
	unwind.insert(unwind.begin(), target);
	while(walk != NULL) {
		unwind.insert(unwind.begin(), **walk);
		walk = path[**walk];
	}
	return unwind;
}

int main() {
	bi_graph<TYPES> g;
	g.addEdge("A","B",4);
	g.addEdge("A","C",1);
	g.addEdge("C","D",1);
	g.addEdge("D","B",1);
	g.addEdge("B","F",2);

	vertex<TYPES> source = g.searchVertex("A");
	vertex<TYPES> target = g.searchVertex("F");
	unordered_map<string, vertex<TYPES>*> path = dijkstra<string,int>(g, source, target);

	vector<vertex<TYPES>> finalPath = createPath(path, target);
	for(auto v : finalPath) {
		cout << *v << endl;
	}

	return 0;
}





	




















	// bi_graph<int, string> g;
	// g.addEdge("Boston", "Plymouth",50);
	// g.addEdge("Austin", "Boston",3000);
	// g.addEdge("Austin", "Seattle",3500);
	// // g.removeEdge("Austin", "Seattle");
	// g.removeEdge("Boston", "Plymouth");

	// vertex<int, string> v = g.searchVertex("Austin");
	// for(auto& e : v.getEdges())
	// 	cout << v.getId() << " -- " << e.getDest().getId() << endl;
	// // g.printEdges();

