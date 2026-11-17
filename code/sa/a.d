// book:begin suffixArray1
int[] suffixArray1 (string S) {
    const int N = S.length.to!int;

    auto sa = iota(N).array;
    auto rank = new int[](2 * N);
    auto nrank = new int[](2 * N);

    // sa_0の計算
    sa.sort!((a, b) => cast(int)(S[a]) < cast(int)(S[b]));

    // rank_0の計算
    foreach (i; 0 .. N) {
        if (i == 0) {
            rank[sa[i]] = 0;
        }
        else {
            rank[sa[i]] = rank[sa[i - 1]] + (S[sa[i]] == S[sa[i - 1]] ? 0 : 1);
        }
    }

    // rank_0の番兵を入れておく
    foreach (i; N .. 2 * N) {
        rank[i] = nrank[i] = -1;
    }

    // ソート用の比較関数
    bool f (int i, int j, int block) {
        if (rank[i] != rank[j]) {
            return rank[i] < rank[j];
        }
        return rank[i + block] < rank[j + block];
    }

    for (int b = 0; (1 << b) < N; b++) {
        // rank_b から sa_b+1を計算
        int sz = 1 << b;
        sa.sort!((a, b) => f(a, b, sz));

        // sa_b+1 から rank_b+1を計算
        foreach (i; 0 .. N) {
            if (i == 0) {
                nrank[sa[i]] = 0;
            }
            else {
                nrank[sa[i]] = nrank[sa[i - 1]] + (f(sa[i - 1], sa[i], sz) ? 1 : 0);
            }
        }

        swap(rank, nrank);
    }

    return sa;
}
// book:end suffixArray1

// book:begin suffixArray2
int[] suffixArray2 (string S) {
    const int N = S.length.to!int;

    // sa_0, rank_0の計算
    auto sa = iota(N).array;
    sa.sort!((a, b) => cast(int)(S[a]) < cast(int)(S[b]));
    auto rank = new int[](2 * N);
    foreach (i; 0 .. N) {
        if (i == 0) {
            rank[sa[i]] = 1;
        }
        else {
            rank[sa[i]] = rank[sa[i - 1]] + (S[sa[i]] == S[sa[i - 1]] ? 0 : 1);
        }
    }

    auto nrank = new int[](2 * N);
    auto nsa = new int[](N);

    auto cnt = new int[](N + 1);
    foreach (b; 0 .. 30) {
        const int sz = 1 << b;
        if (N <= sz) {
            break;
        }

        // (RANK[i], RANK[i + 2^b])の基数ソートを行い、sa_b+1を計算する
        cnt[] = 0;
        foreach (i; 0 .. N) {
            cnt[rank[sz + i]]++;
        }
        foreach (i; 1 .. N + 1) {
            cnt[i] += cnt[i - 1];
        }
        foreach_reverse (i; 0 .. N) {
            cnt[rank[sz + sa[i]]]--;
            int p = cnt[rank[sz + sa[i]]];
            nsa[p] = sa[i];
        }
        swap(sa, nsa);

        cnt[] = 0;
        foreach (i; 0 .. N) {
            cnt[rank[i]]++;
        }
        foreach (i; 1 .. N + 1) {
            cnt[i] += cnt[i - 1];
        }
        foreach_reverse (i; 0 .. N) {
            cnt[rank[sa[i]]]--;
            int p = cnt[rank[sa[i]]];
            nsa[p] = sa[i];
        }
        swap(sa, nsa);

        // rank_b+1の計算
        foreach (i; 0 .. N) {
            if (i == 0) {
                nrank[sa[i]] = 1;
            }
            else {
                nrank[sa[i]] = nrank[sa[i - 1]] + (rank[sa[i - 1]] == rank[sa[i]] && rank[sa[i - 1] + sz] == rank[sa[i] + sz] ? 0 : 1);
            }
        }
        swap(rank, nrank);
    }

    return sa;
}
// book:end suffixArray2

// book:begin suffixArray3
int[] suffixArray3 (string S) {
    int[] KS (int[] X) {
        // 各要素は0以上|X|未満

        // 小さいケースは愚直に処理
        if (X.length <= 5) {
            auto sa = iota(cast(int)(X.length)).array;
            bool cmp (const int[] a, const int[] b) {
                int al = cast(int)(a.length);
                int bl = cast(int)(b.length);
                foreach (i; 0 .. al) {
                    if (i == bl || b[i] < a[i]) {
                        return false;
                    }
                    if (a[i] < b[i]) {
                        return true;
                    }
                }
                return al < bl;
            }
            sa.sort!((a, b) => cmp(X[a .. $], X[b .. $]));
            return sa;
        }

        // N: Xの長さ
        // N0: mod3で0の場所の数
        // N1: mod3で1の場所の数
        // N2: mod3で2の場所の数
        const int N = cast(int)(X.length);
        int N0 = cast(int)(X.length + 2) / 3;
        int N1 = cast(int)(X.length + 1) / 3;
        int N2 = cast(int)(X.length) / 3;

        // Xにアクセスするときの補助関数
        int at (int idx) {
            enforce(0 <= idx);
            if (idx < N) {
                return X[idx] + 1;
            }
            return 0;
        }

        // mod3で1と2の場所を起点とした長さ3の文字列をradix sortで1つの塊とする
        auto sa3 = new int[](N1 + N2);
        auto nsa3 = new int[](N1 + N2);
        {
            int cur = 0;
            foreach (i; 0 .. N) {
                if (i % 3 != 0) {
                    sa3[cur++] = i;
                }
            }
        }

        auto cnt = new int[](N + 1);
        foreach_reverse (p; 0 .. 3) {
            cnt[] = 0;
            foreach (i; sa3) {
                cnt[at(i + p)]++;
            }
            foreach (i; 0 .. N) {
                cnt[i + 1] += cnt[i];
            }

            foreach_reverse (i; sa3) {
                int j = --cnt[at(i + p)];
                nsa3[j] = i;
            }
            swap(sa3, nsa3);
        }

        // エンコードされた値の作成
        auto rank3 = new int[](N);
        foreach (i; 0 .. N1 + N2) {
            if (i == 0) {
                rank3[sa3[i]] = 0;
                continue;
            }
            int pl = 0;
            foreach (p; 0 .. 3) {
                if (at(sa3[i] + p) != at(sa3[i - 1] + p)) {
                    pl = 1;
                    break;
                }
            }
            rank3[sa3[i]] = rank3[sa3[i - 1]] + pl;
        }

        // SA12とRANK12の構築
        auto s12 = new int[](N1 + N2 + 1);
        foreach (i; 0 .. N1) {
            s12[i] = rank3[3 * i + 1] + 1;
        }
        s12[N1] = 0;
        foreach (i; 0 .. N2) {
            s12[N1 + 1 + i] = rank3[3 * i + 2] + 1;
        }
        auto sa12 = KS(s12);
        auto rank12 = new int[](N + 3);

        // sa12[i]を元の[0 .. N)に戻す
        int restoreIndex (int x) {
            if (x < N1) {
                return 3 * x + 1;
            }
            return 3 * (x - N1 - 1) + 2;
        }

        foreach (i; 0 .. N1 + N2) {
            rank12[restoreIndex(sa12[i + 1])] = i;
        }
        rank12[N] = rank12[N + 1] = rank12[N + 2] = -1;

        // SA0の構築: (X[i], rank12[i + 1])のradix sort
        auto sa0 = new int[](N0);
        foreach (i; 0 .. N0) {
            sa0[i] = 3 * i;
        }
        {
            auto nsa0 = new int[](N0);
            cnt[] = 0;
            foreach (i; sa0) {
                cnt[rank12[i + 1] + 1]++;
            }
            foreach (i; 0 .. N) {
                cnt[i + 1] += cnt[i];
            }
            foreach_reverse (i; sa0) {
                int j = --cnt[rank12[i + 1] + 1];
                nsa0[j] = i;
            }
            swap(sa0, nsa0);

            cnt[] = 0;
            foreach (i; sa0) {
                cnt[X[i]]++;
            }
            foreach (i; 0 .. N) {
                cnt[i + 1] += cnt[i];
            }
            foreach_reverse (i; sa0) {
                int j = --cnt[X[i]];
                nsa0[j] = i;
            }
            swap(sa0, nsa0);
        }

        // SA0とSA12のマージ
        auto sa = new int[](N);
        {
            int i = 0, j = 1;
            foreach (k; 0 .. N) {
                if (j == N1 + N2 + 1) {
                    sa[k] = sa0[i++];
                    continue;
                }
                if (i == N0) {
                    sa[k] = restoreIndex(sa12[j++]);
                    continue;
                }

                int x = sa0[i];
                int y = restoreIndex(sa12[j]);
                bool zero = false;

                if (y % 3 == 1) {
                    if (at(x) < at(y) || (at(x) == at(y) && rank12[x + 1] < rank12[y + 1])) {
                        zero = true;
                    }
                }
                else {
                    if (at(x) < at(y) || (at(x) == at(y) && at(x + 1) < at(y + 1)) || (at(x) == at(y) && at(x + 1) == at(y + 1) && rank12[x + 2] < rank12[y + 2])) {
                        zero = true;
                    }
                }

                if (zero) {
                    sa[k] = x;
                    i++;
                }
                else {
                    sa[k] = y;
                    j++;
                }
            }
        }

        return sa;
    }

    const int N = cast(int)(S.length);
    auto X = new int[](N);
    int[128] cnt;
    foreach (c; S) {
        cnt[c]++;
    }
    foreach (i; 0 .. 127) {
        cnt[i + 1] += cnt[i];
    }
    foreach (c; S) {
        cnt[c]--;
    }
    foreach (i; 0 .. N) {
        X[i] = cnt[S[i]];
    }

    return KS(X);
}
// book:end suffixArray3
