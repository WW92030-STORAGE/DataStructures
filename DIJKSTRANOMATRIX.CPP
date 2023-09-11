#include <bits/stdc++.h>
using namespace std;

#define ll long long
#define MAX_VAL LLONG_MAX

vector<ll> dijkstra(vector<vector<pair<int, int>>>& adj, int src) { // adj is a bidirectional adjacency list, src is the start
    // adj[i] is the set of all nodes adjacent to i by weight. (A, B) denotes a path to node A with weight B.
    int n = adj.size();
    vector<ll> res(n, MAX_VAL);
    res[src] = 0;
    vector<bool> vis(n, false);
    priority_queue<pair<ll, int>> pq; // (weight, node)
    pq.push({0, src});
    while (pq.size() > 0) {
        pair<ll, int> p = pq.top();
        pq.pop();
        
        int node = p.second;
        if (vis[node]) continue;
        vis[node] = true;
        for (auto i : adj[node]) {
            if (res[i.first] > res[node] + i.second) {
                res[i.first] = res[node] + i.second;
                pq.push({-1 * res[i.first], i.first});
            }
        }
        
    }
    return res;
}

int main()
{
    // https://www.cses.fi/problemset/task/1671/
    
    int n, p;
    cin >> n >> p;
    vector<vector<pair<int, int>>> graph(n, vector<pair<int, int>>());
    
    int s, e, c;
    for (int i = 0; i < p; i++) {
        cin >> s >> e >> c;
        graph[s - 1].push_back({e - 1, c});
    }
    
    vector<ll> res = dijkstra(graph, 0);
    for (auto i : res) cout << i << " ";


    return 0;
}





