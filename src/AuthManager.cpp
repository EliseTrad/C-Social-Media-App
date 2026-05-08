#include "AuthManager.h"
#include "User.h"
#include <functional>
#include <sstream>

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

    if (username.empty() || password.empty())
    {
        return false;
    }
    if (!userManager.signup(username, hashPassword(password)))
    {
        return false;
    }
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

    if (username.empty() || password.empty())
    {
        return false;
    }

    const User *user = userManager.findUser(username);

    if (!user)
    {
        return false;
    }

    if (user->passwordHash != hashPassword(password))
    {
        return false;
    }

    if (!sessionStack.empty() && sessionStack.top() == username)
    {
        return false;
    }

    sessionStack.push(username);

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
    // Combine password with a constant salt for hashing
    std::hash<std::string> hasher;
    std::stringstream ss;
    // Combine password with a salt constant and convert to hex
    size_t hash_val = hasher(password + std::string("SOCIAL_MEDIA_APP_SALT"));
    ss << std::hex << hash_val;
    return ss.str();
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
    if (sessionStack.empty())
    {
        return false;
    }

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
    if (sessionStack.empty())
    {
        return "";
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

const UserManager &AuthManager::getUserManager() const
{
    return userManager;
}

std::string AuthManager::viewNextNotification()
{
    if (sessionStack.empty())
    {
        return "Error: no user logged in";
    }

    const std::string username = sessionStack.top();
    User *user = userManager.findUser(username);
    if (!user)
    {
        return "Error: user not found";
    }

    return user->viewNotification();
}