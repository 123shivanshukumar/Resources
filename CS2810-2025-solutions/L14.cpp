#include <bits/stdc++.h>
#include <chrono>

using namespace std;

class Node
{
    private:
        string ID;
        int distance;
        
    public:
    vector<pair<string, int>> neighbours;
    Node(): ID(" "), distance(-1){}
    Node(string name): ID(name), distance(-1) {}

    void addNeighbour(string name, int weight)
    {
        neighbours.push_back(make_pair(name, weight));
    }

    const vector<pair<string, int>>& getNeighbours() const
    {
        return neighbours;
    }

    void setDistance(int newDist)
    {
        distance = newDist;
    }

    int getDistance() const
    {
        return distance;
    }

    friend bool operator>(const Node& l, const Node& r)
    {
        return (l.getDistance() == -1 && r.getDistance() >= -1) ||  (l.getDistance() > r.getDistance());
    }

    friend bool operator<(const Node& l, const Node& r)
    {
        return r > l;
    }

    friend bool operator==(const Node& l, const Node& r)
    {
        return !(l < r) && !(r < l); 
    }

    string getID() const
    {
        return ID;
    }

    friend ostream& operator<<(ostream& os, const Node& obj)
    {
        cout << obj.getID() << " " << obj.getDistance() << endl;
        return os;
    }
};

template<typename T>
class MinPriorityQueue
{
    private:
        vector<T> _container;
        size_t _size;

        void siftUp()
        {
            int _index = _size - 1;
            while(_index > 0)
            {
                int _parent = (_index-1)/2;
                if(!(_container[_parent] > _container[_index]))
                {
                    break;
                }
                swap(_container[_parent], _container[_index]);
                _index = _parent;
            }
        }

        void siftDown()
        {
            int _index = 0;
            while(2*_index + 1 < _size)
            {
                if(2*_index + 2 >= _size)
                {
                    if(_container[_index] > _container[2*_index + 1])
                    {
                        swap(_container[_index], _container[2*_index + 1]);
                        _index = 2 * _index + 1;
                    }
                    break;
                }
                int smallerChild = (_container[2*_index + 1] < _container[2*_index+2])? (2*_index + 1) : (2*_index + 2);
                if(_container[_index] > _container[smallerChild])
                {
                    swap(_container[_index], _container[smallerChild]);
                    _index = smallerChild;
                }
                else
                {
                    break;
                }
            }
        }
    
    public:
        MinPriorityQueue():_size(0){ }
        MinPriorityQueue(vector<T> &vec):_size(vec.size()), _container(vec) { }

        const size_t size() const
        {
            return _size;
        }

        const bool empty() const
        {
            return _size == 0;
        }

        void push(const T& obj)
        {
            _container.push_back(obj);
            _size++;
            this->siftUp();
        }

        const T& top() const
        {
            // Ideally you should check here for non-zero size. For the sake of uploading on moodle, assert statements are removed
            return _container[0];
        }

        void pop()
        {
            // Ideally you should check here for non-zero size. For the sake of uploading on moodle, assert statements are removed
            swap(_container[0], _container[_size - 1]);
            _container.pop_back();
            _size--;
            this->siftDown();
        }

};

int main( )
{
    // auto start = std::chrono::high_resolution_clock::now();
    int n, m;
    cin >> n >> m;
    string inp;
    string u, v;
    int w;
    map<string, Node*> mappings;

    for(int i=0; i<n; i++)
    {
        cin >> inp;
        string inpeven = inp  + "$even";
        string inpodd = inp + "$odd";
        mappings.insert({inpeven, new Node(inpeven)});
        mappings.insert({inpodd, new Node(inpodd)});
    }
    for(int i=0; i<m; i++)
    {
        cin >> u >> v >> w;
        string u1 = u + "$even";
        string u2 = u + "$odd";
        
        string v1 = v + "$even";
        string v2 = v + "$odd";
        
        mappings[u1]->addNeighbour(v2, w);
        mappings[u2]->addNeighbour(v1, w);
        
        mappings[v1]->addNeighbour(u2, w);
        mappings[v2]->addNeighbour(u1, w);
    }

    map<string, bool> visited;
    string src, sink;
    cin >> src >> sink;
    string srcEven = src + "$even";
    string sinkEven = sink + "$even";
    mappings[srcEven]->setDistance(0);
    
    MinPriorityQueue<Node> pq;
    pq.push(*mappings[srcEven]);
    while(!pq.empty())
    {
        auto top = pq.top();
        pq.pop();
        if(top.getDistance() > mappings[top.getID()]->getDistance() && mappings[top.getID()]->getDistance()!=-1) continue;

        for(auto &e : top.neighbours)
        {
            if((*mappings[e.first]).getDistance() == -1 || (*mappings[e.first]).getDistance() > top.getDistance() + e.second)
            {
                (*mappings[e.first]).setDistance(top.getDistance() + e.second);
                pq.push((*mappings[e.first]));
            }
        }
    }

    cout << (*mappings[sinkEven]).getDistance() << endl;
    for(auto &it : mappings)
    {
        delete it.second;
    }

    // auto stop = std::chrono::high_resolution_clock::now();
    // auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
    
    // cerr << "Execution time: " << duration.count()/1000 << " ms" << endl;
    return 0;
}