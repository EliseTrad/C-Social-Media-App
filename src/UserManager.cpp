#include "UserManager.h"

/*
 * UserManager implementation
 * --------------------------
 * Provides the in-memory account registry used by authentication and
 * relationship management.
 */

/*
 * Function: signup
 * -----------------
 * Create a new user record when the username is not already present.
 * This is the only place where user objects are inserted into the registry.
 */
bool UserManager::signup(const std::string &username, const std::string &passwordHash)
{
    // Reject duplicate usernames.
    if (users.find(username) != users.end())
    {
        return false;
    }

    // Construct the user record before inserting it into the registry.
    User newUser(username, passwordHash);

    // Store the new user by username for O(1) average lookup.
    users[username] = newUser;

    return true;
}

/*
 * Function: findUser
 * ------------------
 * Return a mutable pointer to a user record if the username exists.
 */
User *UserManager::findUser(const std::string &username)
{
    // Return nullptr when the registry does not contain the requested user.
    if (users.find(username) == users.end())
    {
        return nullptr;
    }

    // Pointer remains valid as long as the user stays in the unordered_map.
    return &users[username];
}

/*
 * Function: findUser
 * ------------------
 * Const overload used by read-only call sites.
 */
const User *UserManager::findUser(const std::string &username) const
{
    // Search the registry without exposing mutable access.
    auto it = users.find(username);

    // Return nullptr when the user is absent.
    if (it == users.end())
    {
        return nullptr;
    }

    // Return a pointer to the stored user object.
    return &(it->second);
}

/*
 * Function: userExists
 * --------------------
 * Check whether the registry already contains the given username.
 */
bool UserManager::userExists(const std::string &username) const
{
    // Existence is determined by a single hash lookup.
    if (users.find(username) != users.end())
    {
        return true;
    }

    return false;
}

/*
 * Function: getAllUsernames
 * -------------------------
 * Collect every registered username into a vector.
 */
std::vector<std::string> UserManager::getAllUsernames() const
{
    // Preallocate result storage only logically; the size is not known here.
    std::vector<std::string> names;

    // Iterate over every user and collect the map keys.
    for (auto user : users)
    {
        names.push_back(user.first);
    }

    return names;
}