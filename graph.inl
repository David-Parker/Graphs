#include <unordered_map>
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

#define GRAPHTYPES template<typename _wgt, typename _id>
#define TYPES _wgt,_id

/* Abstract Graph */
GRAPHTYPES class vertex;
GRAPHTYPES class edges;
GRAPHTYPES class graph;

/* Concrete graphs */
GRAPHTYPES class uni_graph;
GRAPHTYPES class bi_graph;

GRAPHTYPES
class edge {
private:
	vertex<TYPES> dest;
	_wgt distance;

public:
	edge(const vertex<TYPES>& _dest, const _wgt& _distance) :
		 dest(_dest), distance(_distance) {}

	vertex<TYPES> getDest() { return dest; }
	_wgt getDistance() { return distance; }

	bool operator==(const edge<TYPES>& rhs) {
		return ((this->dest.getId()) == (rhs.dest.getId()));
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

public:
	vertex() : id(), edges() { }
	vertex(_id name) : id(name), edges() { }
	void pushEdge(edge<TYPES>& e) {
		edges.push_back(e);
	}

	void removeEdge(edge<TYPES>& e) {
		edges.erase(std::remove(edges.begin(), edges.end(), e), edges.end());
	}

	_id getId() const { return id; }
	std::vector<edge<TYPES>>& getEdges() { return edges; }

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

	void removeVertex(const vertex<TYPES>& v) {
		verticies.erase(verticies.find(v.getId()));
	}

public:
	graph() : verticies() {}
	virtual ~graph() {}
	virtual void addEdge(const _id& d, const _id& s, const _wgt& w) = 0;
	virtual void removeEdge(const _id& d, const _id& s) = 0;
};

GRAPHTYPES
class uni_graph : public graph<TYPES> {
public:
	uni_graph() : graph<TYPES>() {}
	virtual ~uni_graph() {}
	virtual void addEdge(const _id& d, const _id& s, const _wgt& w) {
		vertex<TYPES>& origin = graph<TYPES>::findOrCreateVertex(d);
		vertex<TYPES>& dest = graph<TYPES>::findOrCreateVertex(s);
		edge<TYPES> e(dest,w);
		origin.pushEdge(e);
	}

	virtual void removeEdge(const _id& d, const _id& s) {
		vertex<TYPES> origin;
		try {
			origin = graph<TYPES>::searchVertex(d);
		}
		catch(const char* msg) {
			std::cout << msg << std::endl;
			return;
		}
		/* Find the edge */
		for(edge<TYPES>& e : origin.getEdges()) {
			if(e.getDest().getId() == s)
				origin.removeEdge(e);
		}

		/* Remove a node that has no more edges */
		if(origin.getEdges().size() == 0);
			graph<TYPES>::removeVertex(origin);

	}

	void printEdges() {
		for(auto& v: graph<TYPES>::verticies) {
			std::cout << std::endl << v.second.getId() << ": " << std::endl;
			for(edge<TYPES>& e : v.second.getEdges()) {
				std::cout << v.second.getId() << "--" << e.getDistance() << "--" << e.getDest().getId() << std::endl;
			}
		}
	}
};

GRAPHTYPES
class bi_graph : public uni_graph<TYPES> {
public:
	bi_graph() : uni_graph<TYPES>() {}
	virtual void addEdge(const _id& d, const _id& s, const _wgt& w) {
		vertex<TYPES>& origin = graph<TYPES>::findOrCreateVertex(d);
		vertex<TYPES>& dest = graph<TYPES>::findOrCreateVertex(s);

		edge<TYPES> e(dest,w);
		edge<TYPES> e2(origin,w);
		origin.pushEdge(e);
		dest.pushEdge(e2);
	}

	virtual void removeEdge(const _id& d, const _id& s) {
		vertex<TYPES> origin;
		vertex<TYPES> dest;
		try {
			origin = graph<TYPES>::searchVertex(d);
			dest = graph<TYPES>::searchVertex(s);
		}
		catch(const char* msg) {
			std::cout << msg << std::endl;
			return;
		}
		/* Find the edges */
		for(edge<TYPES>& e : origin.getEdges()) {
			if(e.getDest().getId() == s)
				origin.removeEdge(e);
		}
		for(edge<TYPES>& e : dest.getEdges()) {
			if(e.getDest().getId() == d)
				dest.removeEdge(e);
		}
	}

};

#undef GRAPHTYPES
#undef TYPES