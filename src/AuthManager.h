#pragma once

#include <string>
#include <stack>
#include "UserManager.h"

class AuthManager
{
public:
    explicit AuthManager(UserManager &userManager);

    bool signup(const std::string &username, const std::string &password);
    bool login(const std::string &username, const std::string &password);
    static std::string hashPassword(const std::string &password);
    bool logout();
    std::string getCurrentUser() const;
    bool isLoggedIn() const;

private:
    UserManager &userManager;
    std::stack<std::string> sessionStack;
};
