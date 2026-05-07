#pragma once
#include <string>
#include <unordered_set>
#include <queue>

using namespace std;

/*
 * Class: User
 * -----------
 * Represents a single social-media account in memory.
 *
 * Each user stores:
 * - public identity data (username and password hash)
 * - incoming relationships in `followers`
 * - outgoing relationships in `following`
 * - queued notifications in FIFO order
 *
 * The relationship sets form an adjacency-list style representation of the
 * directed follow graph.
 */
class User
{
public:
    // Unique account name used as the lookup key throughout the system.
    string username;

    // Hashed password string stored after signup and checked on login.
    string passwordHash;

    // Users who follow this account.
    unordered_set<string> followers;

    // Users this account follows.
    unordered_set<string> following;

    // FIFO queue of notifications for this account.
    queue<string> notifications;

    /*
     * Function: User
     * --------------
     * Construct an in-memory user record with the provided username and
     * password hash.
     */
    User() : username(""), passwordHash("") {}
    User(string u, string p);

    /*
     * Function: addNotification
     * --------------------------
     * Append a new notification to the end of the user's notification queue.
     */
    void addNotification(const string &message);

    /*
     * Function: viewNotification
     * ---------------------------
     * Return and remove the next notification in FIFO order.
     *
     * Returns:
     *   The next notification string, or "No notifications" if the queue
     *   is empty.
     */
    string viewNotification();
};