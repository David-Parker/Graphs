#include <stdlib.h>
#include <unordered_map>
#include <iostream>
#include <vector>
#include <deque>
#include <string>
#include <algorithm>

#define GRAPHTYPES template<typename _wgt, typename _id>
#define TYPES _wgt,_id
#define INF std::numeric_limits<int>::max()

/* Abstract Graph */
GRAPHTYPES class vertex;
GRAPHTYPES class edges;
GRAPHTYPES class graph;

/* Concrete graphs */
GRAPHTYPES class uni_graph;
GRAPHTYPES class bi_graph;

using namespace std;

GRAPHTYPES
class edge {
private:
	_id dest;
	_wgt distance;

public:
	edge(const _id& _dest, const _wgt& _distance) :
		 dest(_dest), distance(_distance) {}

	_id getDest() { return dest; }
	_wgt getDistance() { return distance; }

	bool operator==(const edge<TYPES>& rhs) {
		return (dest == rhs.dest);
	}

	bool operator!=(const edge<TYPES>& rhs) {
		return !(operator==(rhs));
	}
};

GRAPHTYPES
class vertex {
private: 
	_id id;
	std::vector<edge<TYPES>> edges;
	int incomingEdges;

public:
	vertex() : id(), edges(), incomingEdges(0) {}
	vertex(_id name) : id(name), edges(), incomingEdges(0) {}
	int getIncomingEdges() { return incomingEdges; }
	void addIncomingEdge() { incomingEdges++; }
	void removeIncomingEdge() { incomingEdges--; }
	void pushEdge(edge<TYPES> e) {
		edges.push_back(e);
	}

	void deleteEdge(const edge<TYPES>& e) {
		edges.erase(std::remove(edges.begin(), edges.end(), e), edges.end());
	}

	_id getId() const { return id; }
	std::vector<edge<TYPES>>& getEdges() { return edges; }

	bool operator==(const vertex<TYPES>& rhs) {
		return this->id == rhs.id;
	}
	_id operator*() {
		return id;
	}

};

GRAPHTYPES
class graph {
protected:
	std::unordered_map<_id, vertex<TYPES>> verticies;

	/* Look up the verticies in O(1) using a hash table, if it doesn't exist -- create it. */
	vertex<TYPES>& findOrCreateVertex(const _id& id) {
		auto dlookup = verticies.find(id);

		if(dlookup != verticies.end()) {
			return (dlookup->second);
		}
		else {
			vertex<TYPES> new_d(id);
			verticies[id] = new_d;
			return verticies[id];
		}
	}

	void removeVertex(const vertex<TYPES>& v) {
		verticies.erase(verticies.find(v.getId()));
	}

	void checkAndRemoveGarbage(vertex<TYPES>& v) {
		if(v.getEdges().size() == 0 && v.getIncomingEdges() == 0)
			removeVertex(v);
	}

public:
	graph() : verticies() {}
	virtual ~graph() {}
	virtual void addEdge(const _id& d, const _id& s, const _wgt& w) = 0;
	virtual void removeEdge(const _id& d, const _id& s) = 0;
	void printEdges() {
		for(auto v : verticies) {
			std::cout << std::endl << v.second.getId() << ": " << std::endl;
			for(edge<TYPES>& e : v.second.getEdges()) {
				std::cout << v.second.getId() << "--" << e.getDistance() << "--" << e.getDest() << std::endl;
			}
		}
	}

	std::unordered_map<_id, vertex<TYPES>>& getVerticies() { return verticies; }


	/* Look up the vertex, but do not create a new one if it doesn't exist */
	vertex<TYPES>& searchVertex(const _id& id) {
		auto dlookup = verticies.find(id);

		if(dlookup != verticies.end()) {
			return (dlookup->second);
		}
		else {
			throw "Vertex does not exist";
		}
	}
};


GRAPHTYPES
class bi_graph : public graph<TYPES> {
public:
	bi_graph() : graph<TYPES>() {}
	virtual void addEdge(const _id& d, const _id& s, const _wgt& w) {
		vertex<TYPES>& origin = graph<TYPES>::findOrCreateVertex(d);
		vertex<TYPES>& dest = graph<TYPES>::findOrCreateVertex(s);

		origin.addIncomingEdge();
		dest.addIncomingEdge();

		edge<TYPES> e(s,w);
		edge<TYPES> e2(d,w);
		origin.pushEdge(e);
		dest.pushEdge(e2);
	}

	virtual void removeEdge(const _id& d, const _id& s) {
		try {
			vertex<TYPES>& origin = graph<TYPES>::searchVertex(d);
			vertex<TYPES>& dest = graph<TYPES>::searchVertex(s);

			/* Find the edges */
			for(edge<TYPES>& e : origin.getEdges()) {
				if(e.getDest() == s)
					origin.deleteEdge(e);
			}
			for(edge<TYPES>& e : dest.getEdges()) {
				if(e.getDest() == d)
					dest.deleteEdge(e);
			}

			origin.removeIncomingEdge();
			dest.removeIncomingEdge();

			/* Remove a node that has no more edges */
			graph<TYPES>::checkAndRemoveGarbage(origin);
			graph<TYPES>::checkAndRemoveGarbage(dest);
		}

		catch(const char* msg) {
			std::cout << msg << std::endl;
			return;
		}
	}
};

GRAPHTYPES
class uni_graph : public graph<TYPES> {
public:
	uni_graph() : graph<TYPES>() {}
	virtual ~uni_graph() {}
	virtual void addEdge(const _id& d, const _id& s, const _wgt& w) {
		vertex<TYPES>& origin = graph<TYPES>::findOrCreateVertex(d);
		vertex<TYPES>& dest = graph<TYPES>::findOrCreateVertex(s);
		dest.addIncomingEdge();

		edge<TYPES> e(s,w);
		origin.pushEdge(e);
	}

	virtual void removeEdge(const _id& d, const _id& s) {
		try {
			vertex<TYPES>& origin = graph<TYPES>::searchVertex(d);
			vertex<TYPES>& dest = graph<TYPES>::searchVertex(s);

				/* Find the edge */
			for(edge<TYPES>& e : origin.getEdges()) {
				if(e.getDest() == s) {
					origin.deleteEdge(e);
				}
			}

			dest.removeIncomingEdge();

			/* Remove a node that has no more edges */
			graph<TYPES>::checkAndRemoveGarbage(origin);
			graph<TYPES>::checkAndRemoveGarbage(dest);
		}
		catch(const char* msg) {
			std::cout << msg << std::endl;
			return;
		}
	}
};


/* Global functions for path finding */
GRAPHTYPES
deque<vertex<TYPES>> createPath(unordered_map<_id, vertex<TYPES>*>& path, vertex<TYPES> target) {
	deque<vertex<TYPES>> unwind;
	vertex<TYPES>* walk = path[*target];
	unwind.push_front(target);
	while(walk != NULL) {
		unwind.insert(unwind.begin(), **walk);
		walk = path[**walk];
	}
	return unwind;
}

/* BFS - Returns a deque of verticies that is a the path from source to target */
GRAPHTYPES
deque<vertex<TYPES>> dijkstra(graph<TYPES>& graph, vertex<TYPES>& source, vertex<TYPES>& target) {
	unordered_map<_id, _wgt> dist;
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
				_wgt total = dist[*curr] + e.getDistance();
				if(total < dist[e.getDest()]) {
					dist[e.getDest()] = total;
					prev[e.getDest()] = &graph.searchVertex(*curr);
				}
			}
		}

		catch(const char* msg) {
			cout << "No path from " << *source << " to " << *target << endl;
			return deque<vertex<TYPES>>();
		}
	}

	return createPath(prev, target);
}

GRAPHTYPES
vertex<TYPES> minDistVertex(vector<vertex<TYPES>>& queue, unordered_map<_id, _wgt>& dist) {
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

#undef GRAPHTYPES
#undef TYPES