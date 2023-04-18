#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdint.h>
#include <stdio.h>
#include <string>
#include <vector>

class yyFileReader {
public:
    yyFileReader(const char* filename)
        : size_(0)
        , data_(nullptr)
        , error_(true)
    {
        if (filename == nullptr)
            return;

        FILE* fp = fopen(filename, "rb");
        if (fp == nullptr) {
            std::cout << "fopen failed" << std::endl;
            return;
        }

        fseek(fp, 0L, SEEK_END);
        int size = ftell(fp);
        fseek(fp, 0L, SEEK_SET);

        uint8_t* data = new uint8_t[size + 1];
        fread(data, size, 1, fp);
        data[size] = 0;  // add '\0'
        fclose(fp);

        data_ = data;
        size_ = size;
        error_ = false;
    }

    ~yyFileReader()
    {
        delete[] data_;
    }

    int size() { return size_; }

    uint8_t* data() { return data_; }

    uint8_t* data() const { return data_; }

    bool isError() { return error_; }

private:
    int size_;
    uint8_t* data_;
    bool error_;
};

class yyFileWriter {
public:
    static void save(const std::string &filename, std::vector<std::string> &lines)
    {
        std::ofstream outfile(filename);
        for (auto &line: lines) {
            outfile << line << std::endl;
        }
    }
};
