#include "User.h"

/*
 * Function: User
 * --------------
 * Initialize a user record with the supplied username and password hash.
 */
User::User(string u, string p)
{
    username = u;

    passwordHash = p;
}

/*
 * Function: addNotification
 * -------------------------
 * Append a notification to the end of the FIFO queue.
 */
void User::addNotification(const string &message)
{
    notifications.push(message);
}

/*
 * Function: viewNotification
 * --------------------------
 * Remove and return the next queued notification.
 */
string User::viewNotification()
{
    if (notifications.empty())
    {
        return "No notifications";
    }

    string msg = notifications.front();

    notifications.pop();

    return msg;
}