关键字 constexpr

    自C++11起，constexpr可用来让表达式核定于编译期。例如：
    constexpr int square(int x)
    {
        return x*x;
    }
    float a [square (9)]； // OK since C++11: a has 81 elements

