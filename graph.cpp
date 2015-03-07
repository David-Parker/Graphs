#include <unordered_map>
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

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

	bool operator==(const edge<types>& rhs) {
		return ((this->dest.getId()) == (rhs.dest.getId()));
	}

	bool operator!=(const edge<types>& rhs) {
		return !(operator==(rhs));
	}
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

	void removeEdge(edge<types>& e) {
		edges->erase(std::remove(edges->begin(), edges->end(), e), edges->end());
	}

	_id getId() const { return id; }
	std::vector<edge<types>>& getEdges() { return *edges; }

	bool operator==(const vertex<types>& rhs) {
		return (this->getId() == rhs.getId());
	}

	bool operator!=(const vertex<types>& rhs) {
		return !(operator==(rhs));
	}

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

	/* Look up the vertex, but do not create a new one if it doesn't exist */
	vertex<types> searchVertex(const _id& id) {
		auto dlookup = verticies.find(id);

		if(dlookup != verticies.end()) {
			return (dlookup->second);
		}
		else {
			throw "Vertex does not exist";
		}
	}

	void removeVertex(const vertex<types>& v) {
		verticies.erase(std::remove(verticies.begin(), verticies.end(), v), verticies.end());
	}

public:
	graph() : verticies() {}
	virtual ~graph() {}
	virtual void addEdge(const _id& d, const _id& s, const _wgt& w) = 0;
	virtual void removeEdge(const _id& d, const _id& s) = 0;
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

	virtual void removeEdge(const _id& d, const _id& s) {
		vertex<types> origin;
		try {
			origin = graph<types>::searchVertex(d);
		}
		catch(const char* msg) {
			std::cout << msg << std::endl;
			return;
		}
		/* Find the edge */
		for(edge<types>& e : origin.getEdges()) {
			if(e.getDest().getId() == s)
				origin.removeEdge(e);
		}

		/* Remove a node that has no more edges */
		// if(origin.getEdges().size() == 0);
		// 	graph<types>::removeVertex(origin);

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

	virtual void removeEdge(const _id& d, const _id& s) {
		vertex<types> origin;
		vertex<types> dest;
		try {
			origin = graph<types>::searchVertex(d);
			dest = graph<types>::searchVertex(s);
		}
		catch(const char* msg) {
			std::cout << msg << std::endl;
			return;
		}
		/* Find the edges */
		for(edge<types>& e : origin.getEdges()) {
			if(e.getDest().getId() == s)
				origin.removeEdge(e);
		}
		for(edge<types>& e : dest.getEdges()) {
			if(e.getDest().getId() == d)
				dest.removeEdge(e);
		}
	}

};