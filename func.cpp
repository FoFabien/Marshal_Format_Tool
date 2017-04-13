#include "func.hpp"
#include <windows.h>
#include <sstream>

std::string dec2str(const int64_t i)
{
    std::stringstream o;
    o << i;
    return o.str();
}

std::string dec2hex(const int64_t i)
{
    std::stringstream o;
    o << std::hex << i;
    return o.str();
}

int32_t str2int(const std::string &i)
{
    std::stringstream c(i);
    int32_t o;
    c >> o;
    return o;
}

std::wstring str2wstr(const std::string &i)
{
    std::wstring o;
    for(size_t k = 0; k < i.size(); ++k) o += i[k];
    return o;
}

std::string wstr2str(const std::wstring &i)
{
    std::string o;
    for(size_t k = 0; k < i.size(); ++k) o += (char)(i[k]);
    return o;
}

std::wstring utf8ToWchar(const std::vector<uint8_t> &in)
{
    std::wstring out;
    uint32_t r;
    char state = 0;
    for(size_t i = 0; i < in.size(); ++i)
    {
        switch(state)
        {
            case 0:
                r = in[i];
                if(in[i] < 0xC2)
                    out += (wchar_t)r;
                else if(in[i] >= 0xC2 && in[i] <= 0xDF) // 2 bytes
                    state = 11;
                else if(in[i] >= 0xE0 && in[i] <= 0xEF) // 3 bytes
                    state = 21;
                else if(in[i] >= 0xF0 && in[i] <= 0xF4) // 4 bytes
                    state = 31;
                break;
            case 11:
                r = (r << 8) + in[i];
                r = (r % 0b1000000) + (((r / 0x100) % 0b100000) << 6);
                out += (wchar_t)r;
                state = 0;
                break;
            case 21:
                r = (r << 8) + in[i];
                state = 22;
                break;
            case 22:
                r = (r << 8) + in[i];
                r = (r % 0b1000000) + (((r / 0x100) % 0b1000000) << 6) + (((r / 0x10000) % 0b10000) << 12);
                out += (wchar_t)r;
                state = 0;
                break;
            case 31:
                r = (r << 8) + in[i];
                state = 32;
                break;
            case 32:
                r = (r << 8) + in[i];
                state = 33;
                break;
            case 33:
                r = (r << 8) + in[i];
                r = (r % 0b1000000) + (((r / 0x100) % 0b1000000) << 6)  + (((r / 0x10000) % 0b1000000) << 12) + (((r / 0x1000000) % 0b1000) << 18);
                out += (wchar_t)r;
                state = 0;
                break;
        }
    }
    return out;
}

std::vector<uint8_t> wcharToUtf8(const std::wstring &in)
{
    std::vector<uint8_t> out;
    uint16_t c;
    for(size_t i = 0; i < in.size(); ++i)
    {
        c = (uint16_t)in[i];
        if(c >= 0x800 && c <= 0xFFFF)
        {
            out.push_back(0b11100000 + ((c / 0b1000000000000) % 0b10000));
            out.push_back(0b10000000 + ((c / 0b1000000) % 0b1000000));
            out.push_back(0b10000000 + (c % 0b1000000));
        }
        else if(c >= 0x80 && c <= 0x7FF)
        {
            out.push_back(0b11000000 + ((c / 0b1000000) % 0b100000));
            out.push_back(0b10000000 + (c % 0b1000000));
        }
        else if(c <= 0x7F)
        {
            out.push_back(c % 0b10000000);
        }
    }
    return out;
}

std::wstring getNumber(int current, int max)
{
    int a = 0;
    int b = 0;
    int c = current;
    while(max >= 10)
    {
        a++;
        max /= 10;
    }
    while(c >= 10)
    {
        b++;
        c /= 10;
    }
    std::wstring result = str2wstr(dec2str(current));

    for(int i = 0 ; i < a - b; ++i)
        result.insert(result.begin(), '0');
    return result;
}

std::vector<std::string> split(const std::string &s, std::vector<char> delim)
{
    std::vector<std::string> elems;
    split(s, delim, elems);
    return elems;
}

std::vector<std::string> split(const std::string &s, std::vector<char> delim, std::vector<std::string> &elems)
{
    std::string item;
    bool ok;

    for(uint32_t i = 0; i < s.size(); ++i)
    {
        ok = true;
        for(uint32_t j = 0; j < delim.size() && ok; ++j)
            if(s[i] == delim[j]) ok = false;
        if(ok) item += s[i];
        else
        {
            elems.push_back(item);
            item.clear();
        }
    }
    if(!item.empty()) elems.push_back(item);
    return elems;
}

bool isNumber(std::string s)
{
    if(s.empty()) return false;
    if(s.size() > 1 && s[0] == '-') s.erase(s.begin());
    std::string::const_iterator it = s.begin();
    while (it != s.end() && std::isdigit(*it)) ++it;
    return (!s.empty() && it == s.end());
}
