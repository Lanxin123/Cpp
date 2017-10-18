main ( ) 定义式 

    我还想澄清一个重要而常被误解的核心语言点：唯一 “正确且其移植性”的main()。拫据C++标准，main()只有两种定义式具备可移植性，那就是 
    int main()
    {
        ...
    } 
    和
    int main(int argc,char* argv[])
    {
        ...
    }

    其中的argv，也就足命令行实参（command-line argument)所形成的array，也可定义为 char**。注意其返回类型必须是int。 
    你可以（但非必要）以一个return语句终结main()。不同于C, C++ 定义了一个隐晦的 
        return 0; 
    于main()终点。这意味着任何程序离幵main()时可以不写return语句。0以外的任何值都代表某种失败。为此，本书中我的所有例子都没在main()终点放上一个return语句。 
    如果不想以“ main()返回”方式结束C++程序，通常应该调用exit()、quick_exit() (C++11之后）或terminate()。
