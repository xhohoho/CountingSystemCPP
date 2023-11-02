#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <iomanip>
#include <ctime>
#include <sstream>
#include <windows.h>
#include <Windows.h>
#include <chrono>
#include <thread>

#ifdef _WIN32
#include <conio.h>
#else
#include <unistd.h>
#include <termios.h>
#endif

bool isKeyPressed(int key) {
    return (GetAsyncKeyState(key) & 0x8000) != 0;
}

// Function to read a single character without waiting for Enter
char readChar() {
    char ch = 0;
    if (_kbhit()) { // Check if a key has been pressed
        ch = _getch(); // Read the key
    }
    return ch;
}

// Constants
const std::string CSV_FILENAME = "count.csv";

void setscreen()
{
    HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);

    // Get current console font information
    CONSOLE_FONT_INFOEX fontInfo;
    fontInfo.cbSize = sizeof(CONSOLE_FONT_INFOEX);
    GetCurrentConsoleFontEx(consoleHandle, FALSE, &fontInfo);

    // Set the desired terminal size
    COORD size;
    size.X = 55; // Width of 8 columns
    size.Y = 10;  // Height of 2 rows

    // Set the terminal size
    SMALL_RECT rect;
    rect.Left = 0;
    rect.Top = 0;
    rect.Right = size.X - 1;
    rect.Bottom = size.Y - 1;  // Adjusted for two lines
    SetConsoleWindowInfo(consoleHandle, TRUE, &rect);
    SetConsoleScreenBufferSize(consoleHandle, size);

    // Set the updated console font
    SetCurrentConsoleFontEx(consoleHandle, FALSE, &fontInfo);
}

void clearScreen()
{
#ifdef _WIN32
    std::system("cls");
#else
    std::system("clear");
#endif
}

void lockFile(const std::string& filename)
{
    std::wstring wfilename(filename.begin(), filename.end());
    SetFileAttributes(wfilename.c_str(), FILE_ATTRIBUTE_READONLY);
}

void unlockFile(const std::string& filename)
{
    std::wstring wfilename(filename.begin(), filename.end());
    SetFileAttributes(wfilename.c_str(), FILE_ATTRIBUTE_NORMAL);
}

// Function to save counts to CSV file
void saveCounts(int vrcounts,int pcbcounts)
{
    std::ofstream file(CSV_FILENAME, std::ios::out | std::ios::app); // Open file for writing and append to existing content
    if (file.is_open()) {
        std::time_t currentTime = std::time(nullptr);
        std::tm localTime;
        if (localtime_s(&localTime, &currentTime) == 0) {
            char timeBuffer[80];
            std::strftime(timeBuffer, sizeof(timeBuffer), "%m/%d/%Y %H:%M:%S", &localTime);
            file << timeBuffer << "," << vrcounts << "," << pcbcounts << std::endl;
            file.close();
            std::cout << "Count saved to " << CSV_FILENAME << std::endl;
        }
        else {
            std::cout << "Error getting local time!" << std::endl;
        }
    }
    else {
        std::cout << "Error saving count to file!" << std::endl;
    }
}

void loadCounts(int& vrcounts,int& pcbcounts, const std::string& targetDate, std::vector<std::string>& dates)
{

    std::ifstream file(CSV_FILENAME);
    if (file.is_open()) {
        vrcounts = 0; // Reset counts vector to all zeros
        pcbcounts = 0;
        std::string line;
        int lastvrcount;
        int lastpcbcount;
        while (std::getline(file, line)) {
            if (!line.empty()) {
                std::istringstream iss(line);
                std::string token;
                std::string token1;
                std::string token2;
                std::getline(iss, token, ','); // Read the date
                std::string countDate = token.substr(0, 10); // Extract only the date portion
                dates.push_back(countDate);
                if (countDate == targetDate) {
                    std::getline(iss, token1, ','); // Read the count for each point
                    std::getline(iss, token2, ','); // Read the count for each point
                    lastvrcount = std::stoi(token1);
                    lastpcbcount = std::stoi(token2);
                    vrcounts = lastvrcount;
                    pcbcounts = lastpcbcount;
                }
            }
        }

        file.close();
    }
    else {

    }
}


int main()
{
    SetConsoleTitleA("Counting System");
    std::system("color F0");

    std::vector<std::string> dates;
    char input = 'x';
    bool toggle = true;
    setscreen();

    do {
        std::system("cls");
        int vrcounts = 0;
        int pcbcounts = 0;


        std::time_t currentTime = std::time(nullptr);
        std::tm localTime;
        if (localtime_s(&localTime, &currentTime) == 0) {
            char timeBuffer[80];
            std::strftime(timeBuffer, sizeof(timeBuffer), "%m/%d/%Y", &localTime);
            std::string currentDate = timeBuffer;

            loadCounts(vrcounts, pcbcounts, currentDate.substr(0, 10), dates);
            std::cout << "|===========================================|" << std::endl;
            std::cout << "|Insert any character to change menu...     |" << std::endl;
            std::cout << "|===========================================|" << std::endl << std::endl;
            std::cout << "Please select your option : " ;

            int vrcount;
            int pcbcount;
            int total1;
            int total2;

            if (toggle==true)
            {
                std::cout << "You selected A for VR " << std::endl;
                std::cout << "Entered how many counts: ";
                std::cin >> vrcount;

                if (vrcount==1122)
                {
                    std::cout << "Resetting..." << std::endl;
                    vrcounts = 0;
                    pcbcounts = 0;
                    unlockFile(CSV_FILENAME);
                    saveCounts(vrcounts, pcbcounts); // Save count to CSV file
                    lockFile(CSV_FILENAME);
                    continue;
                }

                if (vrcount)
                {
                    total1 = vrcounts + vrcount;
                    unlockFile(CSV_FILENAME);
                    saveCounts(total1, pcbcounts); // Save count to CSV file
                    lockFile(CSV_FILENAME);
                }


                if (!vrcount)
                {
                    std::cout << "Invalid input. Please enter a valid number!";
                    std::cin.clear();
                    std::cin.ignore();
                    toggle = !toggle;
                    continue;
                }
            }

            if (toggle==false)
            {
                std::cout << "You selected B for PCB " << std::endl;
                std::cout << "Entered how many counts: ";
                std::cin >> pcbcount;

                if (pcbcount == 1122)
                {
                    std::cout << "Resetting..." << std::endl;
                    vrcounts = 0;
                    pcbcounts = 0;
                    unlockFile(CSV_FILENAME);
                    saveCounts(vrcounts, pcbcounts); // Save count to CSV file
                    lockFile(CSV_FILENAME);
                    continue;
                }


                if (pcbcount)
                {
                    total2 = pcbcounts + pcbcount;
                    unlockFile(CSV_FILENAME);
                    saveCounts(vrcounts, total2); // Save count to CSV file
                    lockFile(CSV_FILENAME);
                }

                if (!pcbcount)
                {
                    std::cout << "Invalid input. Please enter a valid number!";
                    std::cin.clear();
                    std::cin.ignore();
                    toggle = !toggle;
                    continue;
                }
            }

            if (input == 'q' || input == 'Q') {
                return 0;
            }

            if (input == 'r' || input == 'R') {
                std::cout << "Resetting..."<< std::endl;
                vrcounts = 0;
                pcbcounts = 0;
                unlockFile(CSV_FILENAME);
                saveCounts(vrcounts, pcbcounts); // Save count to CSV file
                lockFile(CSV_FILENAME);
            }

            std::this_thread::sleep_for(std::chrono::microseconds(500)); // Add a 1-second delay
        }
        else {
            std::cout << "Error getting local time!" << std::endl;
        }

    } while (true);

    return 0;
}
