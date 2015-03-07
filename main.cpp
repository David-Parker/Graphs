/* 	The goal of this project is to create a system for creating generic graph types,
	complete with methods for constructing graphs and navigating them.				 */

#include <iostream>
#include <vector>
#include <map>
#include <stdio.h>
#include "lib.h"
#include "graph.inl"

using namespace std;
int main() {
	uni_graph<int, string> g;
	g.addEdge("Boston", "Plymouth",50);
	g.addEdge("Austin", "Boston",3000);
	g.addEdge("Austin", "Seattle",3500);
	// g.removeEdge("Austin", "Seattle");
	// g.removeEdge("Boston", "Plymouth");
	g.printEdges();
	return 0;
}