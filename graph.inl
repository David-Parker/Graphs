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
				std::cout << v.second.getId() << "--" << e.getDistance() << "--" << e.getDest().getId() << std::endl;
			}
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

		edge<TYPES> e(dest,w);
		origin.pushEdge(e);
	}

	virtual void removeEdge(const _id& d, const _id& s) {
		try {
			vertex<TYPES>& origin = graph<TYPES>::searchVertex(d);
			vertex<TYPES>& dest = graph<TYPES>::searchVertex(s);

				/* Find the edge */
			for(edge<TYPES>& e : origin.getEdges()) {
				if(e.getDest().getId() == s) {
					cout << "Deleting : " << origin.getId() << " -- " << e.getDest().getId() << endl;
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

GRAPHTYPES
class bi_graph : public uni_graph<TYPES> {
public:
	bi_graph() : uni_graph<TYPES>() {}
	virtual void addEdge(const _id& d, const _id& s, const _wgt& w) {
		vertex<TYPES>& origin = graph<TYPES>::findOrCreateVertex(d);
		vertex<TYPES>& dest = graph<TYPES>::findOrCreateVertex(s);

		origin.addIncomingEdge();
		dest.addIncomingEdge();

		edge<TYPES> e(dest,w);
		edge<TYPES> e2(origin,w);
		origin.pushEdge(e);
		dest.pushEdge(e2);
	}

	virtual void removeEdge(const _id& d, const _id& s) {
		try {
			vertex<TYPES>& origin = graph<TYPES>::searchVertex(d);
			vertex<TYPES>& dest = graph<TYPES>::searchVertex(s);

			/* Find the edges */
			for(edge<TYPES>& e : origin.getEdges()) {
				if(e.getDest().getId() == s)
					origin.deleteEdge(e);
			}
			for(edge<TYPES>& e : dest.getEdges()) {
				if(e.getDest().getId() == d)
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

#undef GRAPHTYPES
#undef TYPES