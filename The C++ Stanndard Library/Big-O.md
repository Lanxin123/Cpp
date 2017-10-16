C++标准库的某些部分，特别STL，需要认真考虑算法和成员函数的效能，因此需要动用
复杂度（complexity）概念。计算机科学家使用特殊符号来表现算法的相对复杂度，如此
便可以很快依据算法的运行时间加以分类，进行算法之间的定性比较。这种衡量成为Big-O标记。

Big-O即数据结构中的O(),忽略常数因子。因为考虑复杂度的时候，输入量必须够大才有意义。

C++标准手册中提到的某些复杂度被成为amortized（分期摊还；折旧成本），意思是长期而言大量操作将
如上述描述般进行，但单一操作可能花费比平均值更长的时间。举个例子，如果你为一个dynamic array
追加元素，运行时间将取决于array是否尚有备用内存。如果内存足够，就属于常量复杂度，因为在尾端
加入一个新元素总是花费相同时间。但如果备用内存不足，那么就是线性复杂度，因为你必须分配足够的内存并
搬动（复制）它们，实际耗用时间取决于当时的元素个数。内存重新分配动作并不常发生。
所以任何“长度充分”的序列（sequence），元素附加动作几乎可说是常亮复杂度。这种复杂度我们便成为amortized
（分期摊还）常量时间。