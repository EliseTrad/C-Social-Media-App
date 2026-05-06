#pragma once

#include <string>
#include <stack>
#include "UserManager.h"

/*
 * Class: AuthManager
 * ------------------
 * Responsible for handling user authentication and session management.
 * Provides functionality for user signup, login, logout, tracking
 * of the currently active session.
 *
 * Uses:
 *   - UserManager for user storage and lookup (hash map)
 *   - std::stack for session management (LIFO behavior)
 */
class AuthManager
{
public:
    /*
     * Function: AuthManager
     * ---------------------
     * Constructs the AuthManager with a reference to the UserManager.
     *
     * Parameters:
     *   userManager - reference to the system's user manager
     */
    explicit AuthManager(UserManager &userManager);

    /*
     * Function: signup
     * ----------------
     * Registers a new user with the given username and password.
     * The password is hashed before being stored.
     *
     * Parameters:
     *   username - unique identifier for the user
     *   password - plaintext password
     *
     * Returns:
     *   true if signup succeeds, false if the user already exists
     *   or input is invalid
     */
    bool signup(const std::string &username, const std::string &password);

    /*
     * Function: login
     * ---------------
     * Authenticates a user using their credentials and establishes
     * a session if successful.
     *
     * Parameters:
     *   username - user's identifier
     *   password - plaintext password to verify
     *
     * Returns:
     *   true if login is successful, false otherwise
     */
    bool login(const std::string &username, const std::string &password);

    /*
     * Function: hashPassword
     * ----------------------
     * Converts a plaintext password into a hashed representation.
     *
     * Parameters:
     *   password - plaintext password
     *
     * Returns:
     *   hashed password as a string
     */
    static std::string hashPassword(const std::string &password);

    /*
     * Function: logout
     * ----------------
     * Terminates the current active session by removing the top
     * user from the session stack.
     *
     * Returns:
     *   true if logout succeeds, false if no active session exists
     */
    bool logout();

    /*
     * Function: getCurrentUser
     * ------------------------
     * Retrieves the username of the currently active session.
     *
     * Returns:
     *   username of the active user, or an empty string if no user
     *   is logged in
     */
    std::string getCurrentUser() const;

    /*
     * Function: isLoggedIn
     * --------------------
     * Checks whether there is an active user session.
     *
     * Returns:
     *   true if at least one user is logged in, false otherwise
     */
    bool isLoggedIn() const;

private:
    UserManager &userManager;
    std::stack<std::string> sessionStack;
};