#pragma once

#include <cstddef>
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
    static constexpr std::size_t kMaxUsernameLength = 8;

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
     * Validates username and password according to rules:
     *   - Username must contain only letters (a–z, A–Z)
     *   - Username maximum length: 8 characters
     *   - Password must not be empty
     * The password is hashed before being stored.
     *
     * Parameters:
     *   username - unique identifier for the user
     *   password - plaintext password
     *
     * Returns:
     *   A SignupResult enumerator describing the outcome:
     *     - Success: user created and stored
     *     - EmptyPassword: password was empty
     *     - InvalidUsernameCharacters: username contains non-letter characters
     *     - UsernameTooLong: username exceeds maximum length of 8
     *     - UserAlreadyExists: username already registered
     */
    enum class SignupResult
    {
        Success,
        EmptyPassword,
        InvalidUsernameCharacters,
        UsernameTooLong,
        UserAlreadyExists
    };

    SignupResult signup(const std::string &username, const std::string &password);

    /*
     * Function: login
     * ---------------
     * Authenticates a user and establishes a session if successful.
     * Validates username format according to same rules as signup:
     *   - Username must contain only letters (a–z, A–Z)
     *   - Username maximum length: 8 characters
     *
     * Parameters:
     *   username - user's identifier
     *   password - plaintext password to verify
     *
     * Returns:
     *   A LoginResult enumerator describing the outcome:
     *     - Success: credentials valid and the user was pushed onto the session stack
     *     - EmptyInput: username or password was empty
     *     - InvalidUsernameCharacters: username contains non-letter characters
     *     - UsernameTooLong: username exceeds maximum length of 8
     *     - NoSuchUser: the username was not found in the user store
     *     - InvalidCredentials: password did not match the stored hash
     *     - AlreadyLoggedIn: the same user is already the active session (top of stack)
     */
    enum class LoginResult
    {
        Success,
        EmptyInput,
        InvalidUsernameCharacters,
        UsernameTooLong,
        NoSuchUser,
        InvalidCredentials,
        AlreadyLoggedIn
    };

    LoginResult login(const std::string &username, const std::string &password);

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

    /*
     * Function: getUserManager (internal use)
     * ----------------------------------------
     * Provides access to the UserManager for feed operations.
     * Internal use only.
     */
    const UserManager &getUserManager() const;

    /*
     * Function: viewNextNotification
     * --------------------------------
     * For the currently logged-in user, retrieves and removes the next notification
     * from their feed queue.
     *
     * Returns:
     *   The next notification string, or an error message if no user is logged in.
     */
    std::string viewNextNotification();

private:
    UserManager &userManager;
    std::stack<std::string> sessionStack;
};