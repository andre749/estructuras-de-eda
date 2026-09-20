#include <bits/stdc++.h>
using namespace std;

template<class T, class Cmp = less<T>>
struct BinomialHeap {
    static const int K = 40;

    struct Node {
        T key;
        Node *child, *sib;
        Node(const T& k) : key(k), child(nullptr), sib(nullptr) {}
    };

    Node* tr[K];
    int sz;
    Cmp cmp;

    BinomialHeap() : sz(0) { fill(tr, tr + K, nullptr); }

    Node* link(Node* a, Node* b) {
        if (cmp(b->key, a->key)) swap(a, b);
        b->sib = a->child;
        a->child = b;
        return a;
    }

    void merge(BinomialHeap& o) {
        Node* carry = nullptr;
        for (int i = 0; i < K; i++) {
            Node* xs[3];
            int m = 0;
            if (tr[i]) xs[m++] = tr[i];
            if (o.tr[i]) xs[m++] = o.tr[i];
            if (carry) xs[m++] = carry;
            o.tr[i] = nullptr;
            carry = nullptr;
            if (m == 0) {
                tr[i] = nullptr;
            } else if (m == 1) {
                tr[i] = xs[0];
            } else if (m == 2) {
                tr[i] = nullptr;
                carry = link(xs[0], xs[1]);
            } else {
                tr[i] = xs[0];
                carry = link(xs[1], xs[2]);
            }
        }
        sz += o.sz;
        o.sz = 0;
    }

    void push(const T& x) {
        Node* c = new Node(x);
        int i = 0;
        while (tr[i]) {
            c = link(tr[i], c);
            tr[i] = nullptr;
            i++;
        }
        tr[i] = c;
        sz++;
    }

    int best() {
        int b = -1;
        for (int i = 0; i < K; i++)
            if (tr[i] && (b < 0 || cmp(tr[i]->key, tr[b]->key))) b = i;
        return b;
    }

    T top() { return tr[best()]->key; }

    void pop() {
        int b = best();
        Node* r = tr[b];
        tr[b] = nullptr;
        BinomialHeap h;
        Node* c = r->child;
        for (int j = b - 1; j >= 0; j--) {
            Node* nx = c->sib;
            c->sib = nullptr;
            h.tr[j] = c;
            c = nx;
        }
        h.sz = (1 << b) - 1;
        sz -= 1 << b;
        delete r;
        merge(h);
    }

    int size() const { return sz; }
    bool empty() const { return sz == 0; }
};
