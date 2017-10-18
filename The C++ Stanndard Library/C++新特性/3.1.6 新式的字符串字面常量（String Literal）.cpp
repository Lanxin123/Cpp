新式字符串字面常量

    自C++11起，你可以定义raw string/*初始字符串*/和multibyte/wide-character等字符串字面常量。

    Raw String Literal
    Raw string允许我们定义字符序列（character sequence)，做法是确切写下其内容使其成为一 个raw character sequence。于是你可以省下很多用来装饰特殊字符的escape/*泄露*/符号。 
    Raw string以R"(幵头，以)"结尾，可以内含line break/*换行符*/。例如一个用来表示“两个反斜线和一个n” 的寻常字面常量可定义如下：

    "\\\\n"//转义字符 \\n转义
    也可以定义它为如下raw string literal:
    R"(\\n)"
    如果要在raw string内写出)“,可使用定义符（delimiter）。因此，一个raw string的完整语法是R“delim(...)delim”，其中delim是个字符序列(character sequence)，最多16个基本字符，
    不可含反斜线（backslash）、空格（whitespace）和小括号（parenthesis）。

    举个例子，下面的raw string literal 
        R"nc(a\ 
            b\nc()" 
            )nc"； 
    等同于以下的寻常string literal： 
    "a\\\n           b\\nc()\"\n            " 
    这样的string内含一个a、一个反斜线、一个新行字符（newline)、若干空格（space)、一个b、一个反斜线、一个n、一个c、一对小括号、一个双引号、一个新行字符，以及若干空格。
        定义正则表达式（regular expression）时raw string literal特别有用。

    编码的（Encoded) String Literal //字符串字面值

    只要使用编码前缀(encoding prefix),你就可以为string literal定义一个特殊的字符编码 (character encoding)下面这些编码前缀都预先定义好了 ： 
    u8定义一个UTF-8编码。UTF-8 string literal以UTF-8编定的某个给定字符起头，字符类型为 const char。 
    u定义一个string literal,带着类型为chiarl6_t的字符。 
    U定义一个string literal，带着类型为char32_t的字符。 
    L定义一个wide string literal,带着类型为wchar_t的字符。 
    例如： 
    L” hello ” //defines.‘hello ‘‘aswchar_t stringliteral 
    Raw string幵头那个R的前面还町以放置一个编码前缀。 


   
