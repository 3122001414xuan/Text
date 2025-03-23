#include "Text.h"
#include <iostream>

namespace mt
{
    Text::Text()
    {
    }
    Text::Text(const char* str)
    {
        this->length = StrLen(str);
        this->text = new char[this->length + 1];
        StrCopy(this->text, str);
    }
    Text::Text(const std::string& str)
    {
        this->length = str.length();
        this->text = new char[this->length + 1];
        StrCopy(this->text, str.c_str());
    }
    Text::Text(const wchar_t* str)
    {

        std::string s = WStringToString(str);
        this->length = s.length();
        this->text = new char[this->length + 1];
        StrCopy(this->text, s.c_str());
    }
    Text::Text(const std::wstring& str)
    {
        std::string s = WStringToString(str);
        this->length = s.length();
        this->text = new char[this->length + 1];
        StrCopy(this->text, s.c_str());
    }
    Text::Text(const Text& other)
    {
        this->length = other.length;
        this->text = new char[this->length + 1];
        StrCopy(this->text, other.text);
    }
    Text::Text(Text&& other)  noexcept
    {
        this->length = other.length;
        this->text = other.text;
        other.length = 0;
        other.text = nullptr;
    }
    Text::~Text()
    {
        delete[] this->text;
        length = 0;
    }

    bool Text::operator==(const Text& other) const
    {
        if (this->length != other.length)
        {
            return false;
        }
        for (int i = 0; i < this->length; i++)
        {
            if (this->text[i] != other.text[i])
            {
                return false;
            }
        }
        return true;
    }
    bool Text::operator!=(const Text& other) const
    {
        return !(*this == other);
    }
    Text Text::operator+(const Text& other)
    {
        char* new_text= CAC(this->text, other.text);
        Text t(new_text);
        delete[] new_text;
        return t;
    }
    Text& Text::operator+=(const Text& other)
    {
        char* new_text= CAC(this->text, other.text);
        delete[] this->text;
        this->text = new_text;
        this->length += other.length;
        return *this;
    }
    Text& Text::operator<<(Text& other) {
        char* new_text = new char[this->length + other.length + 1];
        new_text = CAC(this->text, other.text);
        delete[] this->text;
        this->text = new_text;
        this->length += other.length;
        return *this;
    }
    Text& Text::operator=(const Text& other)
    {
        if (this != &other)
        {
            delete[] this->text;
            this->length = other.length;
            this->text = new char[this->length + 1];
            StrCopy(this->text, other.text);
        }
        return *this;
    }
    Text& Text::operator=(Text&& other) noexcept
    {
        if (this != &other)
        {
            delete[] this->text;
            this->length = other.length;
            this->text = other.text;
            other.length = 0;
            other.text = nullptr;
        }
        return *this;
    }
    char& Text::operator[](int index)
    {
        if (index < 0)
        {
            index = this->length + index;
        }
        if (index >= this->length)
        {
            std::cerr << "Error: index out of range." << std::endl;
        }
        return (this->text[index]);
    }
    //输入输出运算符重载
    std::ostream& operator<<(std::ostream& os, const Text& t)
    {
        if (t.text == nullptr)
        {
            os << "nullptr";
        }
        else
        {
            os << t.text;
        }
        return os;
    }
    std::wostream& operator<<(std::wostream& os, const Text& t)
    {
        std::string s = t.to_string();
        std::wstring wsr = StringToWString(s);
        os << wsr;
        return os;
    }
    std::istream& operator>>(std::istream& is, Text& t)
    {
        std::string s;
        std::getline(is, s);
        t = Text(s);
        return is;
    }
    const char* Text::to_char() const
    {
        return this->text;
    }
    const wchar_t* Text::to_wchar() const
    {
        return CToW(this->text);
    }
    std::string Text::to_string() const
    {
        return std::string(this->text);
    }
    std::wstring Text::to_wstring() const
    {
        std::string s = this->to_string();
        std::wstring wsr = StringToWString(s);
        return wsr;
    }
    int Text::to_int() const
    {
        return std::stoi(this->text);
    }
    float Text::to_float() const
    {
        return std::stof(this->text);
    }
    double Text::to_double() const
    {
        return std::stod(this->text);
    }
    long long Text::to_longlong() const
    {
        return std::stoll(this->text);
    }
    Text Text::join(const std::vector<Text>& strs, const Text delimiter) const
    {
        Text result;
        for (size_t i = 0; i < strs.size(); i++)
        {
            if (result.length == 0)
            {
                result = strs[i];
            }
            else
            {
                result = result + delimiter + strs[i];
            }
        }
        return result;
    }
    int Text::find(const Text& str, int start) const
    {
        int pos = -1;
        for (int i = start; i < this->length; i++)
        {
            if (this->text[i] == str.text[0])
            {
                int j;
                for (j = 1; j < str.length; j++)
                {
                    if (i + j >= this->length || this->text[i + j] != str.text[j])
                    {
                        break;
                    }
                }
                if (j == str.length)
                {
                    pos = i;
                    break;
                }
            }
        }
        return pos;
    }
    int Text::rfind(const Text& str, int start) const
    {
        int pos = -1;
        for (int i = start; i >= 0; i--)
        {
            if (this->text[i] == str.text[0])
            {
                int j;
                for (j = 1; j < str.length; j++)
                {
                    if (i - j < 0 || this->text[i - j] != str.text[j])
                    {
                        break;
                    }
                }
                if (j == str.length)
                {
                    pos = i;
                    break;
                }
            }
        }
        return pos;
    }
    Text Text::substr(int start, int len) const
    {
        //将start和len转换为合法值
        if (length == 0||len==0)
        {
            return Text();
        }
        
        int s_start = std::abs(start) % length;
        if (start < 0)
        {
            start = length - s_start;
        }
        else
        {
            start = s_start;
        }

        int actualLen;
        if (len == 0) {
            actualLen = length - start;
        }
        else {
            actualLen = std::abs(len);  // 获取实际要截取的长度（取绝对值）
        }

        char* new_text = new char[actualLen + 1];
        new_text[actualLen] = '\0';  // 先设置字符串结束符

        int index = start;
        int newIndex = 0;
        if (len >= 0) {  // 正向截取（包含len为0的情况，此时取到末尾）
            for (int i = 0; i < actualLen; ++i) {
                if (index >= this->length) {
                    index = 0;  // 循环到字符串开头
                }
                new_text[newIndex++] = this->text[index++];
            }
        }
        else {  // 反向截取
            for (int i = 0; i < actualLen; ++i) {
                if (index < 0) {
                    index = this->length - 1;  // 循环到字符串末尾
                }
                new_text[newIndex++] = this->text[index--];
            }
        }

        Text t(new_text);
        delete[] new_text;
        return t;
    }
    std::vector<Text>Text::split(const Text delimiter) const
    {
        std::vector<Text> strs;
        int start = 0;
        if (delimiter.length == 0)
        {
            strs.push_back(*this);
            return strs;
        }
        while (1)
        {
            int result = this->find(delimiter, start);
            if (result == -1)
            {
                Text tt = this->substr(start, this->length - start);
                std::cout << start << " " << this->length - start << std::endl;
                strs.push_back(tt);
                break;
            }
            else
            {
                Text tt = this->substr(start, result - start);
                std::cout<<start<<" "<<result << std::endl;
                strs.push_back(tt);
                start = result + delimiter.length;
            }
        }
        return strs;

    }
    std::vector<Text>Text::split(const Text star, const Text end) const
    {
        std::vector<Text> strs;
        int pos = 0;
        while (1)
        {
            int re1 = this->find(star, pos);
            if (re1 == -1)
            {
                break;
            }
            int re2 = this->find(end, re1 + star.length);
            if (re2 == -1)
            {
                break;
            }
            Text tt = this->substr(re1 + star.length, re2 - re1 - star.length);
            strs.push_back(tt);
            pos = re2 + end.length;
        }
        return strs;
    }
    Text Text::replace(const Text& old_str, const Text& new_str) const
    {
        int pos = 0;
        if (old_str.length == 0)
        {
            return *this;
        }
        Text new_text;
        while (1)
        {
            int result = this->find(old_str, pos);
            if (result == -1)
            {
                break;
            }
            else
            {
                new_text << this->substr(pos, result - pos) << new_str;
                pos = result + old_str.length;
            }
        }
        new_text << this->substr(pos, this->length - pos);
        return new_text;

    }
    Text Text::replace(const Text& star, const Text& end, const Text& new_str)const
    {
        int pos = 0;
        Text new_text;
        while (1)
        {
            int re1 = this->find(star, pos);
            if (re1 == -1)
            {
                break;
            }
            int re2 = this->find(end, re1 + star.length);
            if (re2 == -1)
            {
                break;
            }
            new_text << this->substr(pos, re1 - pos) << new_str;
            pos = re2 + end.length;
        }
        new_text << this->substr(pos, this->length - pos);
        return new_text;
    }
    Text Text::replace(int start, int len, const Text& new_str)const
    {
        Text new_text;
        if (start < 0)
        {
            start = length + start;
        }
        else if (start > length)
        {
            std::cerr << "Error: start index out of range." << std::endl;
            return Text();
        }
        new_text << this->substr(0, start) << new_str << this->substr(start + len);
        return new_text;
    }
    bool Text::startsWith(const Text& str) const
    {
        for (int i = 0; i < str.length; i++)
        {
            if (i >= this->length || this->text[i] != str.text[i])
            {
                return false;
            }
        }
        return true;
    }
    bool Text::endsWith(const Text& str) const
    {
        for (int i = 0; i < str.length; i++)
        {
            if (i >= this->length || this->text[this->length - i - 1] != str.text[str.length - i - 1])
            {
                return false;
            }
        }
        return true;
    }
    Text Text::toLower() const
    {
        Text new_text;
        for (int i = 0; i < this->length; i++)
        {
            if (this->text[i] >= 'A' && this->text[i] <= 'Z')
            {
                new_text << std::string(1, char(this->text[i] + 32));
            }
            else
            {
                new_text << std::string(1, char(this->text[i]));
            }
        }
        return new_text;
    }
    Text Text::toUpper() const
    {
        Text new_text;
        for (int i = 0; i < this->length; i++)
        {
            if (this->text[i] >= 'a' && this->text[i] <= 'z')
            {
                new_text << std::string(1, char(this->text[i] - 32));
            }
            else
            {
                new_text << std::string(1, char(this->text[i]));
            }
        }
        return new_text;
    }
    bool Text::isDigit() const
    {
        bool flag = true;
        for (int i = 0; i < this->length; i++)
        {
            if (this->text[i] >= '0' && this->text[i] <= '9' || this->text[i] == '.' || this->text[i] == '+' || this->text[i] == '-')
            {
                continue;
            }
            else
            {
                flag = false;
                break;
            }
        }
        return flag;
    }
    bool Text::isLetter() const
    {
        for (int i = 0; i < this->length; i++)
        {
            if ((this->text[i] < 'a' || this->text[i] > 'z') && (this->text[i] < 'A' || this->text[i] > 'Z'))
            {
                return false;
            }
        }
        return true;
    }
    bool Text::isLetterOrDigit() const
    {
        for (int i = 0; i < this->length; i++)
        {
            if ((this->text[i] < '0' || this->text[i] > '9') && (this->text[i] < 'a' || this->text[i] > 'z') && (this->text[i] < 'A' || this->text[i] > 'Z'))
            {
                return false;
            }
        }
        return true;
    }
    bool Text::isSpace() const
    {
        for (int i = 0; i < this->length; i++)
        {
            if (this->text[i] != ' ' && this->text[i] != '\t' && this->text[i] != '\n' && this->text[i] != '\r')
            {
                return false;
            }
        }
        return true;
    }
    int Text::compare(const Text& other) const
    {
        int len1 = this->length;
        int len2 = other.length;
        int len = len1 < len2 ? len1 : len2;
        for (int i = 0; i < len; i++)
        {
            if (this->text[i] < other.text[i])
            {
                return -1;
            }
            else if (this->text[i] > other.text[i])
            {
                return 1;
            }
        }
        if (len1 < len2)
        {
            return -1;
        }
        else if (len1 > len2)
        {
            return 1;
        }
        else
        {
            return 0;
        }
    }
    // 判断整个Text对象的文本内容是否匹配给定正则表达式
    bool Text::match(const Text& pattern) const {
        try {
            std::regex re(pattern.to_string());
            return std::regex_match(this->to_string(), re);
        }
        catch (std::regex_error& e) {
            std::cerr << "正则表达式错误: " << e.what() << std::endl;
            return false;
        }
    }
    // 在Text对象的文本内容中查找是否存在匹配给定正则表达式的子串
    bool Text::search(const Text& pattern) const {
        try {
            std::regex re(pattern.to_string());
            return std::regex_search(this->to_string(), re);
        }
        catch (std::regex_error& e) {
            std::cerr << "正则表达式错误: " << e.what() << std::endl;
            return false;
        }
    }
    std::vector<Text> Text::extract(const Text& pattern) const {
        std::vector<Text> results;
        try {
            std::regex re(pattern.to_string());
            std::smatch matches;
            std::string textStr = this->to_string();
            auto it = textStr.cbegin();
            while (std::regex_search(it, textStr.cend(), matches, re)) {
                for (size_t i = 1; i < matches.size(); ++i) {  // 从第1个捕获组开始获取内容（第0个是整个匹配）
                    results.push_back(Text(matches[i].str().c_str()));
                }
                it = matches.suffix().first;  // 更新迭代器，继续查找下一处匹配
            }
        }
        catch (std::regex_error& e) {
            std::cerr << "正则表达式错误: " << e.what() << std::endl;
        }
        return results;
    }
    // 以下为辅助函数
    static const wchar_t*  CToW(const char* text) {
        std::string str(text);
        std::wstring wstr = StringToWString(str);
        wchar_t* wct = new wchar_t[wstr.length() + 1];
        for (int i = 0; i < wstr.length(); i++)
        {
            wct[i] = wstr[i];
        }
        wct[wstr.length()] = L'\0';
        return wct;
    }
    //const wchar_t*转char_t*
    static char* WToC(const wchar_t* wideStr) {

        if (wideStr == nullptr) {
            std::cerr << "Input wideStr is nullptr." << std::endl;
            return nullptr;
        }

        // 计算所需大小  
        size_t size = 0;
        errno_t err = wcstombs_s(&size, nullptr, 0, wideStr, _TRUNCATE);
        if (err != 0) {
            std::cerr << "Failed to get size of converted string. Error code: " << err << std::endl;
            return nullptr; // 转换失败，返回 nullptr  
        }

        // 分配内存  
        char* charStr = new char[size + 1]; // +1 for the null terminator  
        if (charStr == nullptr) {
            std::cerr << "Memory allocation failed." << std::endl;
            return nullptr; // 内存分配失败，返回 nullptr  
        }

        // 进行转换  
        size_t convertedChars = 0;
        err = wcstombs_s(&convertedChars, charStr, size + 1, wideStr, _TRUNCATE);
        if (err != 0) {
            std::cerr << "Conversion failed. Error code: " << err << std::endl;
            delete[] charStr; // 如果转换失败，释放内存  
            return nullptr; // 返回 nullptr  
        }

        return charStr; // 返回转换好的字符串  
    }
    //const string转wchar_t*
    static wchar_t* CsToW(const std::string s)
    {
        int len = 0;
        for (char c : s)
        {
            len++;
        }
        wchar_t* m = new wchar_t[len + 1];
        for (int i = 0; i < len; i++)
        {
            m[i] = wchar_t(s[i]);
        }
        m[len] = L'\0';
        return m;
    }
    //const wchar_t*转wchar_t*
    static wchar_t* CwToW(const wchar_t* text)
    {
        size_t len = wcslen(text);
        wchar_t* tt = new wchar_t[len + 1];
        for (int i = 0; i < len; i++)
        {
            tt[i] = text[i];
        }
        tt[len] = L'\0';
        return tt;
    }
    //const std::wstring转wchar_t*
    static wchar_t* WsToW(const std::wstring s)
    {
        int len = 0;
        for (wchar_t c : s)
        {
            len++;
        }
        wchar_t* m = new wchar_t[len + 1];
        for (int i = 0; i < len; i++)
        {
            m[i] = s[i];
        }
        m[len] = L'\0';
        return m;
    }
    
    //计算字符串长度
    int CPP_API StrLen(const char* str)
    {
        int len = 0;
        if (str == nullptr)
        {
            return 0;
        }
        while (str[len] != '\0')
        {
            len++;
        }
        return len;
    }
    //计算wstring长度
    int CPP_API WStrLen(const wchar_t* str)
    {
        int len = 0;
        if (str == nullptr)
        {
            return 0;
        }
        while (str[len] != L'\0')
        {
            len++;
        }
        return len;
    }
    //复制字符串
    void CPP_API StrCopy(char* str1, const char* str2)
    {
        for (int i = 0; i < StrLen(str2); i++)
        {
            str1[i] = str2[i];
        }
        str1[StrLen(str2)] = '\0';
    }
    static char* CAC(const char* str1, const char* str2)
    {
        int len1 = StrLen(str1);
        int len2 = StrLen(str2);
        char* str = new char[len1 + len2 + 1];
        StrCopy(str, str1);
        StrCopy(str + len1, str2);
        return str;
    }
    void  CPP_API Init()
    {
        std::wcin.imbue(std::locale("chs"));//载入中文字符输入方式
        std::wcout.imbue(std::locale("chs"));//载入中文字符输入方式
    }
}
//std::string WString2String(const std::wstring& ws)
//{
//    std::string strLocale = setlocale(LC_ALL, "");
//    const wchar_t* wchSrc = ws.c_str();
//    size_t nDestSize = wcstombs(NULL, wchSrc, 0) + 1;
//    char* chDest = new char[nDestSize];
//    memset(chDest, 0, nDestSize);
//    wcstombs(chDest, wchSrc, nDestSize);
//    std::string strResult = chDest;
//    delete[] chDest;
//    setlocale(LC_ALL, strLocale.c_str());
//    return strResult;
//}
//
//// string => wstring
//std::wstring String2WString(const std::string& s)
//{
//    std::string strLocale = setlocale(LC_ALL, "");
//    const char* chSrc = s.c_str();
//    size_t nDestSize = mbstowcs(NULL, chSrc, 0) + 1;
//    wchar_t* wchDest = new wchar_t[nDestSize];
//    wmemset(wchDest, 0, nDestSize);
//    mbstowcs(wchDest, chSrc, nDestSize);
//    std::wstring wstrResult = wchDest;
//    delete[] wchDest;
//    setlocale(LC_ALL, strLocale.c_str());
//    return wstrResult;
//}
std::wstring CPP_API StringToWString(std::string str)
{
    int len = MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, NULL, 0);
    if (len == 0)
        return std::wstring(L"");
    wchar_t* wct = new wchar_t[len];
    if (!wct)
        return std::wstring(L"");

    MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, wct, len);
    std::wstring wstr(wct);
    delete[] wct;
    wct = NULL;
    return wstr;
}
std::string CPP_API WStringToString(std::wstring wstr) {
    int len = WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), -1, NULL, 0, NULL, NULL);
    if (len == 0)
        return std::string("");
    char* ch = new char[len];
    if (!ch)
        return std::string("");

    WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), -1, ch, len, NULL, NULL);
    std::string str(ch);
    delete[] ch;
    ch = NULL;
    return str;
}
int main()
{
    mt::Init();
    mt::Text str1 = L"a你好a";
   std::vector<mt::Text> vec = str1.split(L"a");
   for (auto s : vec)
   {
       std::cout << s<< std::endl;
   }
    return 0;
}

