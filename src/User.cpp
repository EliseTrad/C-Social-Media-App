
#include "User.h"

/*
 * Function: User
 * --------------
 * Initialize a user record with the supplied username and password hash.
 */
User::User(std::string u, std::string p)
{
    username = u;

    passwordHash = p;
}

/*
 * Function: addNotification
 * -------------------------
 * Append a notification to the end of the FIFO queue.
 */
void User::addNotification(const std::string &message)
{
    notifications.push(message);
}

/*
 * Function: viewNotification
 * --------------------------
 * Remove and return the next queued notification.
 */
std::string User::viewNotification()
{
    if (notifications.empty())
    {
        return "No notifications";
    }

    std::string msg = notifications.front();

    notifications.pop();

    return msg;
}