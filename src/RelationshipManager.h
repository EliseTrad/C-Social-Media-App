#pragma once

#include <string>
#include <vector>

#include "UserManager.h"

class RelationshipManager
{
public:
    explicit RelationshipManager(UserManager &userManager);

    bool follow(const std::string &follower, const std::string &followee);
    bool unfollow(const std::string &follower, const std::string &followee);
    bool isFollowing(const std::string &follower, const std::string &followee) const;

    std::vector<std::string> getFollowers(const std::string &username) const;
    std::vector<std::string> getFollowing(const std::string &username) const;

private:
    UserManager &userManager;
};
