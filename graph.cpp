#include <unordered_map>
#include <vector>
#include <string>
#include "lib.h"

#define graphtypes template<typename _wgt, typename _id>
#define types _wgt,_id

/* Abstract Graph */
graphtypes class vertex;
graphtypes class edges;
graphtypes class graph;

/* Concrete graphs */
graphtypes class uni_graph;
graphtypes class bi_graph;

graphtypes
class edge {
private:
	vertex<types> dest;
	_wgt distance;

public:
	edge(const vertex<types>& _dest, const _wgt& _distance) :
		 dest(_dest), distance(_distance) {}

	vertex<types> getDest() { return dest; }
	_wgt getDistance() { return distance; }
};

graphtypes
class vertex {
private: 
	_id id;
	std::vector<edge<types>> *edges;

public:
	vertex() : id(), edges() { edges = new std::vector<edge<types>>(); }
	vertex(_id name) : id(name), edges() {edges = new std::vector<edge<types>>();}
	void pushEdge(edge<types>& e) {
		edges->push_back(e);
		std::cout << "name: " << e.getDest().getId() << " size: " << edges->size() << std::endl;
	}
	_id getId() { return id; }
	std::vector<edge<types>> getEdges() { return *edges; }

};

graphtypes
class graph {
protected:
	std::unordered_map<_id, vertex<types>> verticies;

public:
	graph() : verticies() {}
	virtual ~graph() {}
	virtual void addEdge(const _id& d, const _id& s, const _wgt& w) = 0;
	// void printEdges() {
	// 	for(auto& v: verticies) {
	// 		print("hey");
	// 		for(edge<types>& e : v.second.getEdges()) {
	// 			print("here/n");
	// 			std::cout << v.second.getId() << "--" << e.getDistance() << "--" << e.getDest().getId() << std::endl;
	// 			// print(v.second.getId())
	// 			// print(e.getDest().getId());
	// 		}
	// 	}
	// }
};

graphtypes
class uni_graph : public graph<types> {
private:
	vertex<types> findVertex(const _id& id) {
		auto dlookup = graph<types>::verticies.find(id);

		if(dlookup != graph<types>::verticies.end()) {
			std::cout << "Found existing vertex: " << dlookup->second.getId() << std::endl;
			return (dlookup->second);
		}
		else {
			// vertex<types> *new_d = new vertex<types>(id);
			// graph<types>::verticies[id] = *new_d;
			// return *new_d;
			vertex<types> new_d(id);
			graph<types>::verticies[id] = new_d;
			return new_d;
		}
	}

public:
	uni_graph() : graph<types>() {}
	virtual void addEdge(const _id& d, const _id& s, const _wgt& w) {
		vertex<types> origin = findVertex(d);
		vertex<types> dest = findVertex(s);

		// std::cout << "adding: " << origin.getId() << std::endl;
		// std::cout << "adding: " << dest.getId() << std::endl;
		// edge<types> *e = new edge<types>(dest,w);
		// origin.pushEdge(*e);
		edge<types> e(dest,w);
		origin.pushEdge(e);
		// for(edge<types>& e : origin.getEdges())
			// print(e.getDest().getId());
	}

	void printEdges() {
		for(auto& v: graph<types>::verticies) {
			std::cout << v.second.getId() << std::endl;
			// print(v.second.getEdges().size());
			for(edge<types>& e : v.second.getEdges()) {
				print("here");
				std::cout << v.second.getId() << "--" << e.getDistance() << "--" << e.getDest().getId() << std::endl;
				// print(v.second.getId())
				// print(e.getDest().getId());
			}
		}
	}
};