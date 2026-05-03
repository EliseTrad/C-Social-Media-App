#pragma once

#include <string>

#include "AuthManager.h"
#include "RelationshipManager.h"
#include "UserManager.h"

class CommandProcessor
{
public:
    CommandProcessor(UserManager &userManager, AuthManager &authManager, RelationshipManager &relationshipManager);

    bool processCommand(const std::string &commandLine);
    void printHelp() const;

private:
    UserManager &userManager;
    AuthManager &authManager;
    RelationshipManager &relationshipManager;
};
