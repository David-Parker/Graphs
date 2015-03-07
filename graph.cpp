#include <unordered_map>
#include <iostream>
#include <vector>
#include <string>

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
	vertex(_id name) : id(name), edges() { edges = new std::vector<edge<types>>(); }
	void pushEdge(edge<types>& e) {
		edges->push_back(e);
	}
	_id getId() { return id; }
	std::vector<edge<types>> getEdges() { return *edges; }

};

graphtypes
class graph {
protected:
	std::unordered_map<_id, vertex<types>> verticies;

	/* Look up the verticies in O(1) using a hash table, if it doesn't exist -- create it. */
	vertex<types> findVertex(const _id& id) {
	auto dlookup = verticies.find(id);

	if(dlookup != verticies.end()) {
		return (dlookup->second);
	}
	else {
		vertex<types> new_d(id);
		verticies[id] = new_d;
		return new_d;
	}
}

public:
	graph() : verticies() {}
	virtual ~graph() {}
	virtual void addEdge(const _id& d, const _id& s, const _wgt& w) = 0;
};

graphtypes
class uni_graph : public graph<types> {
public:
	uni_graph() : graph<types>() {}
	virtual ~uni_graph() {}
	virtual void addEdge(const _id& d, const _id& s, const _wgt& w) {
		vertex<types> origin = graph<types>::findVertex(d);
		vertex<types> dest = graph<types>::findVertex(s);
		edge<types> e(dest,w);
		origin.pushEdge(e);
	}

	void printEdges() {
		for(auto& v: graph<types>::verticies) {
			std::cout << std::endl << v.second.getId() << ": " << std::endl;
			for(edge<types>& e : v.second.getEdges()) {
				std::cout << v.second.getId() << "--" << e.getDistance() << "--" << e.getDest().getId() << std::endl;
			}
		}
	}
};

graphtypes
class bi_graph : public uni_graph<types> {
public:
	bi_graph() : uni_graph<types>() {}
	virtual void addEdge(const _id& d, const _id& s, const _wgt& w) {
		vertex<types> origin = graph<types>::findVertex(d);
		vertex<types> dest = graph<types>::findVertex(s);

		edge<types> e(dest,w);
		edge<types> e2(origin,w);
		origin.pushEdge(e);
		dest.pushEdge(e2);
	}

};