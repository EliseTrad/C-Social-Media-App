#pragma once

#include <string>
#include <vector>

#include "UserManager.h"
/*
 * RelationshipManager
 * -------------------
 * Encapsulates follow/unfollow operations and queries on a directed
 * relationship graph. Each user stores two adjacency sets (followers
 * and following) in the `User` structure. RelationshipManager performs
 * validation (existence checks, self-follow prevention, duplicate
 * prevention) and keeps both sides of each relationship in sync.
 */
class RelationshipManager
{
public:
    /*
     * Function: RelationshipManager
     * -----------------------------
     * Construct a RelationshipManager with access to shared user storage.
     *
     * Parameters:
     *   userManager - reference to the UserManager that owns all user records.
     */
    explicit RelationshipManager(UserManager &userManager);

    /*
     * Function: follow
     * ----------------
     * Establish a directed follow relationship from follower -> followee.
     * Validation includes self-follow prevention, duplicate prevention,
     * and missing-user checks.
     *
     * Returns:
     *   true on success, false otherwise.
     */
    bool follow(const std::string &follower, const std::string &followee);

    /*
     * Function: unfollow
     * ------------------
     * Remove an existing directed follow relationship from follower -> followee.
     *
     * Returns:
     *   true when a relationship existed and was removed, false otherwise.
     */
    bool unfollow(const std::string &follower, const std::string &followee);

    /*
     * Function: isFollowing
     * ---------------------
     * Test whether follower currently follows followee.
     *
     * Returns:
     *   true if relationship exists, false otherwise.
     */
    bool isFollowing(const std::string &follower, const std::string &followee) const;

    /*
     * Function: getFollowers
     * ----------------------
     * Retrieve all usernames that follow the specified user.
     * Returns an empty vector if the user does not exist.
     */
    std::vector<std::string> getFollowers(const std::string &username) const;

    /*
     * Function: getFollowing
     * ----------------------
     * Retrieve all usernames followed by the specified user.
     * Returns an empty vector if the user does not exist.
     */
    std::vector<std::string> getFollowing(const std::string &username) const;

    /*
     * Function: getMutualConnections
     * ------------------------------
     * Compute the intersection of two users' following sets.
     * Returns an empty vector if either user does not exist.
     */
    std::vector<std::string> getMutualConnections(const std::string &userA, const std::string &userB) const;

private:
    UserManager &userManager;
};
