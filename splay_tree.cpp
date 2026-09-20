#include <bits/stdc++.h>
using namespace std;

template<class T>
struct SplayTree {
    vector<array<int, 2>> c;
    vector<int> p, sz, fr;
    vector<T> key;
    int root;

    SplayTree() : root(0) {
        c.push_back({0, 0});
        p.push_back(0);
        sz.push_back(0);
        key.push_back(T());
    }

    int newNode(const T& k) {
        int v;
        if (!fr.empty()) {
            v = fr.back();
            fr.pop_back();
            c[v] = {0, 0};
            p[v] = 0;
            sz[v] = 1;
            key[v] = k;
        } else {
            v = (int)c.size();
            c.push_back({0, 0});
            p.push_back(0);
            sz.push_back(1);
            key.push_back(k);
        }
        return v;
    }

    void pull(int x) { sz[x] = sz[c[x][0]] + sz[c[x][1]] + 1; }
    int dir(int x) { return c[p[x]][1] == x; }

    void rotate(int x) {
        int y = p[x], z = p[y], d = dir(x);
        int dy = z ? dir(y) : 0;
        int b = c[x][d ^ 1];
        c[y][d] = b;
        if (b) p[b] = y;
        c[x][d ^ 1] = y;
        p[y] = x;
        p[x] = z;
        if (z) c[z][dy] = x;
        pull(y);
        pull(x);
    }

    void splay(int x, int goal = 0) {
        while (p[x] != goal) {
            int y = p[x], z = p[y];
            if (z != goal) rotate(dir(x) == dir(y) ? y : x);
            rotate(x);
        }
        if (!goal) root = x;
    }

    int size() const { return sz[root]; }
    bool empty() const { return root == 0; }

    int insert(const T& k) {
        int v = newNode(k);
        if (!root) {
            root = v;
            return v;
        }
        int cur = root;
        while (true) {
            sz[cur]++;
            int d = (k < key[cur]) ? 0 : 1;
            if (!c[cur][d]) {
                c[cur][d] = v;
                p[v] = cur;
                break;
            }
            cur = c[cur][d];
        }
        splay(v);
        return v;
    }

    int find(const T& k) {
        int cur = root, last = 0;
        while (cur) {
            last = cur;
            if (k < key[cur]) cur = c[cur][0];
            else if (key[cur] < k) cur = c[cur][1];
            else {
                splay(cur);
                return cur;
            }
        }
        if (last) splay(last);
        return 0;
    }

    int lower_bound(const T& k) {
        int cur = root, last = 0, res = 0;
        while (cur) {
            last = cur;
            if (key[cur] < k) cur = c[cur][1];
            else {
                res = cur;
                cur = c[cur][0];
            }
        }
        if (last) splay(last);
        return res;
    }

    int upper_bound(const T& k) {
        int cur = root, last = 0, res = 0;
        while (cur) {
            last = cur;
            if (k < key[cur]) {
                res = cur;
                cur = c[cur][0];
            } else cur = c[cur][1];
        }
        if (last) splay(last);
        return res;
    }

    int order_of_key(const T& k) {
        int cur = root, last = 0, r = 0;
        while (cur) {
            last = cur;
            if (key[cur] < k) {
                r += sz[c[cur][0]] + 1;
                cur = c[cur][1];
            } else cur = c[cur][0];
        }
        if (last) splay(last);
        return r;
    }

    T kth(int k) {
        int cur = root;
        while (true) {
            int ls = sz[c[cur][0]];
            if (k < ls) cur = c[cur][0];
            else if (k == ls) break;
            else {
                k -= ls + 1;
                cur = c[cur][1];
            }
        }
        splay(cur);
        return key[cur];
    }

    void eraseNode(int x) {
        splay(x);
        int l = c[x][0], r = c[x][1];
        p[l] = 0;
        p[r] = 0;
        fr.push_back(x);
        if (!l) {
            root = r;
            return;
        }
        int m = l;
        while (c[m][1]) m = c[m][1];
        splay(m);
        c[m][1] = r;
        if (r) p[r] = m;
        pull(m);
    }

    bool erase(const T& k) {
        int x = find(k);
        if (!x) return false;
        eraseNode(x);
        return true;
    }
};
