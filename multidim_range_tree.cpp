#include <bits/stdc++.h>
using namespace std;

template<int D>
struct RangeTreeND {
    typedef long long ll;
    typedef array<int, D> P;
    typedef vector<pair<P, ll>> V;

    struct Tree {
        vector<int> key;
        vector<ll> pre;
        vector<Tree> seg;
    };

    V pts;
    Tree root;

    void add(const P& p, ll w = 1) { pts.push_back({p, w}); }

    void build() { build(root, pts, 0); }

    void build(Tree& t, V& p, int d) {
        sort(p.begin(), p.end(), [&](const pair<P, ll>& a, const pair<P, ll>& b) {
            return a.first[d] < b.first[d];
        });
        int n = (int)p.size();
        t.key.resize(n);
        for (int i = 0; i < n; i++) t.key[i] = p[i].first[d];
        if (d == D - 1) {
            t.pre.assign(n + 1, 0);
            for (int i = 0; i < n; i++) t.pre[i + 1] = t.pre[i] + p[i].second;
            return;
        }
        if (n == 0) return;
        t.seg.resize(2 * n - 1);
        buildSeg(t, 0, 0, n, p, d);
    }

    void buildSeg(Tree& t, int v, int l, int r, V& p, int d) {
        V sub(p.begin() + l, p.begin() + r);
        build(t.seg[v], sub, d + 1);
        if (r - l == 1) return;
        int m = (l + r) / 2;
        buildSeg(t, v + 1, l, m, p, d);
        buildSeg(t, v + 2 * (m - l), m, r, p, d);
    }

    ll go(const Tree& t, int v, int nl, int nr, int l, int r, int d, const P& lo, const P& hi) const {
        if (r <= nl || nr <= l) return 0;
        if (l <= nl && nr <= r) return query(t.seg[v], d, lo, hi);
        int m = (nl + nr) / 2;
        return go(t, v + 1, nl, m, l, r, d, lo, hi) +
               go(t, v + 2 * (m - nl), m, nr, l, r, d, lo, hi);
    }

    ll query(const Tree& t, int d, const P& lo, const P& hi) const {
        int l = lower_bound(t.key.begin(), t.key.end(), lo[d]) - t.key.begin();
        int r = upper_bound(t.key.begin(), t.key.end(), hi[d]) - t.key.begin();
        if (l >= r) return 0;
        if (d == D - 1) return t.pre[r] - t.pre[l];
        return go(t, 0, 0, (int)t.key.size(), l, r, d + 1, lo, hi);
    }

    ll query(const P& lo, const P& hi) const { return query(root, 0, lo, hi); }
};
