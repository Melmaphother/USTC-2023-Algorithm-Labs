#include "SparseMatrix.h"

#include <cassert>
#include <fstream>
#include <iostream>

#include <algorithm>
#include <chrono>

#include <queue>

Edge::Edge(int r, int c) : row(r), col(c) {}

bool Edge::operator<(const Edge &e) const {
	return row < e.row || (row == e.row && col < e.col);
}

SparseMatrix::SparseMatrix(std::string data_path, std::string mode) {
	this->mode = mode;
	assert(mode == "small" || mode == "large");
	std::ifstream f(data_path);
	assert(f.is_open());
	int	 row, col;
	char sep;
	int	 count	   = 0;
	int	 sep_count = mode == "small" ? 500000 : 5000000;
	std::cout << "Read file begin..." << std::endl;
	while (true) {
		if (mode == "small") {
			f >> row >> col;
		} else if (mode == "large") {
			f >> row >> sep >> col;
		}
		if (f.fail()) break;
		adj_list[row].insert(col);
		vertices.insert(row);
		vertices.insert(col);
		// edges.insert(std::move(Edge(row, col)));
		count++;
		if (count % sep_count == 0) {
			std::cout << count << ' ' << row << ' ' << col << std::endl;
		}
	}
	num_vertices = vertices.size();
	for (auto it = vertices.begin(); it != vertices.end(); it++) {
		visited[*it] = 0;
	}
	num_edges = edges.size();
	f.close();
	std::cout << "Read file end." << std::endl;
}

void SparseMatrix::bfs() {
	std::cout << "BFS begin..." << std::endl;

	auto start = std::chrono::high_resolution_clock::now();
	for (auto [vertex, isvisited] : visited) {
		if (isvisited == 0) {
			// std::cout << "BFS start from " << vertex << std::endl;
			bfs(vertex);
		}
	}
	auto end = std::chrono::high_resolution_clock::now();

	std::cout << "BFS end." << std::endl;

	bool flag = true;
	for (auto [vertex, isvisited] : visited) {
		if (isvisited == 0) {
			std::cout << "Vertex " << vertex << " is not visited." << std::endl;
			flag = false; //* once there is a vertex not visited, the graph
						  //* is not searched completed.
		}
	}
	if (flag) {
		std::cout << "Graph is searched completed." << std::endl;
	} else {
		std::cout << "Graph is not searched completed." << std::endl;
	}

	if (mode == "small") {
		auto duration =
			std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
		std::cout << "Duration of bfs is " << duration.count() << "ms"
				  << std::endl;
	} else {
		auto duration =
			std::chrono::duration_cast<std::chrono::seconds>(end - start);
		std::cout << "Duration of bfs is " << duration.count() << "s"
				  << std::endl;
	}
}

void SparseMatrix::bfs(int start_vertex) {
	std::queue<int> q;
	q.push(start_vertex);
	visited[start_vertex] = 1;
	while (!q.empty()) {
		int v = q.front();
		q.pop();
		for (auto it = adj_list[v].begin(); it != adj_list[v].end(); it++) {
			if (visited[*it] == 0) {
				q.push(*it);
				visited[*it] = 1;
			}
		}
	}
}

int SparseMatrix::get_num_vertices() { return num_vertices; }

int SparseMatrix::get_num_edges() { return num_edges; }