#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <algorithm>
using namespace std;

enum Rank { SENAPATI = 3, DANDANAYAKA = 2, CHATURANGINI = 1 };

class Sentinel;

class RankManager {
private:
    int rankCount[4] = {0};  // Counts for each rank: 0, 1, 2, 3

public:
    void increaseRankCount(Rank rank) {
        rankCount[rank]++;
    }

    int getRankCount(Rank rank) const {
        return rankCount[rank];
    }

    void prepareHigherRankInfo(int& countHigherSenapati, int& countHigherDandanayaka, int& countHigherChaturangini) const {
        countHigherSenapati = 0;
        countHigherDandanayaka = rankCount[SENAPATI];
        countHigherChaturangini = rankCount[SENAPATI] + rankCount[DANDANAYAKA];
    }
};

class Sentinel {
public:
    int id;
    string name;
    Rank rank;

    Sentinel(int _id, string _name, string _rank) : id(_id), name(_name) {
        if (_rank == "SENAPATI") rank = SENAPATI;
        else if (_rank == "DANDANAYAKA") rank = DANDANAYAKA;
        else rank = CHATURANGINI;
    }

    virtual int countHigherRanked(const RankManager& manager) const = 0;
    virtual ~Sentinel() {}
};

class Senapati : public Sentinel {
public:
    Senapati(int id, string name) : Sentinel(id, name, "SENAPATI") {}

    int countHigherRanked(const RankManager& manager) const override {
        int countHigherSenapati, countHigherDandanayaka, countHigherChaturangini;
        manager.prepareHigherRankInfo(countHigherSenapati, countHigherDandanayaka, countHigherChaturangini);
        return countHigherSenapati;
    }
};

class Dandanayaka : public Sentinel {
public:
    Dandanayaka(int id, string name) : Sentinel(id, name, "DANDANAYAKA") {}

    int countHigherRanked(const RankManager& manager) const override {
        int countHigherSenapati, countHigherDandanayaka, countHigherChaturangini;
        manager.prepareHigherRankInfo(countHigherSenapati, countHigherDandanayaka, countHigherChaturangini);
        return countHigherDandanayaka;
    }
};

class Chaturangini : public Sentinel {
public:
    Chaturangini(int id, string name) : Sentinel(id, name, "CHATURANGINI") {}

    int countHigherRanked(const RankManager& manager) const override {
        int countHigherSenapati, countHigherDandanayaka, countHigherChaturangini;
        manager.prepareHigherRankInfo(countHigherSenapati, countHigherDandanayaka, countHigherChaturangini);
        return countHigherChaturangini;
    }
};

struct RankComparator {
    bool operator()(Sentinel* a, Sentinel* b) const {
        if (a->rank != b->rank) return a->rank > b->rank;
        return a->id < b->id;
    }
};

template<typename NodeType>
class Kingdom {
    int n;
    vector<vector<int>> adj;
    vector<NodeType*> sentinels;
    unordered_map<int, NodeType*> idToSentinel;
    RankManager rankManager;

    vector<vector<int>> dp;
    vector<bool> visited;

public:
    Kingdom(int _n) : n(_n) {
        adj.resize(n);
        dp.resize(n, vector<int>(2, -1));
        visited.resize(n, false);
    }

    void addEdge(int u, int v) {
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    void addSentinel(NodeType* s) {
        sentinels.push_back(s);
        idToSentinel[s->id] = s;
        rankManager.increaseRankCount(s->rank);
    }

    int vertexCover(int node, int take) {
        if (dp[node][take] != -1) return dp[node][take];
        visited[node] = true;

        int sum = 0;
        for (int child : adj[node]) {
            if (!visited[child]) {
                if (take == 0)
                    sum += vertexCover(child, 1);
                else
                    sum += min(vertexCover(child, 0), vertexCover(child, 1));
            }
        }

        visited[node] = false;
        return dp[node][take] = sum + take;
    }

    int minSentinelsRequired() {
        fill(dp.begin(), dp.end(), vector<int>(2, -1));
        fill(visited.begin(), visited.end(), false);
        return min(vertexCover(0, 0), vertexCover(0, 1));
    }

    void sortSentinels() {
        sort(sentinels.begin(), sentinels.end(), RankComparator());
    }

    void printSortedIDs() {
        for (auto s : sentinels)
            cout << s->id << " ";
        cout << "\n";
    }

    int countHigher(int id) {
        return idToSentinel[id]->countHigherRanked(rankManager);
    }

    ~Kingdom() {
        for (auto s : sentinels)
            delete s;
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    Kingdom<Sentinel> empire(n);

    for (int i = 0; i < n - 1; ++i) {
        int u, v;
        cin >> u >> v;
        empire.addEdge(u, v);
    }

    for (int i = 0; i < n; ++i) {
        string name, rank;
        cin >> name >> rank;
        Sentinel* s = nullptr;
        if (rank == "SENAPATI") s = new Senapati(i, name);
        else if (rank == "DANDANAYAKA") s = new Dandanayaka(i, name);
        else s = new Chaturangini(i, name);
        empire.addSentinel(s);
    }

    int q;
    cin >> q;
    while (q--) {
        int type;
        cin >> type;
        if (type == 1) {
            cout << empire.minSentinelsRequired() << "\n";
        } else if (type == 2) {
            empire.sortSentinels();
            empire.printSortedIDs();
        } else if (type == 3) {
            int id;
            cin >> id;
            cout << empire.countHigher(id) << "\n";
        }
    }

    return 0;
}
