关键字 noexcept

    C++11提供了关键字noexcept，用来指明某个函数无法----或不打算---抛出异常。例如： 
    void foo () noexcept； 
    声明了 foo()不打算拋出异常。若有异常未在foo()内被处理---亦即如果foo()拋出异常---程序会被终止，然后std::terminate()被调用并默汄调用std::abort()//程序被终止。