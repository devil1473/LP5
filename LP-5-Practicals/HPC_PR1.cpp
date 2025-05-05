// Practical 1: Design and implement Parallel Breadth First Search and Depth First Search based on existing algorithms using OpenMP. Use a Tree or an undirected graph for BFS and DFS .  
// Sequential
#include <iostream>
#include <queue>
#include <vector>
#include <omp.h>
using namespace std;

class Graph {
private:
    int V;
    vector<int>* adj;

public:
    Graph(int V) {
        this->V = V;
        adj = new vector<int>[V];
    }

    void addEdge(int v, int w) {
        adj[v].push_back(w);
        adj[w].push_back(v);
    }--
    void bfs(int s) {
        vector<bool> visit(V, false);
        queue<int> q;

        visit[s] = true;
        q.push(s);

        while (!q.empty()) {
            int u = q.front();
            q.pop();
            cout << u << " ";

            for (int i = 0; i < adj[u].size(); i++) {
                if (!visit[adj[u][i]]) {
                    visit[adj[u][i]] = true;
                    q.push(adj[u][i]);
                }
            }
        }
    }

    void dfs(int s) {
        vector<bool> vis(V, false);
        dfs_helper(s, vis);
    }

private:
    void dfs_helper(int u, vector<bool>& visited) {
        visited[u] = true;
        cout << u << " ";

        // Process neighbors serially (not parallel)
        for (int i = 0; i < adj[u].size(); i++) {
            if (!visited[adj[u][i]]) {
                dfs_helper(adj[u][i], visited);
            }
        }
    }
};

int main() {
    Graph g(6);
    g.addEdge(0, 1);
    g.addEdge(0, 2);
    g.addEdge(1, 3);
    g.addEdge(2, 4);
    g.addEdge(3, 4);
    g.addEdge(3, 5);

    cout << "BFS starting from vertex 3: ";
    g.bfs(3);
    cout << endl;

    cout << "DFS starting from vertex 5: ";
    g.dfs(5);
    cout << endl;

    return 0;
}

// Parallel BFS and DFS

// Practical 1: Parallel BFS and DFS using OpenMP

// #include <iostream>
// #include <queue>
// #include <vector>
// #include <omp.h>
// using namespace std;

// class Graph {
// private:
//     int V;
//     vector<int>* adj;

// public:
//     Graph(int V) {
//         this->V = V;
//         adj = new vector<int>[V];
//     }

//     void addEdge(int v, int w) {
//         adj[v].push_back(w);
//         adj[w].push_back(v); // Undirected graph
//     }

//     // Parallel BFS
//     void parallelBFS(int start) {
//         vector<bool> visited(V, false);
//         queue<int> q;

//         visited[start] = true;
//         q.push(start);

//         cout << "Parallel BFS starting from vertex " << start << ": ";

//         while (!q.empty()) {
//             int level_size = q.size();

//             // Temporary queue for this level
//             vector<int> currentLevel;

//             // Extract current level nodes
//             for (int i = 0; i < level_size; i++) {
//                 int u = q.front();
//                 q.pop();
//                 currentLevel.push_back(u);
//                 cout << u << " ";
//             }

//             // Parallel processing of neighbors
//             #pragma omp parallel for
//             for (int i = 0; i < currentLevel.size(); i++) {
//                 int u = currentLevel[i];
//                 for (int j = 0; j < adj[u].size(); j++) {
//                     int neighbor = adj[u][j];

//                     // Critical section to avoid race conditions
//                     #pragma omp critical
//                     {
//                         if (!visited[neighbor]) {
//                             visited[neighbor] = true;
//                             q.push(neighbor);
//                         }
//                     }
//                 }
//             }
//         }

//         cout << endl;
//     }

//     // Parallel DFS (basic parallelism at neighbor level)
//     void parallelDFS(int start) {
//         vector<bool> visited(V, false);
//         cout << "Parallel DFS starting from vertex " << start << ": ";
//         parallelDFSUtil(start, visited);
//         cout << endl;
//     }

// private:
//     void parallelDFSUtil(int u, vector<bool>& visited) {
//         // Mark visited
//         visited[u] = true;
//         cout << u << " ";

//         // Parallelize visiting neighbors
//         #pragma omp parallel for
//         for (int i = 0; i < adj[u].size(); i++) {
//             int neighbor = adj[u][i];

//             // Synchronize access to shared `visited` array
//             bool shouldVisit = false;
//             #pragma omp critical
//             {
//                 if (!visited[neighbor]) {
//                     visited[neighbor] = true;
//                     shouldVisit = true;
//                 }
//             }

//             if (shouldVisit) {
//                 parallelDFSUtil(neighbor, visited); // Recursive DFS
//             }
//         }
//     }
// };

// int main() {
//     Graph g(6);
//     g.addEdge(0, 1);
//     g.addEdge(0, 2);
//     g.addEdge(1, 3);
//     g.addEdge(2, 4);
//     g.addEdge(3, 4);
//     g.addEdge(3, 5);

//     g.parallelBFS(0);
//     g.parallelDFS(0);

//     return 0;
// }
