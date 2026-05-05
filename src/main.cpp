#include <iostream>
#include <string>

#include "AuthManager.h"
#include "CommandProcessor.h"
#include "RelationshipManager.h"
#include "UserManager.h"

/* =========================
   CLI MAIN LOOP
   ========================= */
/*
 * The program implements a simple CLI main loop with the following flow:
 *   1) Read a line from standard input
 *   2) Parse the line into a command and arguments
 *   3) Execute the command via CommandProcessor
 *
 * Typing 'exit' breaks the loop and the program terminates.
 */
int main()
{
    /* Instantiate the manager layer. */
    UserManager userManager;
    AuthManager authManager(userManager);
    RelationshipManager relationshipManager(userManager);

    /* Owns the CLI command execution layer and passes manager references. */
    CommandProcessor commandProcessor(authManager, relationshipManager);

    /* Reused buffer for each user-entered command line. */
    std::string commandLine;

    /* Read -> parse -> execute */
    while (std::getline(std::cin, commandLine))
    {
        if (commandLine == "exit")
        {
            break;
        }

        commandProcessor.processCommand(commandLine);
    }

    return 0;
}
