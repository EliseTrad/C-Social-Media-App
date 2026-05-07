#pragma once

#include <string>
#include <unordered_map>
#include <vector>

#include "User.h"

/*
 * Class: UserManager
 * ------------------
 * Owns the central in-memory registry of all users.
 *
 * The registry is implemented with an unordered_map so username lookup,
 * signup checks, and relationship access remain O(1) on average.
 */
class UserManager
{
public:
    /*
     * Function: signup
     * -----------------
     * Insert a new user into the registry if the username is not already
     * taken.
     *
     * Returns:
     *   true if the user was inserted, false if the username already exists.
     */
    bool signup(const std::string &username, const std::string &passwordHash);

    /*
     * Function: findUser
     * -------------------
     * Look up a user by username and return a mutable pointer when found.
     * Returns nullptr when the user does not exist.
     */
    User *findUser(const std::string &username);

    /*
     * Function: findUser
     * -------------------
     * Const overload of findUser for read-only access.
     */
    const User *findUser(const std::string &username) const;

    /*
     * Function: userExists
     * ---------------------
     * Test whether a username is already registered.
     */
    bool userExists(const std::string &username) const;

    /*
     * Function: getAllUsernames
     * --------------------------
     * Return every username currently stored in the registry.
     */
    std::vector<std::string> getAllUsernames() const;

private:
    // Primary username-to-user lookup table.
    std::unordered_map<std::string, User> users;
};
