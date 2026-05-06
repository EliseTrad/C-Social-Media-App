// RelationshipManager.cpp
#include "RelationshipManager.h"

#include <algorithm>

/*
 * RelationshipManager implementation
 * ---------------------------------
 * Implements follow/unfollow operations and provides query helpers.
 * Implementation focuses on clear validation, bidirectional consistency,
 * and efficient average-case operations using hash sets stored in `User`.
 */

RelationshipManager::RelationshipManager(UserManager &userManager) : userManager(userManager) {}

bool RelationshipManager::follow(const std::string &follower, const std::string &followee)
{
	// Prevent self-follow
	if (follower == followee)
	{
		return false;
	}

	User *followerUser = userManager.findUser(follower);
	User *followeeUser = userManager.findUser(followee);

	// Both users must exist
	if (!followerUser || !followeeUser)
	{
		return false;
	}

	// Prevent duplicate follows
	if (followerUser->following.count(followee) > 0)
	{
		return false;
	}

	followerUser->following.insert(followee);
	followeeUser->followers.insert(follower);
	return true;
}

bool RelationshipManager::unfollow(const std::string &follower, const std::string &followee)
{
	User *followerUser = userManager.findUser(follower);
	User *followeeUser = userManager.findUser(followee);

	if (!followerUser || !followeeUser)
	{
		return false;
	}

	if (followerUser->following.count(followee) == 0)
	{
		return false;
	}

	followerUser->following.erase(followee);
	followeeUser->followers.erase(follower);
	return true;
}

bool RelationshipManager::isFollowing(const std::string &follower, const std::string &followee) const
{
	const User *followerUser = userManager.findUser(follower);
	if (!followerUser)
	{
		return false;
	}

	return followerUser->following.count(followee) > 0;
}

std::vector<std::string> RelationshipManager::getFollowers(const std::string &username) const
{
	const User *user = userManager.findUser(username);
	if (!user)
	{
		return {};
	}

	std::vector<std::string> followers;
	followers.reserve(user->followers.size());

	for (const auto &f : user->followers)
	{
		followers.push_back(f);
	}

	return followers;
}

std::vector<std::string> RelationshipManager::getFollowing(const std::string &username) const
{
	const User *user = userManager.findUser(username);
	if (!user)
	{
		return {};
	}

	std::vector<std::string> following;
	following.reserve(user->following.size());

	for (const auto &f : user->following)
	{
		following.push_back(f);
	}

	return following;
}

std::vector<std::string> RelationshipManager::getMutualConnections(const std::string &userA, const std::string &userB) const
{
	const User *firstUser = userManager.findUser(userA);
	const User *secondUser = userManager.findUser(userB);

	if (!firstUser || !secondUser)
	{
		return {};
	}

	const std::unordered_set<std::string> &setA = firstUser->following;
	const std::unordered_set<std::string> &setB = secondUser->following;

	const std::unordered_set<std::string> &smaller = (setA.size() <= setB.size()) ? setA : setB;
	const std::unordered_set<std::string> &larger = (setA.size() > setB.size()) ? setA : setB;

	std::vector<std::string> mutual;
	for (const auto &c : smaller)
	{
		if (larger.count(c) > 0)
		{
			mutual.push_back(c);
		}
	}

	return mutual;
}
