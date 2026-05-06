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
    explicit RelationshipManager(UserManager &userManager);

    // Establish a directed follow from `follower` -> `followee`.
    // Returns true on success, false on error (self-follow, duplicate,
    // or missing user).
    bool follow(const std::string &follower, const std::string &followee);

    // Remove a previously established follow. Returns true on success,
    // false if users are missing or the relationship does not exist.
    bool unfollow(const std::string &follower, const std::string &followee);

    // Returns whether `follower` is currently following `followee`.
    bool isFollowing(const std::string &follower, const std::string &followee) const;

    // Query operations return lists of usernames. If the user does not
    // exist the returned vector is empty.
    std::vector<std::string> getFollowers(const std::string &username) const;
    std::vector<std::string> getFollowing(const std::string &username) const;

    // Compute mutual connections as the intersection of two users' following
    // sets. Returns an empty vector if either user does not exist.
    std::vector<std::string> getMutualConnections(const std::string &userA, const std::string &userB) const;

private:
    UserManager &userManager;
};
