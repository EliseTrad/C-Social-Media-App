#include "CommandProcessor.h"

#include <iostream>
#include <sstream>

/* =========================
    COMMAND PROCESSOR SETUP
    ========================= */

/*
 * Function: CommandProcessor
 * --------------------------
 * Construct and initialize the command dispatch table with manager references.
 * Uses an unordered_map to map command names to handler functions
 * for O(1) average lookup instead of long if-else chains.
 */
CommandProcessor::CommandProcessor(AuthManager &authManager, RelationshipManager &relationshipManager)
    : authManager(authManager), relationshipManager(relationshipManager)
{
    handlers["echo"] = [this](const std::vector<std::string> &args)
    { handleEcho(args); };
    handlers["help"] = [this](const std::vector<std::string> &args)
    { handleHelp(args); };
    handlers["exit"] = [this](const std::vector<std::string> &args)
    { handleExit(args); };
    handlers["signup"] = [this](const std::vector<std::string> &args)
    { handleSignup(args); };
    handlers["login"] = [this](const std::vector<std::string> &args)
    { handleLogin(args); };
    handlers["logout"] = [this](const std::vector<std::string> &args)
    { handleLogout(args); };
    handlers["follow"] = [this](const std::vector<std::string> &args)
    { handleFollow(args); };
    handlers["unfollow"] = [this](const std::vector<std::string> &args)
    { handleUnfollow(args); };
    handlers["followers"] = [this](const std::vector<std::string> &args)
    { handleFollowers(args); };
    handlers["following"] = [this](const std::vector<std::string> &args)
    { handleFollowing(args); };
    handlers["mutuals"] = [this](const std::vector<std::string> &args)
    { handleMutuals(args); };
    handlers["feed_next"] = [this](const std::vector<std::string> &args)
    { handleFeedNext(args); };
}

/* =========================
    BUILT-IN COMMAND HANDLERS
    ========================= */

/*
 * Function: handleEcho
 * --------------------
 * Echoes the provided arguments to standard output, separated by spaces.
 *
 * Parameters:
 *   args - tokens following the 'echo' command.
 */
void CommandProcessor::handleEcho(const std::vector<std::string> &args) const
{
    for (std::size_t index = 0; index < args.size(); ++index)
    {
        if (index > 0)
        {
            std::cout << " ";
        }

        std::cout << args[index];
    }

    std::cout << '\n';
}

/*
 * Function: handleHelp
 * --------------------
 * Prints a short list of available commands for the CLI.
 *
 * Parameters:
 *   args - unused for this command.
 */
void CommandProcessor::handleHelp(const std::vector<std::string> &args) const
{
    (void)args; // Unused parameter
    std::cout << "Available commands: echo, help, exit, signup, login, logout, follow, unfollow, followers, following, mutuals, feed_next\n";
}

/*
 * Function: handleExit
 * --------------------
 * Prints a shutdown message. Actual program termination is handled by main
 * which exits the read loop when 'exit' is typed.
 *
 * Parameters:
 *   args - unused for this command.
 */
void CommandProcessor::handleExit(const std::vector<std::string> &args) const
{
    (void)args; // Unused parameter
    std::cout << "Exiting...\n";
}

/*
 * Function: handleSignup
 * ----------------------
 * Calls AuthManager to create a new account with the provided credentials.
 *
 * Parameters:
 *   args - vector expected to contain [username, password].
 */
void CommandProcessor::handleSignup(const std::vector<std::string> &args)
{
    if (args.size() < 2)
    {
        std::cout << "Usage: signup <username> <password>\n";
        return;
    }

    const bool success = authManager.signup(args[0], args[1]);
    if (success)
    {
        std::cout << "Signup successful for " << args[0] << "\n";
    }
    else
    {
        std::cout << "Signup failed: user may already exist\n";
    }
}

/*
 * Function: currentUser
 * ---------------------
 * Retrieve the currently active session user from the top of the stack.
 *
 * Returns:
 *   The active username as a std::string, or an empty string if no user
 *   is logged in.
 */
std::string CommandProcessor::currentUser() const
{
    return authManager.getCurrentUser();
}

/*
 * Function: handleLogin
 * ---------------------
 * Calls AuthManager to validate credentials. If successful, pushes the username
 * onto the session stack to model an active login.
 *
 * Parameters:
 *   args - vector expected to contain [username, password].
 */
void CommandProcessor::handleLogin(const std::vector<std::string> &args)
{
    if (args.size() < 2)
    {
        std::cout << "Usage: login <username> <password>\n";
        return;
    }

    const bool success = authManager.login(args[0], args[1]);
    if (success)
    {
        std::cout << "Login successful for " << args[0] << "\n";
    }
    else
    {
        std::cout << "Login failed: invalid credentials\n";
    }
}

/*
 * Function: handleLogout
 * ----------------------
 * Pops the current user from the session stack. If the stack is empty a
 * message is printed and no action is taken.
 *
 * Parameters:
 *   args - unused for this command.
 */
void CommandProcessor::handleLogout(const std::vector<std::string> &args)
{
    (void)args; // Unused parameter

    const std::string loggedInUser = currentUser();
    const bool success = authManager.logout();
    if (success)
    {
        std::cout << "Success: logout successful for " << loggedInUser << "\n";
    }
    else
    {
        std::cout << "Error: no active session" << "\n";
    }
}

/*
 * Function: handleFollow
 * ----------------------
 * Calls RelationshipManager to establish a follow relationship with another user.
 * The current session user is the follower.
 *
 * Parameters:
 *   args - vector expected to contain [followee_username].
 */
void CommandProcessor::handleFollow(const std::vector<std::string> &args)
{
    if (args.size() < 1)
    {
        std::cout << "Usage: follow <username>\n";
        return;
    }

    if (!authManager.isLoggedIn())
    {
        std::cout << "Error: no user logged in\n";
        return;
    }

    const std::string follower = currentUser();
    const std::string followee = args[0];

    const bool success = relationshipManager.follow(follower, followee);
    if (success)
    {
        std::cout << follower << " is now following " << followee << "\n";
    }
    else
    {
        std::cout << "Follow failed: users may not exist or already following\n";
    }
}

/*
 * Function: handleUnfollow
 * ------------------------
 * Calls RelationshipManager to remove a follow relationship with another user.
 * The current session user is the follower.
 *
 * Parameters:
 *   args - vector expected to contain [followee_username].
 */
void CommandProcessor::handleUnfollow(const std::vector<std::string> &args)
{
    if (args.size() < 1)
    {
        std::cout << "Usage: unfollow <username>\n";
        return;
    }

    if (!authManager.isLoggedIn())
    {
        std::cout << "Error: no user logged in\n";
        return;
    }

    const std::string follower = currentUser();
    const std::string followee = args[0];

    const bool success = relationshipManager.unfollow(follower, followee);
    if (success)
    {
        std::cout << follower << " is no longer following " << followee << "\n";
    }
    else
    {
        std::cout << "Unfollow failed: users may not exist or not following\n";
    }
}

/*
 * Function: handleFollowers
 * -------------------------
 * Retrieves and displays the list of followers for a user.
 * If no username is provided, uses the current logged-in user.
 *
 * Parameters:
 *   args - vector optionally containing [username]. If empty, uses current user.
 */
void CommandProcessor::handleFollowers(const std::vector<std::string> &args)
{
    std::string targetUser;

    if (args.empty())
    {
        targetUser = currentUser();
        if (targetUser.empty())
        {
            std::cout << "Error: no user logged in and no username provided\n";
            return;
        }
    }
    else
    {
        targetUser = args[0];
    }

    std::vector<std::string> followers = relationshipManager.getFollowers(targetUser);
    if (followers.empty())
    {
        std::cout << targetUser << " has no followers\n";
    }
    else
    {
        std::cout << targetUser << " followers: ";
        for (size_t i = 0; i < followers.size(); ++i)
        {
            if (i > 0)
                std::cout << ", ";
            std::cout << followers[i];
        }
        std::cout << "\n";
    }
}

/*
 * Function: handleFollowing
 * -------------------------
 * Retrieves and displays the list of users that a given user follows.
 * If no username is provided, uses the current logged-in user.
 *
 * Parameters:
 *   args - vector optionally containing [username]. If empty, uses current user.
 */
void CommandProcessor::handleFollowing(const std::vector<std::string> &args)
{
    std::string targetUser;

    if (args.empty())
    {
        targetUser = currentUser();
        if (targetUser.empty())
        {
            std::cout << "Error: no user logged in and no username provided\n";
            return;
        }
    }
    else
    {
        targetUser = args[0];
    }

    std::vector<std::string> following = relationshipManager.getFollowing(targetUser);
    if (following.empty())
    {
        std::cout << targetUser << " is not following anyone\n";
    }
    else
    {
        std::cout << targetUser << " following: ";
        for (size_t i = 0; i < following.size(); ++i)
        {
            if (i > 0)
                std::cout << ", ";
            std::cout << following[i];
        }
        std::cout << "\n";
    }
}

/*
 * Function: handleMutuals
 * ----------------------
 * Retrieves and displays mutual connections (bidirectional followers) for a user.
 * A mutual connection is a user who both follows and is followed by the target user.
 * If no username is provided, uses the current logged-in user.
 *
 * Parameters:
 *   args - vector optionally containing [username]. If empty, uses current user.
 */
void CommandProcessor::handleMutuals(const std::vector<std::string> &args)
{
    std::string targetUser;

    if (args.empty())
    {
        targetUser = currentUser();
        if (targetUser.empty())
        {
            std::cout << "Error: no user logged in and no username provided\n";
            return;
        }
    }
    else
    {
        targetUser = args[0];
    }

    std::vector<std::string> mutuals = relationshipManager.getMutualConnections(targetUser);
    if (mutuals.empty())
    {
        std::cout << targetUser << " has no mutual connections\n";
    }
    else
    {
        std::cout << targetUser << " mutuals: ";
        for (size_t i = 0; i < mutuals.size(); ++i)
        {
            if (i > 0)
                std::cout << ", ";
            std::cout << mutuals[i];
        }
        std::cout << "\n";
    }
}

/*
 * Function: handleFeedNext
 * ------------------------
 * Retrieves and displays the next notification from the current user's feed queue.
 * Requires an active login session.
 *
 * Parameters:
 *   args - unused for this command.
 */
void CommandProcessor::handleFeedNext(const std::vector<std::string> &args)
{
    (void)args; // Unused parameter

    std::string notification = authManager.viewNextNotification();
    std::cout << notification << "\n";
}

/* =========================
    PARSING AND DISPATCH
    ========================= */

/*
 * Function: parseCommand
 * ----------------------
 * Tokenizes a raw command line into a leading command and a list of args.
 *
 * Parameters:
 *   commandLine - raw input line.
 *
 * Returns:
 *   ParsedCommand with 'command' and 'args' populated.
 */
ParsedCommand CommandProcessor::parseCommand(const std::string &commandLine) const
{
    ParsedCommand parsedCommand;
    std::stringstream stream(commandLine);

    stream >> parsedCommand.command;

    std::string argument;
    while (stream >> argument)
    {
        parsedCommand.args.push_back(argument);
    }

    return parsedCommand;
}

/*
 * Function: processCommand
 * ------------------------
 * Parse the provided line and dispatch to the registered command handler.
 * Empty command lines are ignored. Unknown commands return false.
 *
 * Parameters:
 *   commandLine - raw input line.
 *
 * Returns:
 *   true if the command was handled or ignored; false if the command is
 *   unknown.
 */
bool CommandProcessor::processCommand(const std::string &commandLine)
{
    const ParsedCommand parsedCommand = parseCommand(commandLine);

    if (parsedCommand.command.empty())
    {
        return true;
    }

    const auto handlerIt = handlers.find(parsedCommand.command);
    if (handlerIt != handlers.end())
    {
        handlerIt->second(parsedCommand.args);
        return true;
    }

    std::cout << "Unknown command: " << parsedCommand.command << "\n";
    return false;
}
