/* 	The goal of this project is to create a system for creating generic graph types,
	complete with methods for constructing graphs and navigating them.				 */

#include <iostream>
#include <vector>
#include <unordered_map>
#include <stdio.h>
#include <limits>
#include <Windows.h>
#include "lib.h"
#include "graph.inl"

#define TYPES int,string
#define PATHTYPES template <typename _id, typename _dist>
#define INF std::numeric_limits<int>::max()

using namespace std;

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

/* BFS - Returns a vector of verticies that is a the path from source to target */
PATHTYPES
vector<vertex<TYPES>> dijkstra(graph<TYPES>& graph, vertex<TYPES>& source, vertex<TYPES>& target) {
	unordered_map<_id, _dist> dist;
	unordered_map<_id, vertex<TYPES>*> prev;
	vector<vertex<TYPES>> queue;

	dist[*source] = 0;
	prev[*source] = NULL;

	for(auto& v : graph.getVerticies()) {
		if(*v.second != *source) {
			dist[*v.second] = INF;
			prev[*v.second] = NULL;
		}
		queue.push_back(v.second);
	}

	while(!queue.empty()) {
		try {
			vertex<TYPES> curr = minDistVertex(queue,dist);
			if(curr == target) {
				return createPath(prev, target);
			}

			queue.erase(remove(queue.begin(), queue.end(), *curr), queue.end());

			for(auto& e : curr.getEdges()) {
				_dist total = dist[*curr] + e.getDistance();
				if(total < dist[*e.getDest()]) {
					dist[*e.getDest()] = total;
					prev[*e.getDest()] = &graph.searchVertex(*curr);
				}
			}
		}

		catch(const char* msg) {
			cout << "No path from " << *source << " to " << *target << endl;
			return vector<vertex<TYPES>>();
		}
	}

	return createPath(prev, target);
}

PATHTYPES
vertex<TYPES> minDistVertex(vector<vertex<TYPES>>& queue, unordered_map<_id, _dist>& dist) {
	int min = INF;
	vertex<TYPES> minvert;
	for(auto& v : queue) {
		if(dist[v.getId()] < min) {
			minvert = v;
			min = dist[*v];
		}
	}

	if(minvert == vertex<TYPES>())
		throw "No possible path";

	return minvert;
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

	vector<vertex<TYPES>> path = dijkstra<string,int>(g, source, target);

	for(auto v : path) {
		cout << *v;
		if(*v != *path[path.size() - 1])
			cout << "->";
	}

	return 0;
}

