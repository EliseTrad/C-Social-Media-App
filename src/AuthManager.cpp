#include "AuthManager.h"
#include "User.h"
#include <iostream>
#include <functional>

/*
 * Function: AuthManager
 * ---------------------
 * Constructs the AuthManager and initializes it with a reference
 * to the UserManager. UserManager handles user storage and lookup.
 *
 * Parameters:
 *   userManager - reference to the system's user manager
 */
AuthManager::AuthManager(UserManager &userManager)
    : userManager(userManager) {}

/*
 * Function: signup
 * ----------------
 * Attempts to register a new user by delegating to UserManager.
 * The password is hashed before being stored.
 *
 * Parameters:
 *   username - unique identifier for the user
 *   password - plaintext password
 *
 * Returns:
 *   true if signup is successful, false if the user already exists
 */
bool AuthManager::signup(const std::string &username, const std::string &password)
{
    if (!userManager.signup(username, hashPassword(password))) {
        std::cout << "User Already Exists" << "\n";
        return false;
    }
    std::cout << "Signup Successful" << "\n";
    return true;
}

/*
 * Function: login
 * ---------------
 * Authenticates a user by verifying their credentials. If successful,
 * the username is pushed onto the session stack to establish an active session.
 *
 * Parameters:
 *   username - user's identifier
 *   password - plaintext password to verify
 *
 * Returns:
 *   true if login is successful, false otherwise
 */
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

/*
 * Function: hashPassword
 * ----------------------
 * Converts a plaintext password into a hashed representation using
 * the standard library hash function.
 *
 * Parameters:
 *   password - plaintext password
 *
 * Returns:
 *   hashed password as a string
 */
std::string AuthManager::hashPassword(const std::string &password)
{
    return std::to_string(std::hash<std::string>{}(password));
}

/*
 * Function: logout
 * ----------------
 * Terminates the current active session by removing the top user
 * from the session stack.
 *
 * Returns:
 *   true if logout is successful, false if no active session exists
 */
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

/*
 * Function: getCurrentUser
 * ------------------------
 * Retrieves the username of the currently active session from the stack.
 *
 * Returns:
 *   the username of the active user, or a message if no user is logged in
 */
std::string AuthManager::getCurrentUser() const
{
    if (sessionStack.empty()) {
        return "No Current User";
    }

    return sessionStack.top();
}

/*
 * Function: isLoggedIn
 * --------------------
 * Checks whether there is an active user session.
 *
 * Returns:
 *   true if at least one user is logged in, false otherwise
 */
bool AuthManager::isLoggedIn() const
{
    return !sessionStack.empty();
}