#ifndef PRM_GRAPH_UTILITY_H
#define PRM_GRAPH_UTILITY_H

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/breadth_first_search.hpp>
#include <boost/graph/depth_first_search.hpp>
#include <boost/graph/visitors.hpp>
#include <boost/range/adaptor/map.hpp>
#include <boost/parameter.hpp>

namespace utils
{
//! Directed graph
typedef boost::adjacency_list<boost::vecS,
                              boost::vecS,
                              boost::bidirectionalS,
                              boost::property<boost::vertex_distance_t, int>,
                              boost::property<boost::edge_weight_t, int> >
    DirectedGraph;

//! Undirected graph
typedef boost::adjacency_list<boost::vecS,
                              boost::vecS,
                              boost::undirectedS,
                              boost::property<boost::vertex_distance_t, int>,
                              boost::property<boost::edge_weight_t, int> >
    UndirectedGraph;

struct cycle_detector : public boost::dfs_visitor<>
{
    cycle_detector(bool &has_cycle)
        : _has_cycle(has_cycle) {}

    template <class Edge, class Graph>
    void back_edge(Edge, Graph &)
    {
        _has_cycle = true;
    }

  protected:
    bool &_has_cycle;
};

inline void getMoralGraph(UndirectedGraph &ug, const DirectedGraph &graph) 
{
	typedef boost::graph_traits<DirectedGraph>::vertex_descriptor Vertex;

	unsigned int nbNodes = boost::num_vertices(graph);

	for (unsigned int i = 0; i < nbNodes; i++ )
	{
		boost::add_vertex(ug);
	}

	boost::graph_traits<DirectedGraph>::vertex_iterator vi , vi_end;
	for (boost::tie(vi, vi_end) = vertices(graph); vi != vi_end; ++vi)
	{
		boost::graph_traits<DirectedGraph>::in_edge_iterator ei , ei_end, ei2, ei2_end;
		for (boost::tie(ei, ei_end) = in_edges(*vi, graph); ei != ei_end; ++ei)
		{
			Vertex s = source(*ei, graph);
			Vertex t = target(*ei, graph);

			for (boost::tie(ei2, ei2_end) = in_edges(*vi, graph); ei2 != ei2_end; ++ei2)
			{
				Vertex s1 = source(*ei, graph);
				Vertex s2 = source(*ei2, graph);
				if (s1 != s2 && !boost::edge(s1, s2, ug).second)
				{
					boost::add_edge(s1, s2, ug);
				}
			}
			if (!boost::edge(s, t, ug).second)
			{
				boost::add_edge(s, t, ug);
			}
		}
	}
}

template<class Graph>
unsigned int getMaxDegree(const Graph &g) 
{
    unsigned int nbNodes(boost::num_vertices(g));
    unsigned int maxDegree = 0;
    for (unsigned int i = 0; i < nbNodes; i++)
    { 
        unsigned int degree =  boost::degree(i, g);
        if ( maxDegree < degree) 
        {
            maxDegree = degree;
        }
    }
    return maxDegree;
}

template<class Graph>
unsigned int getMaxInDegree(const Graph &g) 
{
    unsigned int nbNodes(boost::num_vertices(g));
    unsigned int maxInDegree = 0;
    for (unsigned int i = 0; i < nbNodes; i++)
    { 
        unsigned int in_degree =  boost::in_degree(i, g);
        if ( maxInDegree < in_degree) 
        {
            maxInDegree = in_degree;
        }
    }
    return maxInDegree;
}
}
struct found_vertex : std::exception {}; 

template<class Graph, class Vertex, class Tag> 
struct reachable_visitor : public boost::base_visitor<reachable_visitor<Graph, Vertex, Tag>> { 
	typedef Tag event_filter;
    reachable_visitor(const Vertex& v) : vertex(v) {} 
    void operator()(const Vertex& v, const Graph& g) const { 
        if(v == vertex) throw found_vertex(); 
    }
private: 
    Vertex vertex;
};

template <class Graph, class Vertex, class Tag>
reachable_visitor<Graph, Vertex, Tag> reach_vertex(Vertex v, Tag) {
  return reachable_visitor<Graph, Vertex, Tag>(v);
}

template<class Graph, class Vertex> 
struct dfs_reachable_visitor : public boost::dfs_visitor<> { 
public: 
    dfs_reachable_visitor(const Vertex& v) : vertex(v) {} 
    void discover_vertex(const Vertex& v, const Graph& g) const { 
        if(v == vertex) throw found_vertex(); 
    }
private: 
    Vertex vertex;
};

/*template<class Graph, class Vertex> 
bool dfs_reachable(const Graph& g, const Vertex& start, const Vertex& end) { 
    try { 
		boost::depth_first_search(g, 
			boost::visitor(dfs_reachable_visitor<Graph, Vertex>(end)).root_vertex(start)
		)); 
    } catch(found_vertex&) { 
        return true; 
    } 
    return false; 
}*/

template<class Graph, class Vertex> 
struct bfs_reachable_visitor : boost::bfs_visitor<> { 
public: 
    bfs_reachable_visitor(const Vertex& v) : vertex(v) {} 
    void discover_vertex(const Vertex& v, const Graph&) const {
        if(v == vertex) throw found_vertex();
    }
private: 
    Vertex vertex;
};

template<class Graph, class Vertex> 
bool reachable(const Graph& g, const Vertex& start, const Vertex& end) { 
    try { 
        boost::breadth_first_search(g, start, boost::visitor(bfs_reachable_visitor<Graph, Vertex>(end))); 
    } catch(found_vertex&) { 
        return true; 
    } 
    return false; 
}

template<class Graph, class Vertex, class Edge, class EdgeIt> 
struct objects_slot_chain_visitor : boost::bfs_visitor<> { 
public: 
    objects_slot_chain_visitor(const std::string& slotChain) : slotChain(slotChain) {} 
    void discover_vertex(const Vertex& v, const Graph& g) const {
		//If this vertex is part of vertices in the slot chain
		if(std::find(betweenVertices.begin(), betweenVertices.end(), v) < betweenVertices.end()){
			std::pair<EdgeIt, EdgeIt> iterators = boost::out_edges(v, g);
			//For each edge that is going out this vertex
			for(EdgeIt it = iterators.first; it != iterators.second; it++){
				//If the edge links correspond to the correct slotChain part, add the vertex to "betweenVertices"
				Vertex t = boost::target(*(it.first), g);
				//...
				//If the edge correspond to the end of a slotchain, add the out_vertex to the resultVertices
				//...
			}
		}
    }
	void discover_edge(const Edge& e, const Graph& g) const{
		Vertex v = boost::target(e, g);
	}
private: 
    std::vector<Vertex> resultVertices;
	std::vector<Vertex> betweenVertices;
	std::string slotChain;
};


#endif  // PRM_GRAPH_UTILITY_H
