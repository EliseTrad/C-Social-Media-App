#include "AuthManager.h"
#include "User.h"
#include <cctype>
#include <functional>
#include <sstream>

namespace
{
    bool hasLettersOnly(const std::string &value)
    {
        if (value.empty())
        {
            return false;
        }

        for (const char ch : value)
        {
            if (!std::isalpha(static_cast<unsigned char>(ch)))
            {
                return false;
            }
        }

        return true;
    }
} // namespace

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
 * Validates username format (letters only, max 8 chars) and password non-emptiness.
 * The password is hashed before being stored.
 *
 * Parameters:
 *   username - unique identifier for the user
 *   password - plaintext password
 *
 * Returns:
 *   An AuthManager::SignupResult value indicating the specific outcome:
 *     - Success: user created and stored
 *     - EmptyPassword: password validation failed
 *     - InvalidUsernameCharacters: username contains non-letter characters
 *     - UsernameTooLong: username exceeds 8 character limit
 *     - UserAlreadyExists: user already registered with that username
 */
AuthManager::SignupResult AuthManager::signup(const std::string &username, const std::string &password)
{

    if (password.empty())
    {
        return SignupResult::EmptyPassword;
    }

    if (!hasLettersOnly(username))
    {
        return SignupResult::InvalidUsernameCharacters;
    }

    if (username.size() > kMaxUsernameLength)
    {
        return SignupResult::UsernameTooLong;
    }

    if (!userManager.signup(username, hashPassword(password)))
    {
        return SignupResult::UserAlreadyExists;
    }
    return SignupResult::Success;
}

/*
 * Function: login
 * ---------------
 * Authenticates a user by verifying their credentials and validating username format.
 * If successful, the username is pushed onto the session stack to establish an active session.
 * Username validation matches signup rules: letters only, max 8 characters.
 *
 * Parameters:
 *   username - user's identifier
 *   password - plaintext password to verify
 *
 * Returns:
 *   An AuthManager::LoginResult value describing the specific outcome:
 *     - Success: credentials valid and session established
 *     - EmptyInput: username or password empty
 *     - InvalidUsernameCharacters: username contains non-letter characters
 *     - UsernameTooLong: username exceeds 8 character limit
 *     - NoSuchUser: username not found
 *     - InvalidCredentials: password hash mismatch
 *     - AlreadyLoggedIn: same user already active in session
 */
AuthManager::LoginResult AuthManager::login(const std::string &username, const std::string &password)
{

    if (username.empty() || password.empty())
    {
        return LoginResult::EmptyInput;
    }

    if (!hasLettersOnly(username))
    {
        return LoginResult::InvalidUsernameCharacters;
    }

    if (username.size() > kMaxUsernameLength)
    {
        return LoginResult::UsernameTooLong;
    }

    const User *user = userManager.findUser(username);

    if (!user)
    {
        return LoginResult::NoSuchUser;
    }

    if (user->passwordHash != hashPassword(password))
    {
        return LoginResult::InvalidCredentials;
    }

    if (!sessionStack.empty() && sessionStack.top() == username)
    {
        return LoginResult::AlreadyLoggedIn;
    }

    sessionStack.push(username);

    return LoginResult::Success;
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
 *   the username of the active user, or an empty string if no user is logged in
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