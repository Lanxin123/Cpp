标准模板库
//我对这个部分超级期待。。。

    STL（standard template library,标准模板库）是C++标准库的核心，它深刻影响了标准库的整体结构。STL是一个泛型(generic)程序库，提供一系列软件方案，利用先进、高效的
    算法来管理数据。程序猿无需了解STL的原理，便可享受数据结构和算法领域中的这一个新成果。    
        从程序员的角度看来，STL是由一些可适应不同需求的集合类（collection class） 和一些能够在这些数据集合上运作的算法构成。STL内的所有组件都由template（模板）构成， 所以其元素可以是任意类型。
    更妙的是，STL建立了一个框架（framework）,在此框架下你可以提供其他集合类或算法，与现有的组件搭配共同运作。总之，STL陚予了C++新的抽象层次。抛开dynamic array (动态数组)、linked list (链表)、
    binary tree (二叉树)、hash table （散列表）之类的东西吧，也不必再烦心各种search（査找）算法了。你只需使用恰当的集合类，然后调用其成员函数和（或）算法来处理数据，就万事大吉。 
        当然，如此的灵活性并非免费午餐，代价总是有的。首要的一点是，STL并不好懂。正 因如此，本书倾注了好几章篇幅，为你讲解STL的内容。本章介绍STL的总体慨念，探讨 其编程技术。第一个范例展示如何使用STL以及运用过程中的考量。第7章至第11章详细 讨论STL的各组件（包括容器〔container〕、迭代器〔iterator〕、
    函数对象〔functionobject〕、 算法〔algorithm〕）和更多范例。
    



