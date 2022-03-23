#include "csv2json.hpp"
#include "json.hpp"
#include <fstream>
#include <iostream>

using namespace std;
using nlohmann::json;
using rapidcsv::Document;

int main(int argc, char **argv)
{
    bool validArgCount = false;
    switch (argc)
    {
    case 4:
        validArgCount = true;
        break;

    default:
        validArgCount = false;
        break;
    }

    if(!validArgCount)
    return 1;

    if (string(argv[1]) == "csv2json")
    {
        csv2json(argv[2], argv[3]);
    }
    else if (string(argv[1]) == "json2csv")
    {
        json2csv(argv[2], argv[3]);
    }
    else
    return 1;

    return 0;
}
