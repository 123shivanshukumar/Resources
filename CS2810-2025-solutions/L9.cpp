#include <iostream>
#include <vector>
#include <stack>
#include <algorithm>
using namespace std;

const int MAXN = 100005;

class GraphAlgorithm {
public:
    virtual void Query() = 0;
    virtual ~GraphAlgorithm() {}
};

class Graph {
public:
    int N, M;
    vector<int> hype;
    vector<vector<int>> adj, rev_adj;
    vector<bool> visited;
    vector<int> scc_id, order, component;
    int scc_count;

    Graph(int n, int m) : N(n), M(m), hype(N+1), adj(N+1), rev_adj(N+1), visited(N+1), scc_id(N+1), order(N+1) {}

    void addEdge(int u, int v) {
        adj[u].push_back(v);
        rev_adj[v].push_back(u);
    }

    void dfs1(int v) {
        visited[v] = true;
        for (int u : adj[v]) {
            if (!visited[u]) dfs1(u);
        }
        order.push_back(v);
    }

    void dfs2(int v) {
        visited[v] = true;
        component.push_back(v);
        scc_id[v] = scc_count;
        for (int u : rev_adj[v]) {
            if (!visited[u]) dfs2(u);
        }
    }

    void findSCCs() {
        visited.assign(N+1, false);
        for (int i = 1; i <= N; i++) {
            if (!visited[i]) dfs1(i);
        }
        visited.assign(N+1, false);
        reverse(order.begin(), order.end());
        scc_count = 0;
        for (int v : order) {
            if (!visited[v]) {
                component.clear();
                dfs2(v);
                scc_count++;
            }
        }
    }
};

class maxHype : public GraphAlgorithm {
private:
    Graph& G;
    vector<int> scc_hype, dp;
    vector<vector<int>> scc_adj;

public:
    maxHype(Graph& g) : G(g), dp(G.N+1, -1), scc_hype(G.N+1, 0) {}

    int dfs(int v) {
        if (dp[v] != -1) return dp[v];
        dp[v] = scc_hype[v];
        int max_path = 0;
        for (int u : scc_adj[v]) {
            max_path = max(max_path, dfs(u));
        }
        dp[v] += max_path;
        return dp[v];
    }

    void Query() override {
        G.findSCCs();

        scc_hype.resize(G.scc_count);
        scc_adj.resize(G.scc_count);

        for (int v = 1; v <= G.N; ++v) {
            scc_hype[G.scc_id[v]] += G.hype[v];
        }

        for (int u = 1; u <= G.N; ++u) {
            for (int v : G.adj[u]) {
                if (G.scc_id[u] != G.scc_id[v]) {
                    scc_adj[G.scc_id[u]].push_back(G.scc_id[v]);
                }
            }
        }

        int max_hype = 0;
        for (int i = 0; i < G.scc_count; ++i) {
            max_hype = max(max_hype, dfs(i));
        }

        cout << max_hype << endl;
    }
};

class isCycle : public GraphAlgorithm {
private:
    Graph& G;
    vector<int> color;

    bool dfs(int v) {
        color[v] = 1;
        for (int u : G.adj[v]) {
            if (color[u] == 1) return true;
            if (color[u] == 0 && dfs(u)) return true;
        }
        color[v] = 2;
        return false;
    }

public:
    isCycle(Graph& g) : G(g), color(G.N+1, 0) {}

    void Query() override {
        bool hasCycle = false;
        for (int i = 1; i <= G.N; i++) {
            if (color[i] == 0 && dfs(i)) {
                hasCycle = true;
                break;
            }
        }
        cout << (hasCycle ? "YES" : "NO") << endl;
    }
};

class indepComponent : public GraphAlgorithm {
private:
    Graph& G;
    vector<int> disc, low, stackMember;
    vector<int> st;
    int time;
    int scc_count;
    int max_size;

    void tarjanSCC(int u) {
        disc[u] = low[u] = ++time;
        st.push_back(u);
        stackMember[u] = 1;

        for (int v : G.adj[u]) {
            if (disc[v] == -1) {
                tarjanSCC(v);
                low[u] = min(low[u], low[v]);
            } else if (stackMember[v]) {
                low[u] = min(low[u], disc[v]);
            }
        }

        int w = 0;
        if (low[u] == disc[u]) {
            int component_size = 0;
            while (st.back() != u) {
                w = st.back();
                stackMember[w] = 0;
                st.pop_back();
                component_size++;
            }
            w = st.back();
            stackMember[w] = 0;
            st.pop_back();
            component_size++;
            
            scc_count++;
            max_size = max(max_size, component_size);
        }
    }

public:
    indepComponent(Graph& g) : G(g), disc(G.N+1, -1), low(G.N+1, -1), stackMember(G.N+1, 0), time(0), scc_count(0), max_size(0) {}

    void Query() override {
        for (int i = 1; i <= G.N; i++) {
            if (disc[i] == -1) {
                tarjanSCC(i);
            }
        }
        cout << scc_count << " " << max_size << endl;
    }
};


class validOrder : public GraphAlgorithm {
private:
    Graph& G;
    vector<int> in_degree;

public:
    validOrder(Graph& g) : G(g), in_degree(G.N+1, 0) {}

    void Query() override {
        for (int i = 1; i <= G.N; i++) {
            for (int v : G.adj[i]) {
                in_degree[v]++;
            }
        }

        vector<int> order;
        vector<int> q;
        for (int i = 1; i <= G.N; i++) {
            if (in_degree[i] == 0) q.push_back(i);
        }
        sort(q.begin(), q.end());

        while (!q.empty()) {
            int v = q.front();
            q.erase(q.begin());
            order.push_back(v);
            for (int u : G.adj[v]) {
                if (--in_degree[u] == 0) {
                    q.push_back(u);
                }
            }
            sort(q.begin(), q.end());
        }

        if (order.size() != G.N) {
            cout << "NO" << endl;
        } else {
            for (int v : order) {
                cout << v << " ";
            }
            cout << endl;
        }
    }
};

int main() {
    int N, M;
    cin >> N >> M;

    Graph G(N, M);

    for (int i = 1; i <= N; i++) {
        cin >> G.hype[i];
    }

    for (int i = 0; i < M; i++) {
        int u, v;
        cin >> u >> v;
        G.addEdge(u, v);
    }

    int Q;
    cin >> Q;

    vector<GraphAlgorithm*> algorithms = {
        new isCycle(G),
        new indepComponent(G),
        new validOrder(G),
        new maxHype(G)
    };

    while (Q--) {
        int query_type;
        cin >> query_type;
        algorithms[query_type - 1]->Query();
    }

    for (auto alg : algorithms) {
        delete alg;
    }

    return 0;
}
