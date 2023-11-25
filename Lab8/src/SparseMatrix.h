#include <map>
#include <set>
#include <string>
#include <unordered_map>
#include <unordered_set>

struct Edge {
public:
	Edge(int r, int c);
	bool operator<(const Edge &e) const;

private:
	int row;
	int col;
};

class SparseMatrix {
public:
	SparseMatrix(std::string data_path, std::string mode);
	void bfs();

public:
	int get_num_vertices();
	int get_num_edges();

private:
	int bfs(int start_vertex);

private:
	std::unordered_map<int, std::unordered_set<int>> adj_list;
	std::unordered_set<int>							 vertices;
	std::set<Edge>									 edges;
	std::unordered_map<int, bool>					 visited;
	int												 num_vertices;
	int												 num_edges;
	std::string										 mode;
};
