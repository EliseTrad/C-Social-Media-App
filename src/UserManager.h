#pragma once

#include <string>
#include <unordered_map>
#include <vector>

#include "User.h"

class UserManager
{
public:
    bool signup(const std::string &username, const std::string &passwordHash);
    User *findUser(const std::string &username);
    const User *findUser(const std::string &username) const;
    bool userExists(const std::string &username) const;
    std::vector<std::string> getAllUsernames() const;

private:
    std::unordered_map<std::string, User> users;
};
