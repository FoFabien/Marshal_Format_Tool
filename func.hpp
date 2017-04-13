#ifndef FUNC_HPP
#define FUNC_HPP

#include <string>
#include <stdint.h>
#include <vector>

std::string dec2str(const int64_t i);
std::string dec2hex(const int64_t i);
int32_t str2int(const std::string &i);
std::wstring str2wstr(const std::string &i);
std::string wstr2str(const std::wstring &i);
std::wstring utf8ToWchar(const std::vector<uint8_t> &in);
std::vector<uint8_t> wcharToUtf8(const std::wstring &in);
std::wstring getNumber(int current, int max);
std::vector<std::string> split(const std::string &s, std::vector<char> delim);
std::vector<std::string> split(const std::string &s, std::vector<char> delim, std::vector<std::string> &elems);
bool isNumber(std::string s);

#endif // FUNC_HPP
