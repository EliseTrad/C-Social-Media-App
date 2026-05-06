#include "AuthManager.h"
#include "User.h"
#include <iostream>
#include <functional>

AuthManager::AuthManager(UserManager &userManager)
    : userManager(userManager) {}

bool AuthManager::signup(const std::string &username, const std::string &password)
{
    if (!userManager.signup(username, hashPassword(password))) {
        std::cout << "User Already Exists" << "\n";
        return false;
    }
    std::cout << "Signup Successful" << "\n";
    return true;
}


bool AuthManager::login(const std::string &username, const std::string &password)
{
    const User* user = userManager.findUser(username);

    if (!user) {
        std::cout << "User Does Not Exist" << "\n";
        return false;
    }

    if (user->passwordHash != hashPassword(password)) {
        std::cout << "Incorrect Password" << "\n";
        return false;
    }

    if (!sessionStack.empty() && sessionStack.top() == username) {
        std::cout << "User Already Logged In" << "\n";
        return false;
    }

    sessionStack.push(username);
    std::cout << "Login Successful " << username << "\n";

    return true;
}

std::string AuthManager::hashPassword(const std::string &password)
{
    return std::to_string(std::hash<std::string>{}(password));
}

bool AuthManager::logout()
{
    if (sessionStack.empty()) {
        std::cout << "No Active Session" << "\n";
        return false;
    }

    std::cout << "Logout Successful " << sessionStack.top() << "\n";
    sessionStack.pop();

    return true;
}

std::string AuthManager::getCurrentUser() const
{
    if (sessionStack.empty()) {
        return "No Current User";
    }

    return sessionStack.top();
}

bool AuthManager::isLoggedIn() const
{
    return !sessionStack.empty();
}