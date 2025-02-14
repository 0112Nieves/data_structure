#include <iostream>
#include <unordered_map>
#include <vector>
#include <unordered_set>
#include <set>
#include <queue>
#include <sstream>
using namespace std;
unordered_map<int, vector<int>> adj_list;

int count_connected_components(unordered_map<int, vector<int>>& adj_list, const set<int>& nodes){
    unordered_set<int> visited;
    int count = 0;

    for(int node : nodes){
        if(visited.find(node) == visited.end()){
            count++;
            queue<int> q;
            q.push(node);
            visited.insert(node);
            while(!q.empty()){
                int current = q.front();
                q.pop();
                for(int neighbor : adj_list[current]){
                    if(visited.find(neighbor) == visited.end()){
                        visited.insert(neighbor);
                        q.push(neighbor);
                    }
                }
            }
        }
    }
    return count;
}

bool has_cycle(int node, int parent, unordered_map<int, vector<int>>& adj_list, unordered_set<int>& visited){
    visited.insert(node);
    for(int i = 0; i < adj_list[node].size(); i++){
        int neighbor = adj_list[node][i];
        if(visited.find(neighbor) == visited.end()){
            if(has_cycle(neighbor, node, adj_list, visited)) return true;
        }
        else if(neighbor != parent){
            return true;
        }
    }
    return false;
}

bool can_form_single_chain(unordered_map<int, vector<int>>& adj_list, const unordered_set<int>& selected) {
    unordered_map<int, vector<int>> subgraph;
    for (int node : selected) {
        for (int neighbor : adj_list[node]) {
            if (selected.find(neighbor) != selected.end()) {
                subgraph[node].push_back(neighbor);
            }
        }
    }

    unordered_set<int> visited_subgraph;
    if (has_cycle(*selected.begin(), -1, subgraph, visited_subgraph)) {
        return false;
    }

    unordered_map<int, int> degree_count;
    for (int node : selected) {
        degree_count[node] = adj_list[node].size();
    }

    int degree_one_count = 0;
    int degree_two_count = 0;

    for (const auto& entry : degree_count) {
        int degree = entry.second;
        if (degree == 1) {
            degree_one_count++;
        } else if (degree == 2) {
            degree_two_count++;
        } else {
            return false;
        }
    }

    return degree_one_count == 2 && degree_two_count == selected.size() - 2;
}

int count_cycles(unordered_map<int, vector<int>>& adj_list, const set<int>& nodes) {
    unordered_set<int> visited;
    int cycle_count = 0;

    for (int node : nodes) {
        if (visited.find(node) == visited.end()) {
            unordered_set<int> local_visited;
            queue<pair<int, int>> q;
            q.push({node, -1});
            visited.insert(node);

            while (!q.empty()) {
                int current = q.front().first;
                int parent = q.front().second;
                q.pop();

                for (int neighbor : adj_list[current]) {
                    if (visited.find(neighbor) == visited.end()) {
                        visited.insert(neighbor);
                        q.push({neighbor, current});
                    } else if (neighbor != parent && local_visited.find(neighbor) == local_visited.end()) {
                        cycle_count++;
                        local_visited.insert(current);
                    }
                }
            }
        }
    }
    return cycle_count / 2;
}

int min_open_rings(unordered_map<int, vector<int>>& adj_list, const set<int>& nodes) {
    int total_edges = 0;

    for (const auto& pair : adj_list) {
        total_edges += pair.second.size();
    }
    total_edges /= 2;

    int components = count_connected_components(adj_list, nodes);
    int total_nodes = nodes.size();
    int cycle_count = count_cycles(adj_list, nodes);

    return cycle_count + (components - 1);
}

int main()
{
    int n, m, a, b;
    while(cin >> n && n){
        adj_list.clear();
        set<int> nodes;
        for(int i = 1; i <= n; i++) nodes.insert(i);
        cin >> m;
        for(int i = 0; i < m; i++){
            cin >> a >> b;
            adj_list[a].push_back(b);
            adj_list[b].push_back(a);
        }
        // 1.
        int result = count_connected_components(adj_list, nodes);
        cout << result << endl;  
        // 2.  
        int min_rings = min_open_rings(adj_list, nodes);
        cout << min_rings << endl;
        // 3.
        printf("Text open sequence:");
        string input;
        cin.ignore();
        getline(cin, input);
        stringstream ss(input);
        unordered_set<int> selected;
        int num;
        while (ss >> num) {
            selected.insert(num);
        }

        if (can_form_single_chain(adj_list, selected)) {
            cout << "Yes" << endl;
        } else {
            cout << "No" << endl;
        }
    }
    
    return 0;
}