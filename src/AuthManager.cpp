#include "AuthManager.h"
#include "User.h"

#include <functional>

AuthManager::AuthManager(UserManager &userManager)
    : userManager(userManager) {}

bool AuthManager::signup(const std::string &username, const std::string &password)
{
    if (userManager.userExists(username)) {
        return false;
    }
    return userManager.signup(username, hashPassword(password));
}

bool AuthManager::login(const std::string &username, const std::string &password) const
{
    const User *user = userManager.findUser(username);
    if (!user) {
        return false;
    }
    return user->passwordHash == hashPassword(password);
}

std::string AuthManager::hashPassword(const std::string &password)
{
    return std::to_string(std::hash<std::string>{}(password));
}