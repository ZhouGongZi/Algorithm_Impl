#include <iostream>
#include <vector>
#include <unordered_set>
#include <limits.h>
using namespace std;

int findMinDist(vector<int>& dist, unordered_set<int>& vis) {
	int idx = -1;
	int minDist = INT_MAX;
	for (int i = 0; i < dist.size(); ++i) {
		if (!vis.count(i) && dist[i] < minDist) {
			idx = i;
			minDist = dist[i];
		}
	}
	return idx;
}

vector<int> dijsktra(vector<vector<int>>& graph, int src) {
	int n = graph.size();

	vector<int> dist(n, INT_MAX);
	unordered_set<int> vis;
	dist[src] = 0;
	

	for (int i = 0; i < n - 1; ++i) {
		int mi = findMinDist(dist, vis);
		vis.insert(mi);

		for (int j = 0; j < n; ++j) {
			if (vis.count(j)) continue;
			if (graph[mi][j] == 0) continue;
			if (dist[j] > dist[mi] + graph[mi][j]) {
				dist[j] = dist[mi] + graph[mi][j];
			}
		}
	}
	return dist;
}

int main() {
	vector<vector<int>> graph = { { 0, 4, 0, 0, 0, 0, 0, 8, 0 },
                        { 4, 0, 8, 0, 0, 0, 0, 11, 0 },
                        { 0, 8, 0, 7, 0, 4, 0, 0, 2 },
                        { 0, 0, 7, 0, 9, 14, 0, 0, 0 },
                        { 0, 0, 0, 9, 0, 10, 0, 0, 0 },
                        { 0, 0, 4, 14, 10, 0, 2, 0, 0 },
                        { 0, 0, 0, 0, 0, 2, 0, 1, 6 },
                        { 8, 11, 0, 0, 0, 0, 1, 0, 7 },
                        { 0, 0, 2, 0, 0, 0, 6, 7, 0 } };
    vector<int> res = dijsktra(graph, 0);
    for (int i : res) cout << i << endl;
	return 0;
}