# Command-Line Social Media Backend (C++)

This project is a command-line social media backend written in C++ that
simulates a small subset of Instagram-style behavior. The implementation is
intentionally centered on data structures and algorithmic tradeoffs: user
storage, relationship graphs, session tracking, command dispatch, and
notification delivery are all built on standard library containers chosen for
their asymptotic behavior.

## Overview

The program provides the following capabilities:

- User registration and login/logout authentication
- Directed follow and unfollow relationships
- Retrieval of followers, following, and mutual connections
- Notification delivery through a FIFO queue
- Command-line parsing and command dispatch through a hash-based lookup table

The execution flow is:

1. `main.cpp` reads each input line from standard input.
2. `CommandProcessor` parses the command and arguments.
3. The requested operation is dispatched to `AuthManager` or
   `RelationshipManager`.
4. Managers update shared in-memory state stored by `UserManager` and `User`.

## Data Structures

The implementation uses the following core containers:

- `std::unordered_map<std::string, User>` for the user registry, providing O(1)
  average-case username lookup and insertion.
- `std::unordered_set<std::string>` for `followers` and `following`, giving O(1)
  average-case membership checks, insertions, and deletions.
- An implicit directed graph represented as adjacency lists stored inside each
  `User` object.
- `std::stack<std::string>` for active session tracking in `AuthManager`.
- `std::queue<std::string>` for notifications in each `User` object.
- `std::unordered_map<std::string, std::function<void(const std::vector<std::string>&)>>`
  for command dispatch in `CommandProcessor`.

This design is optimized for sparse graphs, which is the natural shape of social
networks. An adjacency list is far more space-efficient than an adjacency matrix
because the number of actual follow relationships is typically much smaller than
the number of all possible user pairs.

## Build

Any C++17-compatible compiler should work. From the repository root, a typical
MinGW or GCC build looks like this:

```powershell
g++ -std=c++17 -O2 -g ^
  src\main.cpp src\CommandProcessor.cpp src\AuthManager.cpp ^
  src\RelationshipManager.cpp src\UserManager.cpp src\User.cpp ^
  -o src\main.exe
```

If your compiler is not on `PATH`, replace `g++` with the full path to your
compiler executable.

## Run

```powershell
.\src\main.exe
```

The program reads commands from standard input until the line `exit` is entered.

## Commands

- `help` prints the list of supported commands.
- `signup <username> <password>` creates a new account.
- `login <username> <password>` authenticates a user and pushes that username
  onto the session stack.
- `logout` pops the current session from the stack.
- `follow <username>` creates a directed follow edge from the current logged-in
  user to the target user.
- `unfollow <username>` removes the directed follow edge from the current user
  to the target user.
- `followers [username]` prints the followers of the given user, or the current
  logged-in user if the username is omitted.
- `following [username]` prints the accounts followed by the given user, or the
  current logged-in user if the username is omitted.
- `mutuals [username]` prints mutual connections, using the current logged-in
  user when no username is supplied.
- `feed_next` prints and removes the next notification from the current user's
  queue.
- `echo <text...>` prints the remaining tokens exactly as entered, separated by
  spaces.
- `exit` terminates the read loop in `main.cpp`.

## Behavioral Notes

- `followers`, `following`, and `mutuals` work without an argument only when a
  user is already logged in.
- If no username is provided for those commands and no session exists, the
  program prints an error.
- `follow` and `unfollow` require an active session.
- `feed_next` requires an active session and returns the next queued
  notification for the current user.
- Relationship iteration order is not guaranteed because the implementation uses
  `std::unordered_set`, so output order may vary between runs.
- Notifications are generated when a user follows or unfollows another user.
- Usernames and passwords may not be empty.
- The current implementation stores everything in memory; there is no database
  or file persistence.

## Complexity Summary

The design prioritizes O(1) average-case performance for the most common
operations:

- User signup and lookup: O(1) average
- Login/logout/session access: O(1)
- Follow/unfollow/isFollowing: O(1) average
- Followers/following retrieval: O(k), where k is the number of relationships
  returned
- Mutual connections: O(min(|followers|, |following|)) average
- Notification enqueue/dequeue: O(1)
- Command dispatch: O(1) average

The overall graph storage cost is O(n + e), where n is the number of users and e
is the number of directed follow relationships.

## Test Data

A scripted smoke test is provided in `tests/smoke_test_commands.txt`.

```powershell
Get-Content .\tests\smoke_test_commands.txt | .\src\main.exe
```

The smoke test covers user registration, login/logout, follow/unfollow behavior,
follower/following retrieval, mutual connection lookup, and feed notification
retrieval.

## Implementation Notes

- `UserManager` owns the central user registry.
- `AuthManager` handles password hashing, login/logout, and the current session
  stack.
- `RelationshipManager` enforces relationship invariants such as self-follow
  prevention, duplicate follow rejection, and bidirectional consistency between
  follower and following sets.
- `CommandProcessor` centralizes parsing and dispatch so the CLI does not rely
  on long if-else chains.

The result is a compact in-memory backend that is easy to reason about
academically and demonstrates why hash-based containers are often the right
choice for large-scale social graph operations.
