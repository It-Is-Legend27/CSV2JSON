/**
 * @file csv2json.hpp
 * @author Angel Badillo Hernandez (https://github.com/It-Is-Legend27)
 * @brief Simple CSV file to JSON file conversions, and vice versa.
 * @version 0.1
 * @date 2022-03-20
 *
 * @copyright Copyright (c) 2022
 *
 */
#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include "json.hpp"
#include "rapidcsv.h"
using namespace std;
using nlohmann::json;
using rapidcsv::Document;

enum JSON_INDENT
{
    MIN = 0,
    DEF = 4
};

bool is_int(const string &s);

bool is_float(const string &s);

void csv2json(string csvName, string jsonName, int indent);

void json2csv(string csvName, string jsonName);

bool is_int(const string &s)
{
    bool is_int = false;
    for (string::const_iterator it = s.cbegin(); it != s.end(); it++)
    {
        if (*it >= '0' && *it <= '9')
            is_int = true;
        else
        {
            is_int = false;
            it = s.end() - 1;
        }
    }
    return is_int;
}

bool is_float(const string &s)
{
    bool is_f = false;
    short pointCount = 0;

    for (string::const_iterator it = s.cbegin(); it != s.end(); it++)
    {
        if (*it >= '0' && *it <= '9')
            is_f = true;
        else if (*it == '.' && pointCount == 0)
            pointCount++;
        else
        {
            is_f = false;
            it = s.end() - 1;
        }
    }
    return is_f;
}

void csv2json(string csvName, string jsonName, int indent = DEF)
{
    Document doc(csvName);
    vector<string> keys = doc.GetColumnNames();
    ofstream outJson;
    outJson.open(jsonName);
    json obj = json::object();
    json A = json::array();

    for (size_t r = 0; r < doc.GetRowCount(); r++)
    {
        for (auto &&c : keys)
        {
            string temp = doc.GetCell<string>(c, r);
            if (is_int(temp))
                obj[c] = stoll(temp);
            else if (is_float(temp))
                obj[c] = stold(temp);
            else
                obj[c] = temp;
        }
        A += obj;
    }

    outJson << A.dump(indent);
    outJson.close();
}

void json2csv(string csvName, string jsonName)
{
    ifstream inJson;
    ofstream outCSV;
    outCSV.open(csvName);
    inJson.open(jsonName);
    json A = json::parse(inJson);
    vector<string> keys;
    inJson.close();

    Document doc;

    if (!A.empty())
    {
        int i = 0;
        for (json::iterator it = A[0].begin(); it != A[0].end(); ++it)
        {
            keys.push_back(it.key());
            doc.SetColumnName(i++, it.key());
        }

        for (size_t i = 0; i < A.size(); i++)
        {
            for (size_t j = 0; j < keys.size(); j++)
            {
                json temp = A[i][keys[j]];
                if (temp.is_number_integer())
                    doc.SetCell(j, i, (long long)temp);
                else if (temp.is_number_float())
                    doc.SetCell(j, i, (long double)temp);
                else
                    doc.SetCell(j, i, (string)temp);
            }
        }
        doc.Save(outCSV);
    }
    outCSV.close();
}