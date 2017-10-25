STL组件

    若干精心勾画的组件共同合作，构筑起STL的基础。这些组件中最关键的是容器、迭代器和算法。 
    
    容器（Container），用来管理某类对象的集合。每一种容器都有其优点和缺点，所以，为了应付不同的需求，STL准备了不同的容器类型。容器可以是array或linked list，或者每个元素有一个特别的key。

    迭代器（Iterator），用来在一个对象集合（collection of objects）内遍历元素。这个对象集合或许是个容器，或许是容器的一部分。迭代器的主要好处是，为所有各式各样的容器提供了一组很小的共通接口。
    例如其中一个操作是行进至集合内的下一元素。至于如何做到当然取决于集合的内部结构。不论这个集合是array或tree或hash table,此一行进动作都能成功，因为每一种容器都提供了自己的迭代器，而这些迭代器了解容器的内部结构，知道该做些什么。 
    迭代器的接口和寻常的pointer差不多，调用operator++就累进，调用operator*就提领（访问）被指向的值。所以你可以把迭代器视为一种smart pointer,能够把“前进至下一元素”的意图转换成合适的操作。

    算法（Algorithm） ，用来处理集合内的元素。它们可以出于不同的目的而査找、排序、修改、使用元素。通过迭代器的协助，我们只需撰写一次算法，就可以将它应用于任意容器，因为所有容器的迭代器都提供一致的接口。 
    
    你还可以提供一些特殊的辅助函数供算法调用，从而获取更佳的灵活性。这样你就可以一方面运用标准算法，一方面配合自己特殊或复杂的需求。例如，你可以提供自己的查找准则（search criterion）或元素合并时的特殊操作。
    特别是由于C++11新引入了lambda,你得以轻松指明在容器元素身上进行任何种类的动作。 
    
    STL的基本观念就是将数据和操作分离。数据由容器类加以管理，操作则由可定制（configurable）的算法定义之。迭代器在两者之间充当黏合剂，使任何算法都可以和任何容器交互运作（图6.1）。

    STL将数据和算法分开对待，而不是合并考虑。从某种意义上说，STL的概念和面向对象编程（OOP）的最初思想是矛盾的。然而那么做有着很重要的因素。
    首先，你可以将各种容器与各种算法结合起来，在很小的框架（framework）内达到非常大的弹性。
    STL的一个根本特性是，所有组件都可以针对任意类型（type）运作。顾名思义，所谓standard template library意味着其内的所有组件都是“可接受任意类型”的template，前提是这些类型必须能够执行必要的操作。
    因此STL成了泛型编程（generic programming）概念下的一个出色范例。容器和算法被泛化为可适用于任意type和class。
    STL甚至提供更泛化的组件。借由特定的适配器（adapter）和函数对象（function objec, functor），你可以补充、约束或定制算法以满足特别需求。


    









