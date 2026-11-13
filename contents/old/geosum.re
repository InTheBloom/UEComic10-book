= 等比数列の和の計算

定数@<m>$\alpha, r$と漸化式
//texequation{
\begin{aligned}
a _ 1 &= \alpha \\
a _ {n + 1} &= r a _ {n}
\end{aligned}
//}
によって定まる数列@<m>$a$を@<B>{等比数列}と言います。
この章では、等比数列の先頭@<m>$n$項の和@<m>$S _ n = a _ 1 + a _ 2 + \dots + a _ n$を高速に計算するアルゴリズムを紹介します。

== 再帰的解法1
本節では@<m>$S _ n$の構造に着目した再帰的な解法を紹介します。
まず簡単のため、等比数列の初項が1、つまり@<m>$\alpha = 1$である場合のみを考えることにします。
ただし、@<m>$\alpha = 1$であるときの解を求めた後に全体を@<m>$\alpha$倍すれば元の問題が解けることに留意してください。
以降の節においても@<m>$\alpha = 1$のケースのみを考えます。

=== アイデア
@<m>$S _ n$のある連続部分を切り取ったとしても、@<m>$r$がひとつずつ掛けられながら足されるという構造は変わりません。
@<m>$x$における@<m>$S _ x$がわかれば、より大きな@<m>$y$に対する@<m>$S _ y$を求めるのに役立ちそうです。
そこで、問題を半分に分けることを考えてみます。

@<m>$n$が偶数のとき、
//texequation{
\begin{aligned}
S _ n &= 1 + r + \dots + r ^ {\frac{n}{2} - 1} + r ^ {\frac{n}{2}} + \dots + r ^ {n - 1} \\
&= S _ {\frac{n}{2}} + r ^ {\frac{n}{2}} S _ {\frac{n}{2}}
\end{aligned}
//}

が成立します。
この事実に従って適切に計算することで、問題のサイズを半分にしていくことができます。
@<m>$n$が奇数のときは@<m>$S _ n = 1 + rS _ {n - 1}$を利用して偶数のケースに帰着させてしまいましょう。

=== アルゴリズム
与えられた自然数@<m>$n$に対して、@<m>$(r ^ n, S _ n)$を計算する関数@<m>$f$を設計します。
以下、@<m>$f(n)[0], f(n)[1]$はそれぞれ@<m>$r ^ n, S _ n$を指すとします。

@<m>$\hrulefill$

1. @<m>$n = 0$のとき@<br>{}
@<m>$(1, 0)$を返却。

2. @<m>$n = 2k$のとき@<br>{}
@<m>$(f(k)[0] ^ 2, f(k)[1] + f(k)[0] \times f(k)[1])$を返却。

3. @<m>$n = 2k + 1$のとき@<br>{}
@<m>$(r \times f(2k)[0], 1 + r \times f(2k)[1])$を返却。

@<m>$\hrulefill$

それぞれの段階は定数回の和と積を計算するだけなので@<m>$O(1)$回の演算を行います。
再帰2回につき@<m>$n$は半分以下になるため@<m>$\Theta (\log n)$回の再帰で@<m>$n = 1$に到達します。
よって、このアルゴリズムは@<m>$\Theta (\log n)$回の演算で終了します。なお、ここで演算回数に着目したのは、多倍長整数などを用いた際に演算のコストが@<m>$O(1)$時間ではなくなるためです。

=== プログラム例
通常の整数の範囲で計算するために@<code>{mod}で割ったあまりを求めるプログラムになっています。
多倍長整数を用いる場合は剰余演算を消してください。

//list[][]{
long[2] f (long r, long n, const long mod) {
    if (n == 0) {
        return [1 % mod, 0];
    }
    if (n % 2 == 1) {
        auto ret = f(r, n - 1, mod);
        return [r * ret[0] % mod, (1 + r * ret[1]) % mod];
    }
    auto ret = f(r, n / 2, mod);
    return [ret[0] * ret[0] % mod, (ret[1] + ret[0] * ret[1] % mod) % mod];
}
//}

== 再帰的解法2
本節では、解法1と少し異なるアプローチで問題サイズを小さくする再帰的な解法を紹介します。

=== アイデア
@<m>$S _ n = 1 + r + r ^ 2 + r ^ 3 + \dots + r ^ {n - 1}$ですが、これを指数が偶数のものと奇数のものに分けてみます。
すると、@<m>$n$が偶数のとき、

//texequation{
S _ n = (1 + r ^ 2 + \dots + r ^ {n - 2}) + (r + r ^ 3 + \dots + r ^ {n - 1})
//}

という@<m>$n / 2$項の塊二つに分解することができます。そして、前者を@<m>$X$、後者を@<m>$Y$としたとき@<m>$Y = rX$の関係が成立しています。
すなわち、@<m>$X$の値がわかれば@<m>$S _ n = (1 + r)X$であるため元の問題が解けます。

ここで@<m>$X$を観察してみると、これは公比が@<m>$r ^ 2$、項数が@<m>$n / 2$の等比数列の和であることがわかります。

以上より、元の問題から@<m>$X$を求める問題、つまり公比二乗、数列の長さが半分の問題に帰着できました。
@<m>$n$が奇数のときは再帰的解法1で紹介したようにして偶数のケースに帰着させればよいです。

=== アルゴリズム
公比@<m>$r$、長さ@<m>$n$の問題に対する解を計算する関数@<m>$g(r, n)$を設計します。

@<m>$\hrulefill$

1. @<m>$n = 0$のとき@<br>{}
@<m>$0$を返却。

2. @<m>$n = 2k$のとき@<br>{}
@<m>$(1 + r)g(r ^ 2, n / 2)$を返却。

3. @<m>$n = 2k + 1$のとき@<br>{}
@<m>$1 + r g(r, n - 1)$を返却。

@<m>$\hrulefill$

こちらも各段階では演算@<m>$O(1)$回、再帰2回につき@<m>$n$は半分以下になるため@<m>$\Theta (\log n)$回の再帰で終了します。よって全体の演算回数は@<m>$\Theta (\log n)$回です。

=== プログラム例
//list[][]{
long g (long r, long n, const long mod) {
    if (n == 0) {
        return 0;
    }
    if (n % 2 == 1) {
        return (1 + r * g(r, n - 1, mod)) % mod;
    }
    return (1 + r) * g(r * r % mod, n / 2, mod) % mod;
}
//}

== 閉じた式を用いた解法
本節では、@<m>$S _ n$の閉じた式による表示から計算する解法を紹介します。

=== アイデア
@<m>$S _ n$は@<m>$S _ {n + 1}$との差を考えることで閉じた式を得ることができます。

//texequation{
\begin{aligned}
S _ {n + 1} - S _ n &= (1 + r + \dots + r ^ n) - (1 + r + \dots + r ^ {n - 1}) \\
&= r ^ n
\end{aligned}
//}

ここで、@<m>$S _ {n + 1} = 1 + rS _ n$を利用すると、

//texequation{
\begin{aligned}
1 + rS _ n - S _ n &= r ^ n \\
(r - 1) S _ n &= r ^ n - 1 \\
S _ n &= \frac{r ^ n - 1}{r - 1}
\end{aligned}
//}

となり、閉じた式を得ることができました。

この式には@<m>$r ^ n$が含まれていますが、これは@<B>{繰り返し二乗法}というアルゴリズムを利用することで@<m>$\Theta (\log n)$回の演算で計算することができます。
繰り返し二乗法については@<secref>{binary-lifting}を参照してください。

この解法には2つの注意点があります。

@<B>{注意点1}@<br>{}
@<m>$r - 1 = 0$のときは両辺を@<m>$r - 1$で割る変形を行うことができませんが、そのような場合は@<m>$S _ n = n$となるので適切に場合分けを行えばよいです。

@<B>{注意点2}@<br>{}
「@<m>$S _ n$を@<m>$M$で割ったあまり」を求めたい場面ではこのまま適用することはできません。
これは法@<m>$M$において@<m>$r - 1$が乗法逆元を持たない場合があるからです。
砕けた言い方をすると、@<m>$M$で割ったあまりとして数を扱うとき「割り算」ができない場合があるということです。

そのような場合、次のように計算をします。

 1. @<m>$r ^ n - 1$を@<m>$M(r - 1)$で割ったあまりを計算する
 2. その値を@<m>$r - 1$で割る

これが正しく動作することを確認しておきましょう。
@<m>$r ^ n - 1$を@<m>$M(r - 1)$で割ったあまりが@<m>$\mathrm{rem}$であったとします。ただし、@<m>$0 \leq \mathrm{rem} < M(r - 1)$です。
この時、ある整数@<m>$k$がただ一つ存在して、

//texequation{
r ^ n - 1 = kM(r - 1) + \mathrm{rem}
//}

と表示できます。
@<m>$S _ n$は常に整数となることから、@<m>$r ^ n - 1$は@<m>$r - 1$で割り切れることがわかります。
ここで、@<m>$kM(r - 1)$は明らかに@<m>$r - 1$で割り切れることから、@<m>$\mathrm{rem}$も@<m>$r - 1$で割り切ることができます。
両辺を@<m>$r - 1$で割ると、

//texequation{
S _ n = kM + \frac{\mathrm{rem}}{r - 1}
//}

となります。仮定より、@<m>$0 \leq \mathrm{rem} / (r - 1) < M$です。
@<m>$M$で割った余りの定義を考えると、@<m>$\mathrm{rem} / (r - 1)$こそが求める値であることがわかりました。

=== アルゴリズム
@<m>$S _ n$を計算する関数@<code>{h}を設計します。

@<m>$\hrulefill$

1. @<m>$r = 1$のとき@<br>{}
@<m>$n$を返却。

2. @<m>$r \neq 1$のとき@<br>{}
@<m>$r ^ n - 1$を繰り返し二乗法で計算し、@<m>$r - 1$で割った商を返却。
ただし@<m>$M$で割ったあまりを計算するときは@<m>$r ^ n - 1$を@<m>$M(r - 1)$で割ったあまりを求める必要がある。

@<m>$\hrulefill$

計算量は繰り返し二乗法が支配的になり、全体@<m>$\Theta (\log n)$回の演算が必要です。

=== プログラム例
繰り返し二乗法による冪乗計算を行う関数@<code>{pow}の実装は省略しています。また、@<code>{mod}で割ったあまりを求めるプログラムになっています。
//list[][]{
long h (long r, long n, const long mod) {
    if (r == 1) {
        return n % mod;
    }
    long ret = pow(r, n, mod * (r - 1)) - 1;
    if (ret < 0) {
        ret += mod * (r - 1);
    }
    ret /= r - 1;
    return ret;
}
//}

== 線形漸化式の行列表示による解法
本節では@<m>$S _ n$と@<m>$S _ {n + 1}$が線形漸化式により結び付けられることを利用した解法を紹介します。

=== アイデア
@<m>$S _ {n + 1}$と@<m>$S _ n$の間には
//texequation{
S _ {n + 1} = r S _ n + 1
//}
という関係が成立します。
この関係は行列積で表現することができて、

//texequation{
\begin{pmatrix}
S _ {n + 1} \\
1
\end{pmatrix}
=
\begin{pmatrix}
r & 1 \\
0 & 1
\end{pmatrix}
\begin{pmatrix}
S _ n \\
1
\end{pmatrix}
//}

となります。@<m>$S _ 0 = 0$とする@<fn>{fn-matpow}と、
//footnote[fn-matpow][@<m>$S _ 0 = 0$はここで定義しましたが、漸化式は依然成立します。]

//texequation{
\begin{aligned}
\begin{pmatrix}
S _ {n} \\
1
\end{pmatrix}
&=
\begin{pmatrix}
r & 1 \\
0 & 1
\end{pmatrix}
\begin{pmatrix}
S _ {n - 1} \\
1
\end{pmatrix} \\
\begin{pmatrix}
S _ n \\
1
\end{pmatrix}
&=
\begin{pmatrix}
r & 1 \\
0 & 1
\end{pmatrix}
\begin{pmatrix}
r & 1 \\
0 & 1
\end{pmatrix}
\begin{pmatrix}
S _ {n - 2} \\
1
\end{pmatrix} \\
&\dots \\
\begin{pmatrix}
S _ n \\
1
\end{pmatrix}
&= 
\begin{pmatrix}
r & 1 \\
0 & 1
\end{pmatrix}
^ n
\begin{pmatrix}
0 \\
1
\end{pmatrix}
\end{aligned}
//}

となります。あとは行列の@<m>$n$乗を計算できれば解くことができます。
そして、行列の@<m>$n$乗も繰り返し二乗法により計算可能です。
繰り返し二乗法については@<secref>{binary-lifting}を参照してください。

=== アルゴリズム

@<m>$\hrulefill$

@<m>$\left( \begin{smallmatrix} r & 1 \\ 0 & 1 \end{smallmatrix} \right) ^ n$
を繰り返し二乗法で計算し、@<m>$\left( \begin{smallmatrix}0 \\ 1\end{smallmatrix} \right)$との積を計算して返却。

@<m>$\hrulefill$

こちらも繰り返し二乗法が支配的で、行列積1回の計算回数を@<m>$c$として、@<m>$\Theta (c \log n)$回の演算が必要です。

=== プログラム例
繰り返し二乗法による冪乗計算を行う関数@<code>{pow}の実装は省略しています。また、@<code>{mod}で割ったあまりを求めるプログラムになっています。

//list[][]{
long i (long r, long n, const long mod) {
    auto ret = pow([r, 1, 0, 1], n, mod);
    return ret[1];
}
//}

== 補足
==={binary-lifting} 繰り返し二乗法について
繰り返し二乗法は冪乗計算を高速に行うアルゴリズムです。
例として@<m>$5 ^ {10}$を考えてみましょう。

中心的なアイデアは、@<m>$5 ^ {10} = 5 \times 5 \times \dots \times 5$として9回の掛け算を行うのではなく、@<m>$5 ^ {10} = 5 ^ 5 \times 5 ^ 5$といった風に@<m>$n / 2$乗の2乗として計算することです。

こうすることで、計算する必要のある指数の大きさをどんどん半分にすることができ、掛け算の回数を@<m>$\Theta (\log n)$回にすることができます。
再帰の各段階では掛け算1回を行うのみなので、計算回数は@<m>$O(1)$回です。よって全体の計算回数も@<m>$\Theta (\log n)$回になります。

@<m>$n$が奇数のときは、@<m>$a ^ n = a ^ {n - 1} \times a$として偶数のケースに帰着させるか、@<m>$a ^ n = a ^ {\lfloor n / 2 \rfloor} \times a ^ {\lfloor n / 2 \rfloor} \times a$とすればよいです。

以下に整数@<m>$a$を@<m>$n$乗した値を@<m>$\mathrm{mod}$で割ったあまりを計算する関数@<code>{pow}を掲載します。
//list[][]{
long pow (long a, long n, const long mod) {
    if (n == 0) {
        return 1 % mod;
    }
    if (n % 2 == 1) {
        return a * pow(a, n - 1, mod) % mod;
    }
    auto half = pow(a, n / 2, mod);
    return half * half % mod;
}
//}

積に結合法則が成立すれば整数の冪乗以外にも利用できます。例えば2×2行列の冪乗を計算する関数は以下のようになります。

//list[][]{
long[4] pow (long[4] a, long n, const long mod) {
    if (n == 0) {
        return [1 % mod, 0, 0, 1 % mod];
    }
    if (n % 2 == 1) {
        auto sm = pow(a, n - 1, mod);
        return [
            (a[0] * sm[0] + a[1] * sm[2]) % mod,
            (a[0] * sm[1] + a[1] * sm[3]) % mod,
            (a[2] * sm[0] + a[3] * sm[2]) % mod,
            (a[2] * sm[1] + a[3] * sm[3]) % mod
        ];
    }
    auto half = pow(a, n / 2, mod);
    return [
        (half[0] * half[0] + half[1] * half[2]) % mod,
        (half[0] * half[1] + half[1] * half[3]) % mod,
        (half[2] * half[0] + half[3] * half[2]) % mod,
        (half[2] * half[1] + half[3] * half[3]) % mod
    ];
}
//}

== 参考文献
 * E - Geometric Progression 解説、shakayami著、@<href>{https://atcoder.jp/contests/abc293/editorial/5972}、2025年9月23日閲覧
 ** 再帰的解法2が紹介されています。
 * E - Geometric Progression 解説、kyopro_friends著、@<href>{https://atcoder.jp/contests/abc293/editorial/5966}、2025年9月23日閲覧
 ** 閉じた式を用いた解法において乗法逆元が存在しない場合の回避策が紹介されています。
 * E - Geometric Progression 解説、cn449著、@<href>{https://atcoder.jp/contests/abc293/editorial/5955}、2025年9月23日閲覧
 ** 線形漸化式の行列表示による解法が紹介されています。
 * 繰り返し二乗法に関する頭の整理 - どこにでもいるSEの備忘録、nogawanogawa著、@<href>{https://www.nogawanogawa.work/entry/a%5En}、2025年9月23日閲覧
 * 行列累乗まとめ(競プロ)、shibak3n著、@<href>{https://zenn.dev/shibak3n/articles/f08a8ad67a7d14}、2025年9月23日閲覧
 ** 繰り返し二乗法の応用が紹介されています。
