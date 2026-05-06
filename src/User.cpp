#include "User.h"

// constructor to create a new user
User::User(string u, string p)
{
    // store username
    username = u;

    // store password hash
    passwordHash = p;
}

// function to add a notification to the queue
void User::addNotification(const string& message)
{
    // add notification at the end of the queue
    notifications.push(message);
}

// function to view notifications
string User::viewNotification()
{
    // check if queue is empty
    if (notifications.empty())
    {
        return "No notifications";
    }

    // get the first notification in the queue
    string msg = notifications.front();

    // remove the notification after viewing it
    notifications.pop();

    // return the notification message
    return msg;
}