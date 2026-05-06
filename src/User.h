// User.h
#pragma once
#include <string>
#include <unordered_set>
#include <queue>

using namespace std;

class User
{
public:
    string username;
    string passwordHash;

    unordered_set<string> followers;
    unordered_set<string> following;

    queue<string> notifications;

    User(string u, string p);

    void addNotification(const string& message);
    string viewNotification();
};