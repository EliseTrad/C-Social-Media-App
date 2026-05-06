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

/*
 * Function: RelationshipManager
 * -----------------------------
 * Constructs a relationship manager bound to a shared UserManager.
 *
 * Parameters:
 *   userManager - reference used for user existence and lookup operations.
 */
RelationshipManager::RelationshipManager(UserManager &userManager) : userManager(userManager) {}

/*
 * Function: follow
 * ----------------
 * Creates a directed edge follower -> followee.
 *
 * Validation:
 * - Prevent self-follow.
 * - Require both users to exist.
 * - Prevent duplicate follows.
 *
 * Returns:
 *   true on success, false otherwise.
 */
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

/*
 * Function: unfollow
 * ------------------
 * Removes a directed edge follower -> followee.
 *
 * Validation:
 * - Require both users to exist.
 * - Require the follow relationship to be present.
 *
 * Returns:
 *   true on success, false otherwise.
 */
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

/*
 * Function: isFollowing
 * ---------------------
 * Checks whether follower currently follows followee.
 *
 * Returns:
 *   true if the directed relationship exists, false otherwise.
 */
bool RelationshipManager::isFollowing(const std::string &follower, const std::string &followee) const
{
	const User *followerUser = userManager.findUser(follower);
	if (!followerUser)
	{
		return false;
	}

	return followerUser->following.count(followee) > 0;
}

/*
 * Function: getFollowers
 * ----------------------
 * Returns all users that follow the given username.
 *
 * Returns:
 *   Vector of follower usernames, or empty if user does not exist.
 */
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

/*
 * Function: getFollowing
 * ----------------------
 * Returns all users followed by the given username.
 *
 * Returns:
 *   Vector of followed usernames, or empty if user does not exist.
 */
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

/*
 * Function: getMutualConnections
 * ------------------------------
 * Computes intersection of two users' following sets.
 *
 * Strategy:
 * - Iterate over the smaller set for efficiency.
 * - Use hash membership checks on the larger set.
 *
 * Returns:
 *   Vector of mutual connections, or empty if either user is missing.
 */
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
