#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <iomanip>
#include <ctime>
#include <sstream>
#include <windows.h>
#include <chrono>
#include <thread>

// Constants
const int NUM_POINTS = 15;
const std::string CSV_FILENAME = "count.csv";
const std::string CSV_FILENAME2 = "count2.csv";

// Function to clear the terminal screen
void clearScreen()
{
#ifdef _WIN32
    std::system("cls");
#else
    std::system("clear");
#endif
}


void displayTable(int& vrcounts, int& pcbcounts)
{
    HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);

    // Get current console font information
    CONSOLE_FONT_INFOEX fontInfo;
    fontInfo.cbSize = sizeof(CONSOLE_FONT_INFOEX);
    GetCurrentConsoleFontEx(consoleHandle, FALSE, &fontInfo);

    // Set the desired font size
    fontInfo.dwFontSize.X = 0;  // Width of 0 means to use the default width
    fontInfo.dwFontSize.Y = 25; // Set the desired font height

    // Set the desired terminal size
    COORD size;
    size.X = 15; // Width of 8 columns
    size.Y = 2; // Height of 1 row

    // Set the terminal size
    SMALL_RECT rect;
    rect.Left = 0;
    rect.Top = 0;
    rect.Right = size.X - 1;
    rect.Bottom = size.Y - 1;
    SetConsoleWindowInfo(consoleHandle, TRUE, &rect);
    SetConsoleScreenBufferSize(consoleHandle, size);

    // Set the updated console font
    SetCurrentConsoleFontEx(consoleHandle, FALSE, &fontInfo);

    if (vrcounts > 0 && vrcounts < 8)
    {
        std::cout << "\033[32m" << "\033[107m" << "VR Count: " << vrcounts << std::endl;
        if (pcbcounts > 0 && pcbcounts < 8)
        {
        std::cout << "\033[32m" << "\033[107m" << "PCB Count: " << pcbcounts;
        }
        else if (pcbcounts >= 8 && pcbcounts < 20)
        {
            std::cout << "\033[33m" << "\033[107m" << "PCB Count: " << pcbcounts;
        }
        else if (pcbcounts >= 20)
        {
            std::cout << "\033[31m" << "\033[107m" << "PCB Count: " << pcbcounts;
        }

        else
        {
            std::cout << "\033[30m" << "\033[107m" << "PCB Count: ";
        }
    }

    else if (vrcounts >= 8 && vrcounts < 20)
    {
        std::cout << "\033[33m" << "\033[107m" << "VR Count: " << vrcounts << std::endl;

        if (pcbcounts > 0 && pcbcounts < 8)
        {
            std::cout << "\033[32m" << "\033[107m" << "PCB Count: " << pcbcounts;
        }
        else if (pcbcounts >= 8 && pcbcounts < 20)
        {
            std::cout << "\033[33m" << "\033[107m" << "PCB Count: " << pcbcounts;
        }
        else if (pcbcounts >= 20)
        {
            std::cout << "\033[31m" << "\033[107m" << "PCB Count: " << pcbcounts;
        }

        else
        {
            std::cout << "\033[30m" << "\033[107m" << "PCB Count: ";
        }
    }

    else if (vrcounts >= 20)
    {
        std::cout << "\033[31m" << "\033[107m" << "VR Count: " << vrcounts << std::endl;

        if (pcbcounts > 0 && pcbcounts < 8)
        {
            std::cout << "\033[32m" << "\033[107m" << "PCB Count: " << pcbcounts;
        }
        else if (pcbcounts >= 8 && pcbcounts < 20)
        {
            std::cout << "\033[33m" << "\033[107m" << "PCB Count: " << pcbcounts;
        }
        else if (pcbcounts >= 20)
        {
            std::cout << "\033[31m" << "\033[107m" << "PCB Count: " << pcbcounts;
        }

        else
        {
            std::cout << "\033[30m" << "\033[107m" << "PCB Count: ";
        }
    }

    else
    {
        std::cout << "\033[30m" << "\033[107m" << "VR Count: " << std::endl;
        if (pcbcounts > 0 && pcbcounts < 8)
        {
            std::cout << "\033[32m" << "\033[107m" << "PCB Count: " << pcbcounts;
        }
        else if (pcbcounts >= 8 && pcbcounts < 20)
        {
            std::cout << "\033[33m" << "\033[107m" << "PCB Count: " << pcbcounts;
        }
        else if (pcbcounts >= 20)
        {
            std::cout << "\033[31m" << "\033[107m" << "PCB Count: " << pcbcounts;
        }

        else
        {
            std::cout << "\033[30m" << "\033[107m" << "PCB Count: ";
        }
    }
}


// Function to load counts from CSV file for a specific date
void loadPoints(int& vrcounts, int& pcbcounts, const std::string& targetDate, std::vector<std::string>& dates) {
    std::ifstream file(CSV_FILENAME);
    std::string lastPoint;
    std::string lastCount;
    vrcounts = 0;
    pcbcounts = 0;

    if (file.is_open()) {
        std::string line;
        while (std::getline(file, line)) {
            if (!line.empty()) {
                std::istringstream iss(line);
                std::string dateTime, point, count;
                std::getline(iss, dateTime, ','); // Date and time
                std::string countDate = dateTime.substr(0, 10); // Extract only the date portion
                dates.push_back(countDate);

                if (countDate == targetDate)
                {
                    std::getline(iss, point, ','); // Point (2nd data)
                    std::getline(iss, count, ','); // Count
                    vrcounts = std::stoi(point);
                    pcbcounts = std::stoi(count);
                }
            }
        }

        file.close();
    }
}


int main()
{
    SetConsoleTitleA("Counting System");
    std::system("color F0");

    //std::vector<std::string> dates;
    int vrcounts = 0;
    int pcbcounts = 0;
    std::vector<std::string> dates;

    std::time_t currentTime = std::time(nullptr);
    std::tm localTime;
    if (localtime_s(&localTime, &currentTime) == 0) {
        char timeBuffer[80];
        std::strftime(timeBuffer, sizeof(timeBuffer), "%m/%d/%Y", &localTime);
        std::string currentDate = timeBuffer;

        do {
            std::system("cls");
            loadPoints(vrcounts, pcbcounts, currentDate, dates); // Load existing counts and dates from CSV file for the current date (only date portion)

            displayTable(vrcounts, pcbcounts);

            std::this_thread::sleep_for(std::chrono::seconds(1)); // Add a 1-second delay

        } while (true);
    }
    else {
        std::cout << "Error getting local time!" << std::endl;
    }

    return 0;
}