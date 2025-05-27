#include <iostream>
#include <vector>
#include <set>
#include <stack>
#include <string>
#include <algorithm>

class Graph {
private:
    int vertices;
    std::vector<std::set<int>> adjList;

public:
    Graph(int v = 0) : vertices(v), adjList(v) {}

    friend std::istream& operator>>(std::istream& in, Graph& g);
    friend std::ostream& operator<<(std::ostream& out, const Graph& g);

    void display() const {
        for (int i = 0; i < vertices; i++) {
            std::cout << i << ": ";
            for (int neighbor : adjList[i]) {
                std::cout << neighbor << " ";
            }
            std::cout << "\n";
        }
    }

    Graph operator+(const Graph& other) const {
        int maxVertices = std::max(this->vertices, other.vertices);
        Graph result(maxVertices);

        for (int i = 0; i < this->vertices; i++) {
            result.adjList[i] = this->adjList[i];
        }

        for (int i = 0; i < other.vertices; i++) {
            result.adjList[i].insert(other.adjList[i].begin(), other.adjList[i].end());
        }

        return result;
    }

    Graph operator-(const Graph& other) const {
        int maxVertices = std::max(this->vertices, other.vertices);
        Graph result(maxVertices);

        for (int i = 0; i < maxVertices; i++) {
            for (int j = i + 1; j < maxVertices; j++) {
                if (i < this->vertices && j < this->vertices && this->adjList[i].count(j) &&
                    i < other.vertices && j < other.vertices && other.adjList[i].count(j)) {
                    result.adjList[i].insert(j);
                    result.adjList[j].insert(i);
                }
            }
        }

        return result;
    }

    Graph operator!() const {
        Graph comp(vertices);

        for (int u = 0; u < vertices; ++u) {
            for (int v = u + 1; v < vertices; ++v) {
                if (adjList[u].find(v) == adjList[u].end() && adjList[v].find(u) == adjList[v].end()) {
                    comp.addEdge(u, v);
                }
            }
        }

        return comp;
    }

    bool isReachable(int u, int v) const {
        if (u == v) return true;

        std::vector<bool> visited(vertices, false);
        std::stack<int> stack;
        stack.push(u);
        visited[u] = true;

        while (!stack.empty()) {
            int current = stack.top();
            stack.pop();

            for (int neighbor : adjList[current]) {
                if (neighbor == v) {
                    return true;
                }

                if (!visited[neighbor]) {
                    visited[neighbor] = true;
                    stack.push(neighbor);
                }
            }
        }

        return false;
    }

    void addEdge(int u, int v) {
        if (adjList[u].count(v) == 0) {
            adjList[u].insert(v);
            adjList[v].insert(u);
        }
    }

    void removeEdge(int u, int v) {
        if (adjList[u].count(v) > 0) {
            adjList[u].erase(v);
            adjList[v].erase(u);
        }
    }
};

std::istream& operator>>(std::istream& in, Graph& g) {
    in >> g.vertices;
    g.adjList.clear();
    g.adjList.resize(g.vertices);

    int edges;
    in >> edges;

    for (int i = 0; i < edges; i++) {
        int u, v;
        in >> u >> v;
        g.adjList[u].insert(v);
        g.adjList[v].insert(u);
    }
    return in;
}

std::ostream& operator<<(std::ostream& out, const Graph& g) {
    for (int i = 0; i < g.vertices; i++) {
        out << "Vertex " << i << ": ";
        for (int neighbor : g.adjList[i]) {
            out << neighbor << " ";
        }
        out << "\n";
    }
    return out;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);
    
    Graph g1, g2;
    std::string s;
    
    std::cin >> s; 
    std::cin >> g1; 

    while (s != "end") {
        std::cin >> s;

        if (s == "union") {
            std::cin >> s; 
            std::cin >> g2;
            g1 = g1 + g2; 
        } else if (s == "intersection") {
            std::cin >> s;
            std::cin >> g2;
            g1 = g1 - g2;
        } else if (s == "complement") {
            g1 = !g1;
        } else if (s == "isReachable") {
            int u, v;
            std::cin >> u >> v;
            std::cout << (g1.isReachable(u, v) ? "Yes\n" : "No\n");
        } else if (s == "add_edge") {
            int u, v;
            std::cin >> u >> v;
            g1.addEdge(u, v);
        } else if (s == "remove_edge") {
            int u, v;
            std::cin >> u >> v;
            g1.removeEdge(u, v);
        } else if (s == "printGraph") {
            std::cout << g1; 
        }
    }

    return 0;
}
