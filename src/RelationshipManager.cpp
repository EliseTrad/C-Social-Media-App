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
 * - Prevent self-follow (returns SelfOperation).
 * - Require both users to exist (returns UserNotFound if missing).
 * - Prevent duplicate follows (returns AlreadyFollowing if exists).
 *
 * On success, updates both followers and following sets bidirectionally,
 * and adds a notification to the followee's feed.
 *
 * Returns:
 *   A FollowResult enumerator indicating success or specific failure reason.
 */
RelationshipManager::FollowResult RelationshipManager::follow(const std::string &follower, const std::string &followee)
{
	// Prevent self-follow
	if (follower == followee)
	{
		return FollowResult::SelfOperation;
	}

	User *followerUser = userManager.findUser(follower);
	User *followeeUser = userManager.findUser(followee);

	// Both users must exist
	if (!followerUser || !followeeUser)
	{
		return FollowResult::UserNotFound;
	}

	// Prevent duplicate follows
	if (followerUser->following.count(followee) > 0)
	{
		return FollowResult::AlreadyFollowing;
	}

	followerUser->following.insert(followee);
	followeeUser->followers.insert(follower);

	// Add notification to followee's feed
	followeeUser->addNotification(follower + " started following you!");

	return FollowResult::Success;
}

/*
 * Function: unfollow
 * ------------------
 * Removes a directed edge follower -> followee.
 *
 * Validation:
 * - Prevent self-unfollow (returns SelfOperation).
 * - Require both users to exist (returns UserNotFound if missing).
 * - Require the follow relationship to be present (returns NotFollowing if absent).
 *
 * On success, removes from both followers and following sets bidirectionally,
 * and adds a notification to the unfollowee's feed.
 *
 * Returns:
 *   An UnfollowResult enumerator indicating success or specific failure reason.
 */
RelationshipManager::UnfollowResult RelationshipManager::unfollow(const std::string &follower, const std::string &followee)
{
	// Prevent self-unfollow
	if (follower == followee)
	{
		return UnfollowResult::SelfOperation;
	}

	User *followerUser = userManager.findUser(follower);
	User *followeeUser = userManager.findUser(followee);

	if (!followerUser || !followeeUser)
	{
		return UnfollowResult::UserNotFound;
	}

	if (followerUser->following.count(followee) == 0)
	{
		return UnfollowResult::NotFollowing;
	}

	followerUser->following.erase(followee);
	followeeUser->followers.erase(follower);

	// Add notification to followee's feed
	followeeUser->addNotification(follower + " unfollowed you.");

	return UnfollowResult::Success;
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
 * Retrieves all users that have a bidirectional (mutual) relationship with the target user.
 * A mutual connection is a user who both follows the target user and is followed by them.
 *
 * Strategy:
 * - Iterate over the user's followers set (smaller or equal size in most cases).
 * - Check membership in the user's following set.
 * - Complexity: O(min(|followers|, |following|)) average case.
 *
 * Returns:
 *   Vector of mutual connection usernames, or empty if user does not exist.
 */
std::vector<std::string> RelationshipManager::getMutualConnections(const std::string &username) const
{
	const User *user = userManager.findUser(username);
	if (!user)
	{
		return {};
	}

	const std::unordered_set<std::string> &followers = user->followers;
	const std::unordered_set<std::string> &following = user->following;

	// Find users that both follow and are followed by the target user
	const std::unordered_set<std::string> &smaller = (followers.size() <= following.size()) ? followers : following;
	const std::unordered_set<std::string> &larger = (followers.size() > following.size()) ? followers : following;

	std::vector<std::string> mutual;
	for (const auto &connection : smaller)
	{
		if (larger.count(connection) > 0)
		{
			mutual.push_back(connection);
		}
	}

	return mutual;
}
