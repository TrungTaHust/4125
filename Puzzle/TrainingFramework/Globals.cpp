#include "Globals.h"
#include <iostream>
#include <fstream>
#include <sstream>

const int Globals::screenWidth = 1280;
const int Globals::screenHeight = 960;
std::vector<int> Globals::topScores = {};
std::string Globals::map = "";

void DayData::print() const {
    std::cout << date << "\n";
    for (const auto& pair : values) {
        std::cout << pair.first << ": " << pair.second << "\n";
    }
}

std::vector<DayData> readFile() {
    std::ifstream file("data.txt");
    std::vector<DayData> data;

    if (!file.is_open()) {
        std::cerr << "Không thể mở file để đọc.\n";
        return data;
    }

    std::string line;
    while (std::getline(file, line)) {
        if (line.empty()) continue;

        DayData day;
        day.date = line; 

        while (std::getline(file, line) && !line.empty()) {
            std::istringstream ss(line);
            std::string key;
            int value;
            if (std::getline(ss, key, ':') && ss >> value) {
                day.values[key] = value;
            }
        }

        data.push_back(day);
    }

    file.close();
    return data;
}

void writeFile(const std::vector<DayData>& data) {
    std::ofstream file("data.txt", std::ios::trunc);

    if (!file.is_open()) {
        std::cerr << "Không thể mở file để ghi.\n";
        return;
    }

    for (const auto& day : data) {
        file << day.date << "\n";
        for (const auto& pair : day.values) {
            file << pair.first << ": " << pair.second << "\n";
        }
        file << "\n"; 
    }

    file.close();
}

void updateData(std::vector<DayData>& data, const DayData& newData) {
    for (auto& day : data) {
        if (day.date == newData.date) {
            for (const auto& pair : newData.values) {
                const std::string& key = pair.first;
                int newValue = pair.second;

                if (day.values[key] < newValue) {
                    day.values[key] = newValue; 
                }
            }
            return;
        }
    }
    data.push_back(newData);
}

std::string getTodayDate() {
    auto now = std::time(nullptr);
    std::tm* tm = std::localtime(&now);

    char buffer[11]; 
    std::strftime(buffer, sizeof(buffer), "%d/%m/%Y", tm);
    return std::string(buffer);
}

DayData createDefaultDayData(const std::string& date) {
    DayData dayData;
    dayData.date = date;
    dayData.values = {
        {"anim1", 0},
        {"anim2", 0},
        {"vehicles", 0},
        {"action", 0},
        {"number", 0},
        {"fruit", 0}
    };
    return dayData;
}