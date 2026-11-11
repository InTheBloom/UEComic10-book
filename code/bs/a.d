import std;

// book:begin Range
struct Range {
    import std.exception;
    alias T = long;

    T l, r;
    this (T l_, T r_) {
        enforce(l_ <= r_);
        l = l_;
        r = r_;
    }

    T at (size_t idx) {
        enforce(idx < r - l);
        return l + idx;
    }
    size_t length () {
        return r - l;
    }
    bool empty () {
        return length() == 0;
    }
    T front () {
        enforce(0 < length());
        return at(0);
    }
    T back () {
        enforce(0 < length());
        return at(length() - 1);
    }
}

// book:end Range

// book:begin meguru
int meguru (alias fun) (int ok, int ng) {
    // fun(ok) == trueかつfun(ng) == falseを不変条件にする

    while (1 < abs(ok - ng)) {
        int mid = (ok + ng) / 2;
        if (fun(mid)) {
            ok = mid;
        }
        else {
            ng = mid;
        }
    }

    return ok;
}
// book:end meguru

// book:begin binarySearch
Range[2] binarySearch (alias fun) (Range r) {
    // fun(r.at())が負 or 正を返すような関数を仮定！

    if (r.empty()) {
        return [r, r];
    }

    if (0 <= fun(r.front())) {
        return [Range(r.front(), r.front()), r];
    }
    if (fun(r.back()) < 0) {
        return [r, Range(r.back() + 1, r.back() + 1)];
    }

    auto L = r.front();
    auto R = r.back();

    while (1 < R - L) {
        auto mid = (L + R) / 2;
        if (fun(mid) < 0) {
            L = mid;
        }
        else {
            R = mid;
        }
    }

    return [Range(r.front(), R), Range(R, r.back() + 1)];
}
// book:end binarySearch

// book:begin search
size_t search (int N, int[] A, int L) {
    // Aはソート済みを想定
    int f (long idx) {
        if (A[idx] < L) {
            return -1;
        }
        return 1;
    }
    auto ret = binarySearch!(f)(Range(0, N));
    return ret[1].length;
}
// book:end search

void main () {
    // ABC255C();
    ABC212C();
}

// book:begin ABC255C
void ABC255C () {
    long X, A;
    int D;
    long N;
    readln.read(X, A, D, N);

    Range[2] ret;
    if (0 <= D) {
        int f (long x) {
            if (A + D * x < X) {
                return -1;
            }
            return 1;
        }
        ret = binarySearch!(f)(Range(0, N));
    }
    else {
        int g (long x) {
            if (X < A + D * x) {
                return -1;
            }
            return 1;
        }
        ret = binarySearch!(g)(Range(0, N));
    }

    long ans = long.max;
    if (!ret[0].empty()) {
        ans = min(ans, abs(X - (A + D * ret[0].back())));
    }
    if (!ret[1].empty()) {
        ans = min(ans, abs(X - (A + D * ret[1].front())));
    }

    writeln(ans);
}

// book:end ABC255C

// book:begin ABC212C
void ABC212C () {
    int N, M;
    readln.read(N, M);
    auto A = readln.split.to!(int[]);
    auto B = readln.split.to!(int[]);

    long ans = long.max;
    B.sort;
    foreach (i; 0 .. N) {
        int f (long idx) {
            if (B[idx] < A[i]) {
                return -1;
            }
            return 1;
        }
        auto ret = binarySearch!(f)(Range(0, M));
        if (!ret[0].empty()) {
            ans = min(ans, A[i] - B[ret[0].back()]);
        }
        if (!ret[1].empty()) {
            ans = min(ans, B[ret[1].front()] - A[i]);
        }
    }

    writeln(ans);
}

// book:end ABC212C

void read (T...) (string S, ref T args) {
    import std.conv : to;
    import std.array : split;
    auto buf = S.split;
    foreach (i, ref arg; args) {
        arg = buf[i].to!(typeof(arg));
    }
}

unittest {
    foreach (l; -5L .. 6L) {
        foreach (r; l .. 6L) {
            // [l, r) 内の境界位置を全探索
            foreach (boundary; l - 1 .. r + 2) {
                long f (long x) {
                    return x < boundary ? -1 : 1;
                }

                auto res = binarySearch!f(Range(l, r));

                long mid = clamp(boundary, l, r);

                assert(res[0] == Range(l, mid),
                    format("left: l=%s r=%s boundary=%s, actual=%s",
                        l, r, boundary, res[0]));

                assert(res[1] == Range(mid, r),
                    format("right: l=%s r=%s boundary=%s, actual=%s",
                        l, r, boundary, res[1]));
            }
        }
    }

    writeln("All tests passed!");
}
