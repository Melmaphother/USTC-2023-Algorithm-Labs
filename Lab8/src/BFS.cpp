#include <cassert>
#include <fstream>
#include <iostream>
#include <queue>
#include <string>
#include <vector>

class Graph {
public:
	Graph(std::string data_path) {
		std::ifstream f(data_path);
		assert(f.is_open());
		std::string line;
		std::getline(f, line);
		num_vertices = 0;
		for (int i = 0; i < line.size(); i++) {
			if (line[i] != ',') { num_vertices++; }
		}
		graph.resize(num_vertices);
		for (int i = 0; i < num_vertices; i++) {
			graph[i].resize(num_vertices);
		}
		visited.resize(num_vertices);
		while (std::getline(f, line)) {
			int i		= line[0] - 'A';
			int j		= line[2] - 'A';
			graph[i][j] = 1;
			graph[j][i] = 1;
		}
		f.close();
	}

	void print_graph() {
        std::cout << "Graph:" << std::endl;
		for (int i = 0; i < num_vertices; i++) {
			for (int j = 0; j < num_vertices; j++) {
				std::cout << graph[i][j] << " ";
			}
			std::cout << std::endl;
		}
        std::cout << std::endl;
	}

    void bfs(int start) {
        std::queue<int> q;
        q.push(start);
        visited[start] = 1;
        while(!q.empty()) {
            int v = q.front();
            q.pop();
            std::cout << static_cast<char>(v + 'A') << " ";
            for(int i = 0; i < num_vertices; i++){
                if(graph[v][i] == 1 && visited[i] == 0){
                    q.push(i);
                    visited[i] = 1;
                }
            }
        }
    }

    ~Graph(){
        graph.clear();
        visited.clear();
    }

private:
	std::vector<std::vector<int>> graph;
	std::vector<int>			  visited;
	int							  num_vertices;
};

int main(){
    Graph g("../data/data.txt");
    g.print_graph();
    g.bfs(0);
    return 0;
}