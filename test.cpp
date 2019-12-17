#include <iostream>
#include <fstream>
#include <gtest/gtest.h>

#include "malhotra.h"

using std::string;

const string path_to_tests_dir = "/home/amir/Documents/Algorithms/Flows/tests/";

TEST(sample, first) {

    std::ifstream fin;
    std::string infile = path_to_tests_dir + "test_1.txt";
    fin.open(infile);

    if (!fin)
        exit(1);

    const long long flow = max_flow_mkm(fin);

    EXPECT_EQ(flow, 12);
}



TEST(sample, second) {

    std::ifstream fin;
    std::string infile = path_to_tests_dir + "test_2.txt";
    fin.open(infile);

    if (!fin)
        exit(1);

    const long long flow = max_flow_mkm(fin);

    EXPECT_EQ(flow, 10);
}


TEST(sample, third) {

    std::ifstream fin;
    std::string infile = path_to_tests_dir + "test_3.txt";
    fin.open(infile);

    if (!fin)
        exit(1);

    const long long flow = max_flow_mkm(fin);

    EXPECT_EQ(flow, 20);
}


TEST(sample, forth) {

    std::ifstream fin;
    std::string infile = path_to_tests_dir + "test_4.txt";
    fin.open(infile);

    if (!fin)
        exit(1);

    const long long flow = max_flow_mkm(fin);

    EXPECT_EQ(flow, 15);
}


TEST(sample, fivth) {

    std::ifstream fin;
    std::string infile = path_to_tests_dir + "test_5.txt";
    fin.open(infile);

    if (!fin)
        exit(1);

    const long long flow = max_flow_mkm(fin);

    EXPECT_EQ(flow, 14);
}


TEST(sample, sixth) {

    std::ifstream fin;
    std::string infile = path_to_tests_dir + "test_6.txt";
    fin.open(infile);

    if (!fin)
        exit(1);

    const long long flow = max_flow_mkm(fin);

    EXPECT_EQ(flow, 25);
}


TEST(sample, seventh) {

    std::ifstream fin;
    std::string infile = path_to_tests_dir + "test_7.txt";
    fin.open(infile);

    if (!fin)
        exit(1);

    const long long flow = max_flow_mkm(fin);

    EXPECT_EQ(flow, 20);
}


TEST(sample, eighth) {

    std::ifstream fin;
    std::string infile = path_to_tests_dir + "test_8.txt";
    fin.open(infile);

    if (!fin)
        exit(1);

    const long long flow = max_flow_mkm(fin);

    EXPECT_EQ(flow, 8);
}
