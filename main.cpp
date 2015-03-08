/* 	The goal of this project is to create a system for creating generic graph types,
	complete with methods for constructing graphs and navigating them.				 */

#include "graph.inl"

int main() {
	bi_graph<int, string> g;
	g.addEdge("A","B",4);
	g.addEdge("A","C",1);
	g.addEdge("C","D",1);
	g.addEdge("D","B",1);
	g.addEdge("B","F",2);

	vertex<int, string> source = g.searchVertex("A");
	vertex<int, string> target = g.searchVertex("F");

	deque<vertex<int, string>> path = dijkstra<int, string>(g, source, target);

	for(auto v : path) {
		cout << *v;
		if(*v != *path[path.size() - 1])
			cout << "->";
	}

	return 0;
}

