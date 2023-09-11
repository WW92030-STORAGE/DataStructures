#include <bits/stdc++.h>
using namespace std;
#define ll int

vector<int> dijkstra(vector<vector<ll>>& adj, int src) { // adj is a bidirectional adjacency list, src is the start
    // adj[i] is the set of all nodes adjacent to i by weight. 0 or lower denotes no edge.
    int n = adj.size();
    vector<ll> res(n, INT_MAX);
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
        for (int i = 0; i < n; i++) {
            if (adj[node][i] <= 0) continue;
            if (res[i] > res[node] + adj[node][i]) {
                res[i] = res[node] + adj[node][i];
                pq.push({-1 * res[i], i});
            }
        }
        
    }
    return res;
}

int main()
{
    vector<vector<int>> graph { { 0, 4, 0, 0, 0, 0, 0, 8, 0 },
                        { 4, 0, 8, 0, 0, 0, 0, 11, 0 },
                        { 0, 8, 0, 7, 0, 4, 0, 0, 2 },
                        { 0, 0, 7, 0, 9, 14, 0, 0, 0 },
                        { 0, 0, 0, 9, 0, 10, 0, 0, 0 },
                        { 0, 0, 4, 14, 10, 0, 2, 0, 0 },
                        { 0, 0, 0, 0, 0, 2, 0, 1, 6 },
                        { 8, 11, 0, 0, 0, 0, 1, 0, 7 },
                        { 0, 0, 2, 0, 0, 0, 6, 7, 0 } };
    
    // https://www.geeksforgeeks.org/dijkstras-shortest-path-algorithm-greedy-algo-7/
    vector<int> res = dijkstra(graph, 0);
    for (auto i : res) cout << i << endl;

    return 0;
}



