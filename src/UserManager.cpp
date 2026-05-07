#include "UserManager.h"

// function to create a new user account
bool UserManager::signup(const std::string& username, const std::string& passwordHash)
{
    // check if the username already exists
    if (users.find(username) != users.end())
    {
        return false;
    }

    // create a new user object
    User newUser(username, passwordHash);

    // add the user to the hashmap
    users[username] = newUser;

    return true;
}

// function to find a user using username
User* UserManager::findUser(const std::string& username)
{
    // if user does not exist return nullptr
    if (users.find(username) == users.end())
    {
        return nullptr;
    }

    // return pointer to the user object
    return &users[username];
}

// const version of findUser
const User* UserManager::findUser(const std::string& username) const
{
    // search for the user
    auto it = users.find(username);

    // if user not found
    if (it == users.end())
    {
        return nullptr;
    }

    // return pointer to found user
    return &(it->second);
}

// check if a username already exists
bool UserManager::userExists(const std::string& username) const
{
    // if username found return true
    if (users.find(username) != users.end())
    {
        return true;
    }

    // otherwise return false
    return false;
}

// function to return all usernames
std::vector<std::string> UserManager::getAllUsernames() const
{
    // vector to store usernames
    std::vector<std::string> names;

    // loop through all users in hashmap
    for (auto user : users)
    {
        // add username to vector
        names.push_back(user.first);
    }

    // return vector of usernames
    return names;
}