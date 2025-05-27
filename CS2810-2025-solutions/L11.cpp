#include <bits/stdc++.h>
using namespace std;

typedef long long ll;

struct Point {
    ll x, y;
    Point(ll x = 0, ll y = 0) : x(x), y(y) {}
    friend istream& operator>>(istream& in, Point& p) {
        return in >> p.x >> p.y;
    }
};

ll distSq(ll x1, ll y1, ll x2, ll y2)
{
    return (x2-x1)*(x2-x1) + (y2-y1)*(y2-y1);
}

class Island
{
protected:
    ll centroidx, centroidy, reachSq;
    string id;

public:
    Island() : centroidx(0), centroidy(0), reachSq(0), id("") {}

    virtual void setReach() = 0;
    virtual ~Island() {}
    virtual void read(istream& in) = 0;
    virtual void print(ostream& out) = 0;

    ll getCentroidX() const { 
        return centroidx; 
    }
    ll getCentroidY() const { 
        return centroidy; 
    }
    ll getReachSq() const { 
        return reachSq; 
    }
    string getId() const { 
        return id; 
    }
    friend istream& operator>>(istream& in, Island& island) {
        island.read(in);
        return in;
    }
    friend ostream& operator<<(ostream& out, Island& island) {
        island.print(out);
        return out;
    }
};

class Rectangle : public Island
{
private:
    vector<Point> vertices;

public:
    Rectangle() : vertices(4) {}

    void setReach() override {
        reachSq = distSq(vertices[0].x, vertices[0].y, centroidx, centroidy);
    }
    void print(ostream& out) override {
        out <<  id << " ";
    }
    void read(istream& in) override {
        in >> id;
        for (auto& pt : vertices){
            in >> pt;
        }
        centroidx = centroidy = 0;
        for(int i=0; i<4; i++)
        {
            centroidx += vertices[i].x;
            centroidy += vertices[i].y;
        }
        centroidx /= 4;
        centroidy /= 4;
    }
};

class Triangle : public Island
{
private:
    vector<Point> vertices;

public:
    Triangle() : vertices(3) {}

    void setReach() override {
        reachSq = max(
            distSq(vertices[0].x, vertices[0].y, centroidx, centroidy), 
            max(
                distSq(vertices[1].x, vertices[1].y, centroidx, centroidy), 
                distSq(vertices[2].x, vertices[2].y, centroidx, centroidy)
            )
        );
    }
    void read(istream& in) override {
        in >> id;
        for (auto& pt : vertices){
            in >> pt;
        }
        centroidx = centroidy = 0;
        for(int i=0; i<3; i++){
            centroidx += vertices[i].x;
            centroidy += vertices[i].y;
        }
        centroidx /= 3;
        centroidy /= 3;
    }

    void print(ostream& out) override {
        out << id << " ";
    }
};

class Circle : public Island
{
private:
    ll radius;

public:
    Circle() : radius(0) {}

    void setReach() override {
        reachSq = radius * radius;
    }
    void read(istream& in) override {
        in >> id >> centroidx >> centroidy >> radius;
    }

    void print(ostream& out) override {
        out << id << " ";
    }
};

int main()
{
    int n;
    cin >> n;
    vector<Island*> islands(n);
    for(int i = 0; i < n; i++)
    {
        string shape;
        cin >> shape;

        if(shape == "RECTANGLE")
            islands[i] = new Rectangle();
        else if(shape == "TRIANGLE")
            islands[i] = new Triangle();
        else
            islands[i] = new Circle();

        cin >> *islands[i];
        islands[i]->setReach();
    }

    vector<vector<ll>> adj(n, vector<ll>(n, 0));

    for(ll i = 0; i < n; i++)
    {
        for(ll j = 0; j < n; j++)
        {
            if(i == j) continue;
            long double r1 = sqrtl(islands[i]->getReachSq());
            long double r2 = sqrtl(islands[j]->getReachSq());
            long double reachSumSq = 2*r1*r2 + islands[i]->getReachSq() + islands[j]->getReachSq();
            ll dist = distSq(islands[i]->getCentroidX(), islands[i]->getCentroidY(), islands[j]->getCentroidX(), islands[j]->getCentroidY());

            if (reachSumSq >= dist)
            {
                adj[i][j] = 1;
                adj[j][i] = 1;
            }
        }
    }

    vector<vector<bool>> dp(1<<n, vector<bool>(n, 0));
    vector<vector<ll>> parent(1<<n, vector<ll>(n, -1));
    ll longest_mask = 1;

    for(ll i = 0; i < n; i++)
        dp[1<<i][i] = 1;

    for(ll mask = 0; mask < (1<<n); mask++)
    {
        for(ll u = 0; u < n; u++)
        {
            if(mask & (1<<u))
            {
                for(ll v = 0; v < n; v++)
                {
                    if(u != v && (mask & (1<<v)) && adj[v][u] && dp[mask ^ (1<<u)][v])
                    {
                        dp[mask][u] = true;
                        parent[mask][u] = v;
                        if (__builtin_popcount(mask) > __builtin_popcount(longest_mask))
                        {
                            longest_mask = mask;
                        }
                    }
                }
            }
        }
    }

    if(__builtin_popcount(longest_mask) == n)
    {
        cout << "YES" << endl;
    }
    else
    {
        cout << "NO" << endl;
        cout << __builtin_popcount(longest_mask) << endl;
    }
    ll end = -1;
    for(ll i = 0; i < n; i++)
    {
        if(dp[longest_mask][i])
        {
            end = i;
            break;
        }
    }

    vector<string> path;
    while(end != -1)
    {
        path.push_back(islands[end]->getId());
        ll p = parent[longest_mask][end];
        longest_mask ^= (1 << end);
        end = p;
    }
    reverse(path.begin(), path.end());

    for(const auto& id : path)
        cout << id << " ";
    cout << endl;

    for(int i=0; i<n; i++)
    {
        delete islands[i];
    }    
    return 0;
}
