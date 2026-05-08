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
    enum class FollowResult
    {
        Success,
        SelfOperation,
        UserNotFound,
        AlreadyFollowing
    };

    enum class UnfollowResult
    {
        Success,
        SelfOperation,
        UserNotFound,
        NotFollowing
    };

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
     * Validation includes self-follow prevention, user existence checks, and
     * duplicate prevention.
     *
     * Returns:
     *   A FollowResult enumerator describing the outcome:
     *     - Success: relationship created and followers/following sets updated
     *     - SelfOperation: follower and followee are the same user
     *     - UserNotFound: at least one user does not exist in the system
     *     - AlreadyFollowing: follower already follows the followee
     */
    FollowResult follow(const std::string &follower, const std::string &followee);

    /*
     * Function: unfollow
     * ------------------
     * Remove an existing directed follow relationship from follower -> followee.
     * Validation includes self-unfollow prevention and user existence checks.
     *
     * Returns:
     *   An UnfollowResult enumerator describing the outcome:
     *     - Success: relationship removed and followers/following sets updated
     *     - SelfOperation: follower and followee are the same user
     *     - UserNotFound: at least one user does not exist in the system
     *     - NotFollowing: no follow relationship exists between the users
     */
    UnfollowResult unfollow(const std::string &follower, const std::string &followee);

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
     * Retrieve all users that have a bidirectional (mutual) relationship with the given user.
     * A mutual connection is a user who both follows the target user and is followed by them.
     * Returns an empty vector if the user does not exist.
     */
    std::vector<std::string> getMutualConnections(const std::string &username) const;

private:
    UserManager &userManager;
};
