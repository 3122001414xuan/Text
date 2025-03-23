#pragma once
#ifndef TEXT_H
#define TEXT_H
#include <iostream>
#include <string>
#include <regex>
#include <windows.h>  //���ͷ�ļ��������
#include <iomanip>
#include<vector>
#include <locale>
#include <codecvt> // macos���룬������ʾString2WString��������
namespace mt {
#define CPP_API  _declspec(dllexport)//���嵼������

    int CPP_API StrLen(const char* str);
    int CPP_API WStrLen(const wchar_t* str);
    void CPP_API StrCopy(char* str1, const char* str2);
    static char* CAC(const char* str1, const char* str2);
    void  CPP_API Init();
    
    class CPP_API Text {
    public:
        //���캯������������
        Text();
        Text(const char* str);
        Text(const wchar_t* str);
        Text(const std::string& str);
        Text(const std::wstring& str);
        //��������ת�����캯����char, wchar_t, int, double, long long�ȣ�
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
        //�������캯��
        Text(const Text& other);
        //�ƶ����캯��
        Text(Text&& other) noexcept;
        //������ֵ�����
        Text& operator=(const Text& other);
        //�ƶ���ֵ�����
        Text& operator=(Text&& other) noexcept;
        ~Text();

        //���������
        bool operator==(const Text& other)const;
        bool operator!=(const Text& other)const;
        Text operator+(const Text& other);
        Text& operator+=(const Text& other);
        //�ȼ���+=��
        Text& operator<<(Text& other);
        template<typename T>
        Text& operator<<(T str)
        {
            // ����һ�� Text ����
            Text temp(str);
            // �������е� operator<< ���ϲ�
            return this->operator<<(temp);
        }
        char& operator[](int index);
        //ת������
        const char* to_char()const;
        const wchar_t* to_wchar()const;
        std::string to_string()const;
        std::wstring to_wstring()const;
        int to_int()const;
        float to_float()const;
        double to_double()const;
        long long to_longlong()const;

        //�������
        friend std::ostream& operator<<(std::ostream& os, const Text& text);
        friend std::wostream& operator<<(std::wostream& os, const Text& text);
        //��������
        friend std::istream& operator>>(std::istream& is, Text& text);

        //�ַ�������
        //����
        Text join(const std::vector<Text>& strs, const Text delimiter) const;
        //�ָ�
        std::vector<Text> split(const Text delimiter) const;
        std::vector<Text> split(const Text start, const Text end) const;
        //����
        int find(const Text& str, int start = 0) const;
        int rfind(const Text& str, int start = 0) const;
        //�滻
        Text replace(const Text& oldStr, const Text& newStr) const;
        Text replace(const Text& start, const Text& end, const Text& newStr) const;
        Text replace(int start, int len, const Text& newStr)const;
        //��ȡ�Ӵ�,ѭ��ȡ����lenΪ0ʱ��ʾȡ����β
        Text substr(int start, int len = 0) const;
        //��Сдת��
        Text toUpper() const;
        Text toLower() const;
        //ǰ��׺�ж�
        bool startsWith(const Text& str) const;
        bool endsWith(const Text& str) const;
        //�ַ��ж�
        bool isDigit() const;
        bool isLetter() const;
        bool isLetterOrDigit() const;
        bool isSpace() const;
        //�ַ����Ƚ�
        int compare(const Text& str) const;

        //������ʽ���
        // �ж�����Text������ı������Ƿ�ƥ�����������ʽ
        bool match(const Text& pattern) const;
        // ��Text������ı������в����Ƿ����ƥ�����������ʽ���Ӵ�
        bool search(const Text& pattern) const;
        std::vector<Text> extract(const Text& pattern) const;
        //��ȡ����
        int getLength() { return length; }
    private:
        //wchar_t* text = nullptr;//���ڴ���ı�����
        char* text = nullptr;//���ڴ���ı�����
        int length = 0;//���ڴ���ı�����
    };

    //�ַ���ת��
    //wchar_t*תchar*
    static char* WToC(const wchar_t* str);
    //char*תwchar_t*
    static const wchar_t* CToW(const char* str);
    //wstringתwchar_t*
    static wchar_t* WsToW(const std::wstring s);
}
std::wstring CPP_API StringToWString(std::string s);
std::string CPP_API WStringToString(std::wstring wstr);
#endif
