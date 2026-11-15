// book:begin hld_full
import std;

void main () {
    int N, Q;
    readln.read(N, Q);
    auto a = readln.split.to!(int[]);
    auto U = new int[](N - 1);
    auto V = new int[](N - 1);
    foreach (i; 0 .. N - 1) {
        readln.read(U[i], V[i]);
    }
    auto graph = new int[][](N);
    foreach (i; 0 .. N - 1) {
        graph[U[i]] ~= V[i];
        graph[V[i]] ~= U[i];
    }

    // HLD
    // book:begin hldPreCalc
    auto tSize = new int[](N);
    auto parent = new int[](N);
    auto depth = new int[](N);
    void hldPreCalc (int pos, int par) {
        tSize[pos] = 1;
        foreach (to; graph[pos]) {
            if (to == par) {
                continue;
            }
            parent[to] = pos;
            depth[to] = depth[pos] + 1;

            hldPreCalc(to, pos);
            tSize[pos] += tSize[to];
        }
    }
    parent[0] = -1;
    depth[0] = 0;
    hldPreCalc(0, -1);
    // book:end hldPreCalc

    // book:begin hldHeavyFirst
    // graph[i][0]がheavy edgeになるようにする
    foreach (i; 0 .. N) {
        int idx = -1;
        int sz = -1;
        foreach (j, to; graph[i].enumerate(0)) {
            if (parent[i] == to) {
                continue;
            }
            if (sz < tSize[to]) {
                sz = tSize[to];
                idx = j;
            }
        }
        if (sz != -1) {
            swap(graph[i][0], graph[i][idx]);
        }
    }
    // book:end hldHeavyFirst

    // book:begin hldBuild
    auto place = new int[](N);
    auto top = new int[](N);
    int currentPlace = 0;
    void hldBuild (int pos, int par) {
        int nextHeavy = graph[pos][0];
        if (nextHeavy == par) {
            return;
        }

        // heavy edgeへ進む
        top[nextHeavy] = top[pos];
        place[nextHeavy] = currentPlace;
        currentPlace++;
        hldBuild(nextHeavy, pos);

        // light edgeへ進む
        foreach (to; graph[pos]) {
            if (to == par || to == nextHeavy) {
                continue;
            }
            top[to] = to;
            place[to] = currentPlace;
            currentPlace++;
            hldBuild(to, pos);
        }
    }

    top[0] = 0;
    place[0] = currentPlace;
    currentPlace++;
    hldBuild(0, -1);
    // book:end hldBuild

    // book:begin hldSegmentBuild
    // segment treeにheavy pathを乗せる
    auto seg = new SegmentTree!(long, (long a, long b) => a + b, () => 0L)(N);
    foreach (i; 0 .. N) {
        seg.set(place[i], a[i]);
    }

    // book:end hldSegmentBuild

    auto ans = new long[](0);
    foreach (i; 0 .. Q) {
        auto query = readln.split.to!(int[]);
        int t = query[0];

        if (t == 0) {
            int p = query[1];
            int x = query[2];
            seg.set(place[p], seg.get(place[p]) + x);
        }

        if (t == 1) {
        // book:begin hldPathQuery
            int u = query[1];
            int v = query[2];

            long ret = 0;
            while (top[u] != top[v]) {
                if (depth[top[u]] < depth[top[v]]) {
                    ret += seg.prod(place[top[v]], place[v] + 1);
                    v = parent[top[v]];
                }
                else {
                    ret += seg.prod(place[top[u]], place[u] + 1);
                    u = parent[top[u]];
                }
            }

            if (depth[u] < depth[v]) {
                ret += seg.prod(place[u], place[v] + 1);
            }
            else {
                ret += seg.prod(place[v], place[u] + 1);
            }
        // book:end hldPathQuery

            ans ~= ret;
        }
    }

    writefln("%(%s\n%)", ans);
}
// book:end hld_full

void read (T...) (string S, ref T args) {
    import std.conv : to;
    import std.array : split;
    auto buf = S.split;
    foreach (i, ref arg; args) {
        arg = buf[i].to!(typeof(arg));
    }
}

import std.traits : ReturnType, isCallable, Parameters;
import std.meta : AliasSeq;

class SegmentTree (T, alias ope, alias e)
if (
           isCallable!(ope)
        && isCallable!(e)
        && is (ReturnType!(ope) == T)
        && is (ReturnType!(e) == T)
        && is (Parameters!(ope) == AliasSeq!(T, T))
        && is (Parameters!(e) == AliasSeq!())
        )
{
    /* 内部の配列が1-indexedで2冪のセグメントツリー */
    import std.format : format;
    T[] X;
    size_t length;

    /* --- Constructors --- */
    this (size_t length_) {
        adjust_array_length(length_);
        for (size_t i = length; i < 2*length; i++) {
            X[i] = e();
        }
        build();
    }

    import std.range.primitives : isInputRange;
    this (R) (R Range)
    if (isInputRange!(R) && is (ElementType!(R) == T))
    {
        adjust_array_length(walkLength(Range));
        size_t i = length;
        foreach (item; Range) {
            X[i] = item;
            i++;
        }
        while (i < 2*length) { X[i] = e(); i++; }
        build();
    }

    /* --- Functions --- */
    private 
        void adjust_array_length (size_t length_) {
            length = 1;
            while (length <= length_) length *= 2;
            X = new T[](2*length);
        }

        void set_with_no_update (size_t idx, T val)
        in {
            assert(idx < length,
                    format("In function \"set_with_no_update\", idx is out of range. (length = %s idx = %s)", length, idx));
        }
        do {
            X[length+idx] = val;
        }

        void build () {
            for (size_t i = length-1; 1 <= i; i--) {
                X[i] = ope(X[2*i], X[2*i+1]);
            }
        }

    public
        override string toString () {
            string res = "";
            int level = 1;
            while ((2^^(level-1)) < X.length) {
                res ~= format("level: %2s | ", level);
                for (size_t i = (2^^(level-1)); i < (2^^level); i++) {
                    res ~= format("%s%s", X[i], (i == (2^^level)-1 ? "\n" : " "));
                }
                level++;
            }
            return res;
        }

        void set (size_t idx, T val)
        in {
            assert(idx < length,
                    format("In function \"set\", idx is out of range. (length = %s idx = %s)", length, idx));
        }
        do {
            idx += length;
            X[idx] = val;
            while (2 <= idx) {
                idx /= 2;
                X[idx] = ope(X[2*idx], X[2*idx+1]);
            }
        }

        T get (size_t idx)
        in {
            assert(idx < length,
                    format("In function \"get\", idx is out of range. (length = %s idx = %s)", length, idx));
        }
        do {
            idx += length;
            return X[idx];
        }

        T prod (size_t l, size_t r)
        in {
            assert(l < length,
                    format("In function \"prod\", l is out of range. (length = %s l = %s)", length, l));
            assert(r <= length,
                    format("In function \"prod\", r is out of range. (length = %s r = %s)", length, r));
            assert(l <= r,
                    format("In function \"prod\", l < r must be satisfied. (length = %s l = %s, r = %s)", length, l, r));
        }
        do {
            /* Returns all prod O(1) */
            if (l == 0 && r == length) return X[1];
            if (l == r) return e();

            /* Closed interval [l, r] */
            r--;
            l += length, r += length;
            T LeftProd, RightProd;
            LeftProd = RightProd = e();

            while (l <= r) {
                if (l % 2 == 1) {
                    LeftProd = ope(LeftProd, X[l]);
                    l++;
                }
                if (r % 2 == 0) {
                    RightProd = ope(X[r], RightProd);
                    r--;
                }

                l /= 2;
                r /= 2;
            }

            return ope(LeftProd, RightProd);
        }
}
