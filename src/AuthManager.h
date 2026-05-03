#pragma once

#include <string>

#include "UserManager.h"

class AuthManager
{
public:
    explicit AuthManager(UserManager &userManager);

    bool signup(const std::string &username, const std::string &password);
    bool login(const std::string &username, const std::string &password) const;
    static std::string hashPassword(const std::string &password);

private:
    UserManager &userManager;
};
