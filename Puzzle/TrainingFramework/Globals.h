#pragma once
#include <vector>
#include <string>
#include <map>
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <sstream>
#include <ctime>
#include <algorithm>
#include <iomanip>

// Cấu trúc dữ liệu cho một ngày
struct DayData {
    std::string date;
    std::map<std::string, int> values;

    void print() const;
};

// Lớp Globals lưu các thông tin chung
class Globals {
public:
    static const int screenWidth;
    static const int screenHeight;
    static std::vector<int> topScores;
    static std::string map;
};

// Các hàm xử lý dữ liệu file
std::vector<DayData> readFile();
void writeFile(const std::vector<DayData>& data);
void updateData(std::vector<DayData>& data, const DayData& newData);
std::string getTodayDate();
DayData createDefaultDayData(const std::string& date);

// Định nghĩa các phím
#define KEY_UP     VK_UP
#define KEY_DOWN   VK_DOWN
#define KEY_LEFT   VK_LEFT
#define KEY_RIGHT  VK_RIGHT
