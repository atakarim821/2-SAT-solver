#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <queue>
#include <set>
#include <unordered_map>
using namespace std;

void dfs(int i, vector<vector<int>>& Radj, vector<pair<int, int>>& post, vector<bool>& visited, int& clock)
{
    visited[i] = true;
    clock++;
    for(auto ele : Radj[i])
    {
        if(!visited[ele]) dfs(ele, Radj, post, visited, clock);
    }
    post.push_back({clock, i});
    clock++;
}

void explore(int node, vector<int>& who, vector<bool>& visit, vector<vector<int>>& adj, int v, map<int,vector<int>>& hashmap)
{
    queue<int>  q;
    q.push(node);
    visit[node] = true;
    while (!q.empty())
    {
        int k = q.size();
        for(int i=0; i<k ;i++)
        {
            int temp = q.front();
            who[temp] = node;
            q.pop();
            hashmap[node].push_back(temp);
            for(auto ele : adj[temp])
            {
                if(!visit[ele])
                {
                    visit[ele] = true;
                    q.push(ele);
                }
            }
        }
    }
}

int main(){
    ios::sync_with_stdio(false);
    int v,c;
    cin >> v >> c;
    map<int , int> myMap;
    for(int i=-v; i<0; i++)
    {
        myMap[i] = abs(i) + v;
    }
    for(int i=1; i<=v; i++)
    {
        myMap[i] = i;
    }
    vector<vector<int>> adj(2*v+1);
    vector<vector<int>> Radj(2*v+1);

    // Here I have completed the implication graph.
    // O(c)
    for(int i=0; i<c; i++)
    {
        int l1, l2; 
        cin >> l1 >> l2;
        adj[myMap[-1*l2]].push_back(myMap[l1]);
        adj[myMap[-1*l1]].push_back(myMap[l2]);

        Radj[myMap[l1]].push_back(myMap[-1*l2]);
        Radj[myMap[l2]].push_back(myMap[-1*l1]);
    }
    // Chcek for SCCs.
    vector<pair<int, int>> post;
    vector<bool> visited(2*v+1, false);
    int clock = 0;
    // O(V+C)
    for(int i=1; i<=2*v; i++)
    {
        if(!visited[i])
        {
            dfs(i, Radj, post, visited, clock);
        }
    }

    sort(post.begin(), post.end());
    reverse(post.begin(), post.end());
    vector<bool> visit(2*v+1, false);
    vector<int> who(2*v+1);
    vector<int> toposort;
    map<int, vector<int>> hashmap;
    for(auto p : post)
    {
        if(!visit[p.second])
        {
            who[p.second] = p.second;
            toposort.push_back(p.second);
            explore(p.second, who, visit, adj, v, hashmap);
        }
    }
    for(int i=1; i<=v; i++)
    {
        if(who[i]==who[i+v]) {cout << "UNSATISFIABLE\n"; return 0;}
    }

    // If program reaches here it means formula is satisfiable.
    cout << "SATISFIABLE\n";

    vector<bool> assign(2*v+1, false);
    vector<pair<int, int>> ans;
    for(int i=0; i<toposort.size(); i++)
    {
        if(!assign[toposort[i]])
        {
            for(auto ele : hashmap[toposort[i]])
            {
                int k;
                if(ele>v) {k = ele-v; ans.push_back({k, -1*k});}
                else {k = ele + v; ans.push_back({ele, ele});}
                assign[ele] = true;
                assign[k] = true;
            }
        }
    }
    sort(ans.begin(), ans.end()); 
    for(auto ele : ans) cout << ele.second << " ";
    return 0;
}
