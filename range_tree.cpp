#include <bits/stdc++.h>
using namespace std;

struct RangeTree {
    typedef long long ll;
    int n;
    vector<int> px, py;
    vector<ll> pw;
    vector<int> xs, yv, rootY;
    vector<ll> wv;
    vector<vector<int>> lp;
    vector<vector<ll>> ps;

    void add(int x, int y, ll w = 1) {
        px.push_back(x);
        py.push_back(y);
        pw.push_back(w);
    }

    void build() {
        n = (int)px.size();
        if (n == 0) return;
        vector<int> id(n);
        iota(id.begin(), id.end(), 0);
        sort(id.begin(), id.end(), [&](int a, int b) {
            if (px[a] != px[b]) return px[a] < px[b];
            return py[a] < py[b];
        });
        xs.resize(n);
        yv.resize(n);
        wv.resize(n);
        for (int i = 0; i < n; i++) {
            xs[i] = px[id[i]];
            yv[i] = py[id[i]];
            wv[i] = pw[id[i]];
        }
        vector<int> ord(n);
        iota(ord.begin(), ord.end(), 0);
        sort(ord.begin(), ord.end(), [&](int a, int b) {
            if (yv[a] != yv[b]) return yv[a] < yv[b];
            return a < b;
        });
        rootY.resize(n);
        for (int i = 0; i < n; i++) rootY[i] = yv[ord[i]];
        lp.assign(4 * n, {});
        ps.assign(4 * n, {});
        build(1, 0, n, ord);
    }

    void build(int v, int l, int r, vector<int>& ord) {
        int len = (int)ord.size();
        lp[v].assign(len + 1, 0);
        ps[v].assign(len + 1, 0);
        int mid = (l + r) / 2;
        vector<int> L, R;
        for (int i = 0; i < len; i++) {
            ps[v][i + 1] = ps[v][i] + wv[ord[i]];
            bool left = ord[i] < mid;
            lp[v][i + 1] = lp[v][i] + left;
            if (r - l > 1) (left ? L : R).push_back(ord[i]);
        }
        if (r - l == 1) return;
        build(2 * v, l, mid, L);
        build(2 * v + 1, mid, r, R);
    }

    void go(int v, int l, int r, int ql, int qr, int a, int b, ll& c, ll& s) {
        if (a >= b || qr <= l || r <= ql) return;
        if (ql <= l && r <= qr) {
            c += b - a;
            s += ps[v][b] - ps[v][a];
            return;
        }
        int mid = (l + r) / 2;
        go(2 * v, l, mid, ql, qr, lp[v][a], lp[v][b], c, s);
        go(2 * v + 1, mid, r, ql, qr, a - lp[v][a], b - lp[v][b], c, s);
    }

    pair<ll, ll> query(int x1, int x2, int y1, int y2) {
        ll c = 0, s = 0;
        if (n == 0 || x1 > x2 || y1 > y2) return {0, 0};
        int ql = lower_bound(xs.begin(), xs.end(), x1) - xs.begin();
        int qr = upper_bound(xs.begin(), xs.end(), x2) - xs.begin();
        if (ql >= qr) return {0, 0};
        int a = lower_bound(rootY.begin(), rootY.end(), y1) - rootY.begin();
        int b = upper_bound(rootY.begin(), rootY.end(), y2) - rootY.begin();
        go(1, 0, n, ql, qr, a, b, c, s);
        return {c, s};
    }

    ll count(int x1, int x2, int y1, int y2) { return query(x1, x2, y1, y2).first; }
    ll sum(int x1, int x2, int y1, int y2) { return query(x1, x2, y1, y2).second; }
};
