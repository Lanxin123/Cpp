Range-Based for 循环

    C++11引入了一种崭新的for循环形式，可以逐一迭代某个给定的区间、数组、集合(range,array,or collection)
    内的每一个元素。其他编程语言可能称次为foreach循环。其一般性语法如下:
    for(decl : coll)
    {
        statement
    }
    其中decl是给定之coll集合中的每个元素的声明：针对这些元素，给定的statement会被执行。例如下面针对传入的初值
    列中的每个元素，调用给定的语句，于是在标准输出装置cout输出元素值：
    for (int i : {2,3,5,7,9,13,17,19})
    {
        std::cout<<i<<std::endl;
    }
    //输出2\n-19\n
    如果要将vector vec的每个元素elem乘以3，可以这么做：
        std ::vector<double> vec;
        ...
        for (autok& elem : vec)
        {
            elem *= 3;
        }
    这里“声明elem为一个reference”很重要，若不这么做，for循环中的语句会作用在元素的一份local copy身上（当然
    或许有时候你想要这样）。
        这意味着，为了避免调用每个元素的copy构造函数和析构函数，你通常应该声明当前元素(current element)为一个
    const reference。于是一个用来“打印某集合内所有元素”的泛型函数应该写成这样：
    template <typename T>
    void printElement(const T& coll)
    {
        for(const auto& elem : coll)
        {
            std::cout<<elem<<std::endl;
        }
    }
    在这里，这种所谓range-based for 语句等同于：
    {
        for (auto _pos = coll.begin();_pos != coll.end(); ++_pos)
        {
            const auto& elem = *_pos;
            std::cout<<elem<<std::endl;
        }
    }
    一般而言，如果coll提供成员函数begin()和end()，那么一个range-based for 循环声明为
    for (decl : coll)
    {
        statement
    }
    便等同于
    {
        for(auto _pos = coll.begin(),_end = coll.end();_pos!=_end;++pos)
        {
            decl = *_pos;
            statement
        }
    }
    或者如果不满足上述条件，那么也等同于一下使用一个全局性begin()和end()且两者都接受coll为实参:
    {
        for(auto _pos = begin(coll),_end= end(coll);_pos!=_end;++_pos)
        {
            decl = *_pos;
            statement
        }
    }
    于是，你甚至可针对初值列(initializer list)使用range-based for 循环，因为class template
    std::initializer_list<>提供了成员函数begin()和end（）。
    此外还有一条规则，允许你使用寻常的、大小一致的C-style array，例如:
    int array[] = {1,2,3,4,5};

    long sum = 0;       //process sum of all elements
    for (int x : array)
    {
        sum += x;
    }

    for(auto elem : {sum,sum*2,sum*4})
    {//print some multipes of sum
        std::cout<<elem<<std::endl;
    }
    输出：
        15
        30
        60