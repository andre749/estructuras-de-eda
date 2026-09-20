#include <bits/stdc++.h>
using namespace std;

template<int B = 30>
struct PersistentTrie {
    vector<array<int, 2>> ch;
    vector<int> cnt;
    vector<int> root;

    PersistentTrie() {
        ch.push_back({0, 0});
        cnt.push_back(0);
        root.push_back(0);
    }

    int clone(int v) {
        array<int, 2> c = ch[v];
        int k = cnt[v];
        ch.push_back(c);
        cnt.push_back(k);
        return (int)ch.size() - 1;
    }

    int update(int v, int x, int delta) {
        int r = clone(v);
        int cur = r;
        cnt[cur] += delta;
        for (int i = B - 1; i >= 0; i--) {
            int b = (x >> i) & 1;
            int nx = clone(ch[cur][b]);
            ch[cur][b] = nx;
            cur = nx;
            cnt[cur] += delta;
        }
        return r;
    }

    int insert(int v, int x) { return update(v, x, 1); }
    int erase(int v, int x) { return update(v, x, -1); }

    int push(int x) {
        root.push_back(insert(root.back(), x));
        return (int)root.size() - 1;
    }

    int count(int a, int b, int x) {
        for (int i = B - 1; i >= 0; i--) {
            int bit = (x >> i) & 1;
            a = ch[a][bit];
            b = ch[b][bit];
        }
        return cnt[b] - cnt[a];
    }

    int less(int a, int b, int x) {
        int res = 0;
        for (int i = B - 1; i >= 0; i--) {
            int bit = (x >> i) & 1;
            if (bit) res += cnt[ch[b][0]] - cnt[ch[a][0]];
            a = ch[a][bit];
            b = ch[b][bit];
        }
        return res;
    }

    int kth(int a, int b, int k) {
        int res = 0;
        for (int i = B - 1; i >= 0; i--) {
            int c = cnt[ch[b][0]] - cnt[ch[a][0]];
            if (k <= c) {
                a = ch[a][0];
                b = ch[b][0];
            } else {
                k -= c;
                res |= 1 << i;
                a = ch[a][1];
                b = ch[b][1];
            }
        }
        return res;
    }

    int maxXor(int a, int b, int x) {
        int res = 0;
        for (int i = B - 1; i >= 0; i--) {
            int bit = (x >> i) & 1;
            int w = bit ^ 1;
            if (cnt[ch[b][w]] - cnt[ch[a][w]] > 0) {
                res |= 1 << i;
                a = ch[a][w];
                b = ch[b][w];
            } else {
                a = ch[a][bit];
                b = ch[b][bit];
            }
        }
        return res;
    }

    int minXor(int a, int b, int x) {
        int res = 0;
        for (int i = B - 1; i >= 0; i--) {
            int bit = (x >> i) & 1;
            if (cnt[ch[b][bit]] - cnt[ch[a][bit]] > 0) {
                a = ch[a][bit];
                b = ch[b][bit];
            } else {
                res |= 1 << i;
                a = ch[a][bit ^ 1];
                b = ch[b][bit ^ 1];
            }
        }
        return res;
    }
};
