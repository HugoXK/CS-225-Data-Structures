## Assignment1(Group 18)

Wang Dingkun
Qian Shuaicun
Hua Bingsheng
Xu Ke

----------
### Ex.1
(i)

----------
### Ex.2
(i)
Since if the list $l$ and parameters $[e, f, g]$ are determined, the result of $src[e, f, g]$ is also uniquely defined, the well-defined conditions are those that ensure the uniqueness of the result with given parameters.
For any split of a list $l$, $l = concat(l_1 +l_2)$, $g(src[e,f,g](l_1),)$

(ii)
(1) Let the length function $len(l) = src[0,1,+](l)$, we let 
$$len([])= e = 0$$$$len([x]) = f(x) = 1$$ $$len(l_1+l_2) = g(len(l_1), len(l_2)) = len(l_1) + len(l_2)$$$$T' =N_ {\geq1}$$(2) Let the function implemented is $F : T\rightarrow P$, let $func(l) = src[[],[F(x)],+](l)$, which $+$operation concatenates two lists. Then we have
$$func([])= e = []$$$$func([x]) = f(x) = [F(x)]$$ $$func(l_1+l_2) = g(func(l_1), func(l_2)) = func(l_1) + func(l_2)$$$$T' =\{ l'\}$$
where $l'$ are lists with elements $\in P$.
(3) Let the sub function $sub(l) = src[[],f(x),+](l)$, we have
$$sub([])= e = []$$$$sub([x]) = f(x) = \begin{cases}
[x],\quad \varphi(x)\\
[], \quad else
\end{cases}$$ $$sub(l_1+l_2) = g(sub(l_1), sub(l_2)) = sub(l_1) + sub(l_2)$$$$T' =\{ l'' | l'' \subset l\}$$

(iii) 
Let the length of the whole list be $n$, let $x, x \in T$ be a single element of the list, let $y_1, y_2 \in T'$.
We apply $f(x)$ exactly $n$ times and $g(y_1, y_2)$ exactly $n-1$ times, suppose each time cost of $f(x)$ is $T_f$, each time cost of $g(y_1,y_2)$ is $T_g$, the total complexity is $O(n * T_f + (n-1) * T_g) = O(n * max(T_f, T_g))$
Since the complexity of $g$ may depend on how the list is realised and how to do the structural recursion (e.g. When we want to combine sublists, the complexity of $g$ varies with your strategies), the real complexity will improve if choosing more effective ways to implement $f$ and $g$. 
Furthermore, the complexity of $f(x)$ and $g(y_1, y_2)$ also changes with different parameters, so we use the average time, the total complexity is $O(n * max(\overline{T_f}, \overline{T_g}))$.

----------
### Ex.3
(i)
$pushback(l,x)$ can be expressed by $append(l,x)$.
$pushfront(l,x)$ can be expressed by $insert(l,1,x)$.
$popback(l)$ can be expressed by $n = l.getlength()$ and $l.getitem(n)$ and $remove(l,n)$.
$popfront(l)$ can be expressed by $l.getitem(1)$ and $remove(l,1)$.


