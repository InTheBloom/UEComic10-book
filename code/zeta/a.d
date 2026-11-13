import std;

void main () {
}

// book:begin mobius_1d
void mobius_1d (int N, int[] A) {
    // 配列Aのメビウス変換をinplaceに行う

    foreach_reverse (i; 1 .. N) {
        // 元の配列の値は隣接2項の差分そのもの
        A[i] = A[i] - A[i - 1];
    }
}
// book:end mobius_1d

// book:begin zeta_2d
void zeta_2d (int N, int M, int[][] A) {
    // 配列Aのゼータ変換をinplaceに行う

    // 各行ゼータ変換
    foreach (i; 0 .. N) {
        foreach (j; 1 .. M) {
            A[i][j] += A[i][j - 1];
        }
    }

    // 各列ゼータ変換
    foreach (j; 0 .. M) {
        foreach (i; 1 .. N) {
            A[i][j] += A[i - 1][j];
        }
    }
}

// book:end zeta_2d

// book:begin mobius_2d
void mobius_2d (int N, int M, int[][] A) {
    // 配列Aのメビウス変換をinplaceに行う

    // 各行メビウス変換
    foreach (i; 0 .. N) {
        foreach_reverse (j; 1 .. M) {
            A[i][j] -= A[i][j - 1];
        }
    }

    // 各列メビウス変換
    foreach (j; 0 .. M) {
        foreach_reverse (i; 1 .. N) {
            A[i][j] -= A[i - 1][j];
        }
    }
}
// book:end mobius_2d

// book:begin divisor_zeta_naive
int[] divisor_sum (int N, int[] A) {
    // A[N]まで使える想定

    auto B = new int[](N + 1);
    foreach (i; 1 .. N + 1) {
        int cur = i;
        while (cur <= N) {
            B[cur] += A[i];
            cur += i;
        }
    }

    return B;
}
// book:end divisor_zeta_naive

// book:begin divisor_zeta
void divisor_zeta (int N, int[] A, int[] primes) {
    // AをinplaceにBに変換
    // primes: N以下の素数の配列

    foreach (p; primes) {
        // 「p軸」に沿って累積和を計算
        const int up = N / p;
        foreach (i; 1 .. up + 1) {
            // p軸で+1した値はi * p
            A[i * p] += A[i];
        }
    }
}
// book:end divisor_zeta

// book:begin divisor_mobius
void divisor_mobius (int N, int[] B, int[] primes) {
    // BをinplaceにAに変換
    // 通常の累積和の逆変換と同様に、各軸後ろから隣接2項の差分を計算

    foreach (p; primes) {
        const int up = N / p;
        foreach_reverse (i; 1 .. up + 1) {
            B[i * p] -= B[i];
        }
    }
}
// book:end divisor_mobius

// book:begin multiple_zeta
void multiple_zeta (int N, int[] A, int[] primes) {
    foreach (p; primes) {
        const int up = N / p;
        foreach_reverse (i; 1 .. up + 1) {
            A[i] += A[i * p];
        }
    }
}
// book:end multiple_zeta

// book:begin multiple_mobius
void multiple_mobius (int N, int[] A, int[] primes) {
    foreach (p; primes) {
        const int up = N / p;
        foreach (i; 1 .. up + 1) {
            A[i] -= A[i * p];
        }
    }
}
// book:end multiple_mobius

// book:begin subset_zeta
void subset_zeta (int N, int[] A) {
    // A[2^N - 1]まで使える前提
    // inplaceにAをBに変換

    foreach (i; 0 .. N) {
        foreach (j; 0 .. 1 << N) {
            // jのibit目が0なら、1にした先に足しこむ
            if ((j & (1 << i)) == 0) {
                A[j | (1 << i)] += A[j];
            }
        }
    }
}

// book:end subset_zeta

// book:begin subset_mobius
void subset_mobius (int N, int[] A) {
    foreach (i; 0 .. N) {
        foreach_reverse (j; 0 .. 1 << N) {
            // 逆向きに隣接項の差分を取る
            if (0 < (j & (1 << i))) {
                A[j] -= A[j ^ (1 << i)];
            }
        }
    }
}
// book:end subset_mobius

// book:begin superset_zeta
void superset_zeta (int N, int[] A) {
    foreach (i; 0 .. N) {
        foreach_reverse (j; 0 .. 1 << N) {
            if (0 < (j & (1 << i))) {
                A[j ^ (1 << i)] += A[j];
            }
        }
    }
}
// book:end superset_zeta

// book:begin superset_mobius
void superset_mobius (int N, int[] A) {
    foreach (i; 0 .. N) {
        foreach (j; 0 .. 1 << N) {
            if ((j & (1 << i)) == 0) {
                A[j] -= A[j | (1 << i)];
            }
        }
    }
}
// book:end superset_mobius
