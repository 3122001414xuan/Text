#pragma once
#ifndef TEXT_H
#define TEXT_H
#include <iostream>
#include <string>
#include <regex>
#include <windows.h>  //这个头文件必须添加
#include <iomanip>
#include<vector>
#include <locale>
#include <codecvt> // macos必须，否则提示String2WString函数报错
namespace mt {
#define CPP_API  _declspec(dllexport)//定义导出符号

    int CPP_API StrLen(const char* str);
    int CPP_API WStrLen(const wchar_t* str);
    void CPP_API StrCopy(char* str1, const char* str2);
    static char* CAC(const char* str1, const char* str2);
    void  CPP_API Init();
    
    class CPP_API Text {
    public:
        //构造函数和析构函数
        Text();
        Text(const char* str);
        Text(const wchar_t* str);
        Text(const std::string& str);
        Text(const std::wstring& str);
        //算数类型转换构造函数（char, wchar_t, int, double, long long等）
        template<typename T>
        Text(T num)
        {
            if constexpr (std::is_arithmetic_v<T>) {
                std::string str_num = std::to_string(num);
                this->length = str_num.length();
                this->text = new char[this->length + 1];
                StrCopy(this->text, str_num.c_str());
            }
            else if constexpr (std::is_same_v<T, bool>) {
                this->length = 1;
                this->text = new char[this->length + 1];
                if (num) {
                    StrCopy(this->text, "true");
                }
                else {
                    StrCopy(this->text, "false");
                }
            }
            else if constexpr (std::is_same_v<T, char*>) {
                Text temp(static_cast<const char*>(num));
                *this = temp;
            }
            else if constexpr (std::is_same_v<T, wchar_t*>) {
                Text temp(static_cast<const wchar_t*>(num));
                *this = temp;
            }
            else if constexpr (std::is_same_v<T, std::string>) {
                Text temp(num.c_str());
                *this = temp;
            }
            else if constexpr (std::is_same_v<T, std::wstring>) {
                Text temp(num.c_str());
                *this = temp;
            }
            else {
                std::cerr << "Error: unsupported type." << std::endl;
            }

        }
        //拷贝构造函数
        Text(const Text& other);
        //移动构造函数
        Text(Text&& other) noexcept;
        //拷贝赋值运算符
        Text& operator=(const Text& other);
        //移动赋值运算符
        Text& operator=(Text&& other) noexcept;
        ~Text();

        //重载运算符
        bool operator==(const Text& other)const;
        bool operator!=(const Text& other)const;
        Text operator+(const Text& other);
        Text& operator+=(const Text& other);
        //等价于+=，
        Text& operator<<(Text& other);
        template<typename T>
        Text& operator<<(T str)
        {
            // 创建一个 Text 对象
            Text temp(str);
            // 调用已有的 operator<< 来合并
            return this->operator<<(temp);
        }
        char& operator[](int index);
        //转换函数
        const char* to_char()const;
        const wchar_t* to_wchar()const;
        std::string to_string()const;
        std::wstring to_wstring()const;
        int to_int()const;
        float to_float()const;
        double to_double()const;
        long long to_longlong()const;

        //输入输出
        friend std::ostream& operator<<(std::ostream& os, const Text& text);
        friend std::wostream& operator<<(std::wostream& os, const Text& text);
        //按行输入
        friend std::istream& operator>>(std::istream& is, Text& text);

        //字符串操作
        //连接
        Text join(const std::vector<Text>& strs, const Text delimiter) const;
        //分割
        std::vector<Text> split(const Text delimiter) const;
        std::vector<Text> split(const Text start, const Text end) const;
        //查找
        int find(const Text& str, int start = 0) const;
        int rfind(const Text& str, int start = 0) const;
        //替换
        Text replace(const Text& oldStr, const Text& newStr) const;
        Text replace(const Text& start, const Text& end, const Text& newStr) const;
        Text replace(int start, int len, const Text& newStr)const;
        //获取子串,循环取数，len为0时表示取到结尾
        Text substr(int start, int len = 0) const;
        //大小写转换
        Text toUpper() const;
        Text toLower() const;
        //前后缀判断
        bool startsWith(const Text& str) const;
        bool endsWith(const Text& str) const;
        //字符判断
        bool isDigit() const;
        bool isLetter() const;
        bool isLetterOrDigit() const;
        bool isSpace() const;
        //字符串比较
        int compare(const Text& str) const;

        //正则表达式相关
        // 判断整个Text对象的文本内容是否匹配给定正则表达式
        bool match(const Text& pattern) const;
        // 在Text对象的文本内容中查找是否存在匹配给定正则表达式的子串
        bool search(const Text& pattern) const;
        std::vector<Text> extract(const Text& pattern) const;
        //获取长度
        int getLength() { return length; }
    private:
        //wchar_t* text = nullptr;//用于存放文本内容
        char* text = nullptr;//用于存放文本内容
        int length = 0;//用于存放文本长度
    };

    //字符间转化
    //wchar_t*转char*
    static char* WToC(const wchar_t* str);
    //char*转wchar_t*
    static const wchar_t* CToW(const char* str);
    //wstring转wchar_t*
    static wchar_t* WsToW(const std::wstring s);
}
std::wstring CPP_API StringToWString(std::string s);
std::string CPP_API WStringToString(std::wstring wstr);
#endif
