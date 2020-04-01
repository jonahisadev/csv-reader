#include <iostream>
#include <fstream>
#include <string>
#include <vector>

struct Column
{
    std::string name;
    std::vector<std::string> contents;
    int max_width = 0;
};

typedef std::vector<Column> Table;

std::string read_file(const std::string& path)
{
    std::string buf;
    std::ifstream file(path);

    if (file.is_open()) {
        std::string line;
        while (!file.eof()) {
            std::getline(file, line);
            buf += line + '\n';
        }
    }

    file.close();
    return std::string(buf);
}

// Expects trimmed line
std::vector<std::string> parse_line(const std::string& line)
{
    std::vector<std::string> words;
    std::string buf;
    for (char c : line) {
        if (c == ',') {
            words.push_back(buf);
            buf = "";
        } else {
            buf += c;
        }
    }
    if (!buf.empty())
        words.push_back(buf);
    return words;
}

void print_table(const Table& table)
{
    // Total width
    int total_width = table.size() + 1;
    for (auto& col : table)
        total_width += col.max_width + 2;

    // Top border
    for (int i = 0; i < total_width; i++)
        std::cout << "-";
    std::cout << std::endl;

    // Header
    for (auto& col : table) {
        std::cout << "| ";
        int width = col.name.length();
        std::cout << "\033[1m" << col.name << "\033[0m";
        int remainder = col.max_width - width;
        for (int x = 0; x < remainder + 1; x++)
            std::cout << " ";
    }
    std::cout << "|" << std::endl;

    // Divider
    std::cout << "|";
    for (int i = 0; i < total_width - 2; i++)
        std::cout << "-";
    std::cout << "|" << std::endl;

    // Contents
    for (int i = 0; i < table[0].contents.size(); i++) {
        for (auto& col : table) {
            std::cout << "| ";
            int width = col.contents[i].length();
            std::cout << col.contents[i];
            int remainder = col.max_width - width;
            for (int x = 0; x < remainder + 1; x++)
                std::cout << " ";
        }
        std::cout << "|" << std::endl;
    }

    // Bottom border
    for (int i = 0; i < total_width; i++)
        std::cout << "-";
    std::cout << std::endl;
}

int main(int argc, char** argv)
{
    if (argc < 2) {
        std::cerr << "Not enough arguments" << std::endl;
        return -1;
    }

    std::string contents = read_file(argv[1]);
    Table cols;

    int count = 0;
    std::string line;
    for (char c : contents) {
        if (c == '\n' || c == '\0') {
            if (line.empty())
                continue;

            std::vector<std::string> words = parse_line(line);

            // Header
            if (count == 0) {
                for (const auto& name : words) {
                    Column col;
                    col.name = name;
                    col.max_width = name.length();
                    cols.push_back(col);
                }
            }

            // Contents
            else {
                for (int i = 0; i < cols.size(); i++) {
                    cols[i].contents.push_back(words[i]);
                    if (words[i].length() > cols[i].max_width)
                        cols[i].max_width = words[i].length();
                }
            }

            count++;
            line = "";
        } else {
            line += c;
        }
    }

    print_table(cols);

    return 0;
}