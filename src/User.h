// User.h
#pragma once
#include <string>
#include <unordered_set>
using namespace std;

class User
{
public:
    string username;
    string passwordHash;
    unordered_set<string> followers;
    unordered_set<string> following;

    User(string u, string p);
};