#include <bits/stdc++.h>
using namespace std;

template<class T = int, int B = sizeof(T) * 8>
struct PersistentTrie {
    using UT = typename make_unsigned<T>::type;

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

    int update(int v, UT x, int delta) {
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

    int insert(int v, T x) { return update(v, (UT)x, 1); }
    int erase(int v, T x) { return update(v, (UT)x, -1); }

    int push(T x) {
        root.push_back(insert(root.back(), x));
        return (int)root.size() - 1;
    }

    int count(int a, int b, T xVal) {
        UT x = (UT)xVal;
        for (int i = B - 1; i >= 0; i--) {
            int bit = (x >> i) & 1;
            a = ch[a][bit];
            b = ch[b][bit];
        }
        return cnt[b] - cnt[a];
    }

    int less(int a, int b, T xVal) {
        UT x = (UT)xVal;
        int res = 0;
        for (int i = B - 1; i >= 0; i--) {
            int bit = (x >> i) & 1;
            if (bit) res += cnt[ch[b][0]] - cnt[ch[a][0]];
            a = ch[a][bit];
            b = ch[b][bit];
        }
        return res;
    }

    T kth(int a, int b, int k) {
        UT res = 0;
        for (int i = B - 1; i >= 0; i--) {
            int c = cnt[ch[b][0]] - cnt[ch[a][0]];
            if (k <= c) {
                a = ch[a][0];
                b = ch[b][0];
            } else {
                k -= c;
                res |= (UT)1 << i;
                a = ch[a][1];
                b = ch[b][1];
            }
        }
        return (T)res;
    }

    T maxXor(int a, int b, T xVal) {
        UT x = (UT)xVal;
        UT res = 0;
        for (int i = B - 1; i >= 0; i--) {
            int bit = (x >> i) & 1;
            int w = bit ^ 1;
            if (cnt[ch[b][w]] - cnt[ch[a][w]] > 0) {
                res |= (UT)1 << i;
                a = ch[a][w];
                b = ch[b][w];
            } else {
                a = ch[a][bit];
                b = ch[b][bit];
            }
        }
        return (T)res;
    }

    T minXor(int a, int b, T xVal) {
        UT x = (UT)xVal;
        UT res = 0;
        for (int i = B - 1; i >= 0; i--) {
            int bit = (x >> i) & 1;
            if (cnt[ch[b][bit]] - cnt[ch[a][bit]] > 0) {
                a = ch[a][bit];
                b = ch[b][bit];
            } else {
                res |= (UT)1 << i;
                a = ch[a][bit ^ 1];
                b = ch[b][bit ^ 1];
            }
        }
        return (T)res;
    }
};
