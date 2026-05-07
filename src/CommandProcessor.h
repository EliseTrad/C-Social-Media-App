#pragma once

#include <functional>
#include <string>
#include <stack>
#include <unordered_map>
#include <vector>

#include "AuthManager.h"
#include "RelationshipManager.h"

/*
 * ParsedCommand
 * -------------
 * Stores the result of splitting a raw CLI line into a command token and its
 * remaining argument tokens.
 */
struct ParsedCommand
{
    std::string command;
    std::vector<std::string> args;
};

/*
 * Module: CommandProcessor
 * ------------------------
 * Handles command parsing, dispatching, and session management for the CLI.
 *
 * Key Design:
 * - Uses std::unordered_map for command dispatch (O(1) average lookup).
 * - Uses std::stack for session handling (O(1) push/pop/top).
 * - Uses std::stringstream for command parsing.
 *
 * The CommandProcessor is the CLI execution layer and in a full integration
 * would route commands to UserManager and RelationshipManager implementations.
 */
class CommandProcessor
{
public:
    /*
     * Function: CommandProcessor
     * --------------------------
     * Construct a CommandProcessor instance with manager references and
     * populate the dispatch map.
     *
     * Parameters:
     *   authManager - reference to AuthManager (for signup/login).
     *   relationshipManager - reference to RelationshipManager (for follow/unfollow).
     */
    CommandProcessor(AuthManager &authManager, RelationshipManager &relationshipManager);

    /*
     * Function: parseCommand
     * ----------------------
     * Split a raw CLI line into a command token and a list of arguments.
     *
     * Parameters:
     *   commandLine - the full raw line entered by the user.
     *
     * Returns:
     *   ParsedCommand with 'command' set to the first token and 'args'
     *   containing the remaining tokens.
     */
    ParsedCommand parseCommand(const std::string &commandLine) const;

    /*
     * Function: processCommand
     * ------------------------
     * Parse and execute a single command line by dispatching to the
     * corresponding handler in the dispatch map.
     *
     * Parameters:
     *   commandLine - the full raw line entered by the user.
     *
     * Returns:
     *   true when the input was handled or ignored (empty input), false
     *   when the command is unknown.
     */
    bool processCommand(const std::string &commandLine);

private:
    // Manager references for command execution.
    AuthManager &authManager;
    RelationshipManager &relationshipManager;

    // Dispatch map used to avoid if-else chains and keep command lookup fast.
    std::unordered_map<std::string, std::function<void(const std::vector<std::string> &)>> handlers;

    // Prints command arguments exactly as entered.
    void handleEcho(const std::vector<std::string> &args) const;

    // Prints the list of supported commands for discoverability.
    void handleHelp(const std::vector<std::string> &args) const;

    // Prints a shutdown message for the CLI.
    void handleExit(const std::vector<std::string> &args) const;

    // Calls AuthManager to create a new account and returns success status.
    void handleSignup(const std::vector<std::string> &args);

    // Calls AuthManager to validate credentials and pushes user to session stack.
    void handleLogin(const std::vector<std::string> &args);

    // Pops the current user from the session stack to end the active session.
    void handleLogout(const std::vector<std::string> &args);

    // Calls RelationshipManager to establish a follow relationship with another user.
    void handleFollow(const std::vector<std::string> &args);

    // Calls RelationshipManager to remove a follow relationship with another user.
    void handleUnfollow(const std::vector<std::string> &args);

    // Calls RelationshipManager to retrieve a user's followers.
    void handleFollowers(const std::vector<std::string> &args);

    // Calls RelationshipManager to retrieve a user's following list.
    void handleFollowing(const std::vector<std::string> &args);

    // Calls RelationshipManager to retrieve mutual connections (bidirectional followers).
    void handleMutuals(const std::vector<std::string> &args);

    // Calls User's feed queue to retrieve and display the next notification.
    void handleFeedNext(const std::vector<std::string> &args);

    // Returns the current session user from AuthManager.
    // Returns:
    // - The active user name, or an empty string when no session exists.
    std::string currentUser() const;
};
