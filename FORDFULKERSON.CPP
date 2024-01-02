#include <vector>
#include <iostream>
#include <set>
#include <climits>
#include <queue>
#include <algorithm>

class FlowGraph {
    public:
    std::vector<std::vector<int64_t>> originalgraph; // Flow (residual) graph representing the original graph
    std::vector<std::vector<int64_t>> residualgraph; // Residual graph that is manipulated with forward and backward edges
    
    int64_t n;
    
    FlowGraph() {
        originalgraph = std::vector<std::vector<int64_t>>();
        residualgraph = std::vector<std::vector<int64_t>>();
    }
    
    FlowGraph(std::vector<std::vector<int64_t>> v) {
        originalgraph = v;
        residualgraph = v;
        n = v.size();
    }
    
    void reset() {
        residualgraph = originalgraph;
    }
    
    void reset(std::vector<std::vector<int64_t>> v) {
        originalgraph = v;
        residualgraph = v;
        n = v.size();
    }
    
    int64_t augment(std::vector<int64_t> path) { // Returns the increase in flow upon success and -1 upon failure
        int64_t len = path.size();
        
        int64_t bottleneck = LLONG_MAX;
        
        for (int64_t i = 0; i < len - 1; i++) { // Make sure this is a path with positive weights
            if (residualgraph[path[i]][path[i + 1]] <= 0) return -1;
            bottleneck = std::min(bottleneck, residualgraph[path[i]][path[i + 1]]);
        }
        
        for (int64_t i = 0; i < len - 1; i++) {
            residualgraph[path[i]][path[i + 1]] -= bottleneck;
            residualgraph[path[i + 1]][path[i]] += bottleneck;
        }
        
        return (bottleneck == LLONG_MAX) ? -1 : bottleneck;
    }
    
    std::vector<int64_t> bfs(int64_t src, int64_t snk) { // BFS a path from the src to the snk
        std::vector<int64_t> parent(n, -1);
        
        std::queue<int64_t> q;
        q.push(src);
        
        while (q.size() > 0) {
            int64_t cur = q.front();
            q.pop();
            for (int64_t i = 0; i < n; i++) {
                if (residualgraph[cur][i] > 0) {
                    if (i == src) continue;
                    if (parent[i] >= 0) continue;
                    q.push(i);
                    parent[i] = cur;
                }
            }
        }
        
        if (parent[snk] < 0) return std::vector<int64_t>();
        
        std::vector<int64_t> path(1, snk);
        while (parent[path[path.size() - 1]] >= 0) path.push_back(parent[path[path.size() - 1]]);
        std::reverse(path.begin(), path.end());
        return path;
    }
    
    int64_t edmondkarp(int64_t src, int64_t snk) {
        reset();
        int64_t res = 0;
        while (true) {
            std::vector<int64_t> path = bfs(src, snk);
            if (path.size() == 0) break;
            int64_t bn = augment(path);
            std::cout << bn << "\n";
            res += bn;
        }
        return res;
    }
};



#include <bits/stdc++.h>
using namespace std;

void disp(vector<vector<int64_t>> v) {
    cout << ">>>\n";
    for (auto i : v) {
        cout << "[";
        for (auto j : i) cout << j << " ";
        cout << "]\n";
    }
}

int main()
{
    /* 
    vector<vector<int64_t>> graph{{0, 0, 20, 10}, {0, 0, 0, 0}, {0, 10, 0, 30}, {0, 20, 0, 0}};
    FlowGraph flow(graph);
    
    cout << flow.edmondkarp(0, 1) << endl;
    
    disp(flow.originalgraph);
    disp(flow.residualgraph);
    
    */
    
    // https://www.cses.fi/problemset/task/1694 WARNING - TLE on a hard test case that stretches the (edges) * (highest edge weight) limit!
    
    int64_t n, m;
    cin >> n >> m;
    vector<vector<int64_t>> graph(n, vector<int64_t>(n, 0));
    int64_t a, b, c;
    for (int64_t i = 0; i < m; i++) {
        cin >> a >> b >> c;
        graph[a - 1][b - 1] += c;
    }
    
    FlowGraph flow(graph);
    cout << flow.edmondkarp(0, n - 1);

    return 0;
}
