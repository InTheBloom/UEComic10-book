= Heavy Light Decomposition
この章では、Heavy Light Decomposition（HLD）と呼ばれる手法を紹介します。

== HLDでできること
HLDは木に対するクエリを処理する強力な手法で、その最大のメリットは木のパスに対するモノイド積クエリを捌けるようになることです。
具体的には、次ができるようになります。

//problem[問題: パスクエリ]{

@<m>$N$頂点の木@<m>$T$とモノイド@<m>$(S, \times, e)$が与えられており、最初、木@<m>$T$の頂点@<m>$i$には値@<m>$A _ i \in S$が書き込まれている。

@<B>{クエリ1:}

@<m>$x, a$が与えられる。頂点@<m>$x$に書き込まれた値を@<m>$O(\log N)$時間で@<m>$a$に書き換えよ。

@<B>{クエリ2:}

パスの端点@<m>$u, v$が与えられる。木@<m>$T$において@<m>$u$から@<m>$v$までのパスが@<m>$p _ 1, p _ 2, \dots, p _ k$（ただし、@<m>$p _ 1 = u, p _ k = v$）と表されるとき、@<m>$A _ {p _ 1} \times A _ {p _ 2} \times \dots \times A _ {p _ k}$を@<m>$O(\log ^ 2 N)$時間で計算せよ。

//}

== HLDについて

//definition[定義: HLD]{
頂点数@<m>$N$の根付き木@<m>$T$が与えられたとする。
各頂点@<m>$x$に対して、@<m>$x$自身を含む部分木の頂点数を@<m>$\mathrm{size}(x)$と表記する。葉でない各頂点@<m>$u$に対して次の手続きを行って全ての辺に赤色か青色を割り当てる。

 1. 頂点@<m>$u$の子方向の頂点が@<m>$v _ 1, v _ 2, \dots, v _ k$であるとする。@<m>$\mathrm{size}(v _ 1), \mathrm{size}(v _ 2), \dots, \mathrm{size}(v _ k)$のうち@<m>$\mathrm{size}(v _ i)$が最大となる子@<m>$v _ i$を1つ任意に選び、辺@<m>$(u, v _ i)$を赤色で塗る。
 2. 1で選ばれた@<m>$v _ i$以外の子すべてについて、辺@<m>$(u, v _ j)$を青色で塗る。

この手続きによって得られる辺の塗り分けを@<m>$T$のHLDと呼ぶことにする。
塗り分けは一意に定まらない場合があるが、以降ではどの塗り分けにも成立する性質を議論していくので、本章では塗り分けを1つ任意に固定して考える。
//}

@<img>{hld_sample}に塗り分けの例を示します。この例では塗り分けは一意になるので、各色が定義に沿ってつけられていることを確認してください。

//image[hld_sample][HLDの例。太い実線は赤色の辺、破線は青色の辺に対応する。][width=70%,pos=h]

直感的に説明を行うために色を考えましたが、通常HLDにおいて赤色の辺をheavy edge、青色の辺をlight edgeと呼びます。以後は本文でもheavy edgeとlight edgeという名称を用います。

HLDの非常に重要な性質として、次のものがあります。

//definition[性質: heavy edgeによる直鎖]{
葉でない各頂点について、heavy edgeは子方向に@<B>{ちょうど1本}伸びている。
//}

この性質が成り立っていることが@<img>{hld_sample}で確認できます。
さらに各頂点の親は高々1つなので、親方向にもheavy edgeは高々1本しか接続できません。従って、light edgeを消したときにできる各連結成分は分岐の無い直鎖になります。
これは木をいくつかの直鎖（列）とそれらを結合する辺（light edge）に分解していると考えられそうです。

自然な予想としてheavy edgeで繋がった直鎖をSegmentTreeなどのデータ構造で列として処理することができれば、パス上の頂点を1つずつ調べなくても良くなりそうです。@<img>{heavypath_lightpath}に頂点6から頂点2のパスを考えたときの例を示します。このパスでは直鎖@<m>$(6)$、直鎖@<m>$(1, 3, 4, 8, 5)$の一部、直鎖@<m>$(10, 2)$の3つの直鎖の上を通ります。この時それぞれの直鎖に乗っかっている部分のモノイド積をSegmentTreeなどで列クエリとして処理することができれば、計算量はパスの長さではなくパスが関与する直鎖の数に依存することになります。これは普通に1つずつ頂点を見るよりも早くなりそうですね。

//image[heavypath_lightpath][頂点6から頂点2へのパス。灰色の頂点がパス上の頂点を表し、パスは3つの直鎖に分割できる。][width=70%,pos=h]

実際この予想は正しくて、木上のどのような@<m>$u\text{-}v$パスについても、@<m>$u$から@<m>$v$に行く過程で通る直鎖は十分少なくなることが知られています。
より具体的には@<m>$O(\log N)$個の直鎖しか通らないことが証明できます。
これがHLDによるパスクエリ処理です。木を上手く分解することでパスを@<m>$O(\log N)$個の列に分割しているということです。
計算量について確認しておきましょう。これら性質を認めてしまえば、頂点の値の更新は列上の1点更新なのでSegmentTreeを用いて@<m>$O(\log N)$時間で処理できます。
パスクエリも@<m>$O(\log N)$個の列に対する区間モノイド積の計算になり、@<m>$O(\log ^ 2 N)$時間で処理できます。

ここからは直鎖が少なくなる理由を説明していきます。

== 直鎖の数について

さて、先に用語を更新しておきましょう。すでにheavy edgeのみを考えたときの各連結成分が直鎖になることを説明しました。HLDではこれら連結成分のことをheavy pathと呼びます。以降は直鎖ではなくheavy pathという語を用います。
今示したいことはどのようなパスにおいても関与するheavy pathが@<m>$O(\log N)$個になることです。ところであるheavy path上から別のheavy path上に「乗り換え」が発生するとき必ずlight edgeを通るため、「どのようなパスにおいても通るlight edgeの数が@<m>$O(\log N)$個になる」という命題を示してもよいです。これを示しましょう。

//definition[補題: 根からのパスに含まれるlight edgeの数]{
任意の頂点@<m>$x$について、根から@<m>$x$までのパスが含むlight edgeは@<m>$O(\log N)$本である。
//}

証明:
根からのパスの途中ある時点で@<m>$(u, v)$を通るとし、@<m>$u$が親側とする。
このとき、@<m>$u, v$それぞれを根とする部分木の頂点数@<m>$\mathrm{size}(u), \mathrm{size}(v)$を考える。
@<m>$(u, v)$がlight edgeになっていることから、@<m>$u$は、@<m>$\mathrm{size}(v) \leq \mathrm{size}(p)$かつ@<m>$v \neq p$となる頂点@<m>$p$を子に持っている。すなわち、@<m>$2 \times \mathrm{size}(v) \leq \mathrm{size}(u)$が成立する。
よって@<m>$\mathrm{size}(v) \leq \mathrm{size}(u) / 2$が成立する。

これは、light edgeを通ると部分木のサイズが@<m>$1/2$以下になるということを表している。全体の頂点数が@<m>$N$であることから、light edgeは@<m>$\log N$回より多く通ることはできない。これで題意は示された。 @<m>$\square$

//definition[命題: パスに含まれるlight edgeの数]{
木上の任意のパスが含むlight edgeは@<m>$O(\log N)$本である。
//}

証明:
木の根を@<m>$r$とし、@<m>$u \text{-} v$パスを考える。@<m>$u$と@<m>$v$のLCAを@<m>$p$とする。
パスは@<m>$u \text{-} p$と@<m>$p \text{-} v$に分解することができるので、このそれぞれが含むlight edgeの本数を考える。
@<m>$u \text{-} p$パスが含むlight edgeの本数は明らかに@<m>$u \text{-} r$パスが含むlight edgeの本数以下であるため、補題よりこれは@<m>$\log N$本以下である。
@<m>$p \text{-} v$パスについても同様にして@<m>$\log N$本以下であることがわかる。
よって題意は示された。 @<m>$\square$

以上より、パス中に現れるheavy pathが十分少ないことがわかりました。

== 実装編
理論的なところは終わりで、ここからは実際にどう実装するかの話をしていきましょう。
以後グラフは親方向にも辺をもつ隣接リスト@<m>$\mathrm{graph}$で保持するとして考えますが、親方向の辺をあらかじめ消すことができれば少しだけ場合分けが消せます。
また、図では引き続き1-indexedですが、プログラムおよび本文では頂点番号は0-indexedで考えて、常に根は頂点@<m>$0$とします。
HLDの実装では、まず最初に以下の3つのデータをdfsで前計算します。

 * @<m>$\mathrm{tsize}[i]$: @<m>$i$を根とする部分木の要素数（@<m>$i$を含む）
 * @<m>$\mathrm{parent}[i]$: @<m>$i$の親頂点（@<m>$\mathrm{parent}[0] = -1$とする）
 * @<m>$\mathrm{depth}[i]$: @<m>$i$の深さ（@<m>$\mathrm{depth}[0] = 0$とする）

実装は以下のようになります。

//list[][][file=code/hld/a.d,region=hldPreCalc]{
//}

さらに@<m>$\mathrm{tsize}$を用いることでどの辺をheavy edgeにすると良いかを選ぶことができます。
後の実装を簡単にするために、これを用いて葉でない頂点たちに対して@<m>$\mathrm{graph}[i][0]$が親以外のheavy edgeになるように隣接リストの順番を入れ替えておきます。
その後次の2データをdfsで前計算します。

 * @<m>$\mathrm{place}[i]$: heavy pathを優先的に通るように木を先行順に走査したとき、頂点@<m>$i$に何番目に踏み入るか（@<m>$\mathrm{place}[0] = 0$とする）
 * @<m>$\mathrm{top}[i]$: @<m>$i$が所属するheavy pathのうち、最も深さが小さい頂点

@<m>$\mathrm{graph}$の並べ替えは次のように行います。

//list[][][file=code/hld/a.d,region=hldHeavyFirst]{
//}

2回目のdfsは次のように行います。

//list[][][file=code/hld/a.d,region=hldBuild]{
//}

@<img>{hld_place}に@<m>$\mathrm{place}$の値がどうなるかを示します。図の通り、@<m>$\mathrm{place}$の値はheavy path上を進むごとに@<m>$+1$されていくようになります。

//image[hld_place][heavy edgeを優先した先行順の走査。頂点の外側の数値はplaceの値を表す。][width=70%,pos=h]

前計算の最後に頂点@<m>$i$の値@<m>$A[i]$をSegmentTree上の@<m>$\mathrm{place}[i]$番目に配置します。こうすることで1つのheavy pathをSegmentTree上の1つの連続区間として扱うことができ、1つのheavy pathを1回のSegmentTreeのクエリで処理できます。

//list[][][file=code/hld/a.d,region=hldSegmentBuild]{
//}

あとはクエリを処理するだけです。クエリ@<m>$(u, v)$を考えます。
パスクエリは@<m>$u, v$それぞれから根の方向にheavy pathを登って最後に@<m>$\mathrm{LCA}(u, v)$で合流するようにしていきます。

まず、@<m>$u, v$が同じheavy pathに乗っていないときを考えます。これはすなわち@<m>$\mathrm{top}[u] \neq \mathrm{top}[v]$のときです。
このとき仮に@<m>$\mathrm{depth}[\mathrm{top}[v]] \leq \mathrm{depth}[\mathrm{top}[u]]$だったとします。このとき@<m>$\mathrm{LCA}(u, v)$は必ず@<m>$\mathrm{top}[u]$よりも上にあるため、@<m>$u$はheavy edgeを始点まで登り切っても@<m>$u \text{-} v$パスをはみ出しません。
よってSegmentTreeで対応する区間@<m>$[\mathrm{place}[\mathrm{top}[u]], \mathrm{place}[u] + 1)$を集計し、@<m>$u$側を別のheavy pathに乗り換えさせればよいです。
大小関係が逆だった時は変数を入れ替えて操作すればよいです。
@<img>{pathquery}に@<m>$3 \text{-} 2$パスの処理の様子を示します。濃い色で塗られているのがこのターンで処理するheavy pathです。集計が終わったら残りは@<m>$3 \text{-} 1$パスを処理すればよく、同じheavy pathに合流することがわかります。

//image[pathquery][頂点3から頂点2へのパスクエリ。濃い色の頂点10と頂点2を集計し、残りを頂点3から頂点1へのパスとして処理する。][width=70%,pos=h]

上記操作を繰り返すことで@<m>$u, v$は同じheavy pathに乗る瞬間が来ます。
同じheavy pathに乗ったときは最後に@<m>$[\min(\mathrm{place}[u], \mathrm{place}[v]), \max(\mathrm{place}[u], \mathrm{place}[v]) + 1)$を集計して終了します。

パス上の総和を計算する例では次のようになります。

//list[][][file=code/hld/a.d,region=hldPathQuery]{
//}

おめでとうございます。
これでHLDを書けるようになりましたね！
今回の実装方針をそのまま用いてVertex Add Path Sum（@<href>{https://judge.yosupo.jp/problem/vertex_add_path_sum}）を解くことができます。是非チャレンジしてみてください。
紹介したコード断片をつなぎ合わせたVertex Add Path Sumに対する完全なソースコードを章末に掲載しておきます。参考にしてみてください。
今回の実装は可換なモノイドにのみ対応したものです。非可換なモノイドに対応させるためには各heavy pathで積を計算する方向と、@<m>$u$側、@<m>$v$側の集計結果を結合する順序などに注意する必要があります。本書では扱いませんが、自ブログ@<href>{https://inthebloom.github.io/post/monoid-technique/}で詳しく解説しています。

== ソースコード
以下にVertex Add Path Sumに正解するコードのうちSegmentTreeの実装と入力用関数を除いたものを示します。SegmentTreeには、要素の取得・更新と半開区間の総和を求める操作を用います。

//list[][][file=code/hld/a.d,region=hld_full]{
//}
