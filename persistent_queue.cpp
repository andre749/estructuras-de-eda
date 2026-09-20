#include <bits/stdc++.h>
using namespace std;

template<class T>
struct PersistentQueue {
    vector<int> par, jmp, dep;
    vector<T> val;
    vector<pair<int, int>> ver;

    PersistentQueue() {
        par.push_back(0);
        jmp.push_back(0);
        dep.push_back(0);
        val.push_back(T());
        ver.push_back({0, 0});
    }

    int newNode(int p, const T& x) {
        int v = (int)par.size();
        par.push_back(p);
        dep.push_back(dep[p] + 1);
        int j = jmp[p];
        if (dep[p] - dep[j] == dep[j] - dep[jmp[j]]) jmp.push_back(jmp[j]);
        else jmp.push_back(p);
        val.push_back(x);
        return v;
    }

    int ancestor(int v, int d) {
        while (dep[v] > d) {
            if (dep[jmp[v]] >= d) v = jmp[v];
            else v = par[v];
        }
        return v;
    }

    int push(int id, const T& x) {
        int tail = newNode(ver[id].first, x);
        ver.push_back({tail, ver[id].second + 1});
        return (int)ver.size() - 1;
    }

    int pop(int id) {
        ver.push_back({ver[id].first, ver[id].second - 1});
        return (int)ver.size() - 1;
    }

    T front(int id) {
        int tail = ver[id].first;
        int sz = ver[id].second;
        return val[ancestor(tail, dep[tail] - sz + 1)];
    }

    T back(int id) { return val[ver[id].first]; }
    int size(int id) { return ver[id].second; }
    bool empty(int id) { return ver[id].second == 0; }
};
