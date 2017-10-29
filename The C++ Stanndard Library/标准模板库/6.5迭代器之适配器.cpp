迭代器之适配器（Iterator Adapter）
    
    迭代器（Iterator）是一个春抽象概念：任何东西，只要其行为类似迭代器，它就是一个迭代器。因此，你可以撰写一些类（class）具备迭代器接口，有着各不同的行为。C++标准库提供了数个预定义的特殊迭代器，提供了数个预定义的特殊迭代器，
    亦即所谓迭代器适配器（iterator adapter）。它们不仅是辅助性质而已，它们赋予整个迭代器抽象概念更强大的威力。
        以下数小节简介下面各种迭代器适配器（iterator adapter）：    
    1. Insert iterator（安插型迭代器）
    2. Stream iterator （串流迭代器）
    3. Reverse iterator （逆向迭代器）
    4. Move iterator （搬移迭代器）（自C++11起）
    9.4节第488页对它们有更翔实的说明。

Insert Iteator（安插型迭代器）

    迭代器适配器的第一个例子是insert iterator,或称inserter。它可以使算法以安插（insert）方式而非覆写（overwrite）方式运作。使用它可以解决算法的“目标空间不足问题”。是的，它会促使目标区间的大小按需求成长。
        Insert iterator内部将接口做了新的定义：
    如果你对某个元素陚值（assign）, 会引发“对其所属集合的安插（insert）动作”。至于插入位置是在容器的最前或最后，或某特定位置上，要视三种不同的insert iterator而定。 
    单步前进（step  forward） 不会造成任何动静（是个no-op）。 
    请看下面这个例子：
    





