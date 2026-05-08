# Command-Line Social Media (C++)

Lightweight command-line social graph simulator written in modern C++ (C++17).
This repository implements an in-memory user registry, directed follow
relationships, session handling, and a simple notification feed — all exposed
via a small CLI.

## Project Overview

This program demonstrates core social-graph operations and data-structure
tradeoffs (hash maps/sets, queues, stacks) in a compact, self-contained C++
codebase. It is intended as an educational backend simulator (no network server
or persistence).

Primary behaviors:

- Create accounts (`signup`) and authenticate (`login`/`logout`).
- Follow/unfollow other users (directed edges) with bidirectional consistency.
- Query followers, following, and mutual connections.
- Simple per-user FIFO notification queue populated on follow/unfollow events.

The CLI entrypoint is [src/main.cpp](src/main.cpp), which reads lines from
standard input and dispatches them to `CommandProcessor`.

## Tech Stack

- Language: C++17
- Standard library containers: `std::unordered_map`, `std::unordered_set`,
  `std::queue`, `std::stack`, `std::vector`
- Build: g++ / MinGW (example tasks provided for VS Code)

## Key Features

- Account management: `signup`, `login`, `logout` (passwords stored as hashed
  strings using a deterministic hash + constant salt in `AuthManager`).
- Relationship graph: directed `follow` / `unfollow` with follower/following
  sets kept in sync (`RelationshipManager`).
- Notifications: follow/unfollow actions enqueue human-readable messages into
  the target user's FIFO feed (`User::notifications`).
- CLI command dispatch is implemented with a hash map for O(1) average lookup
  (`CommandProcessor`).

## Constraints & Validation Rules

- Usernames: letters only (a–z, A–Z), maximum length 8 characters.
- Passwords: must not be empty.
- All state is stored in memory — there is no persistence or database.

These rules are enforced in [src/AuthManager.cpp](src/AuthManager.cpp).

## Folder Structure

- [src/](src/) — C++ source and header files
  - [main.cpp](src/main.cpp) — CLI entrypoint and main loop
  - [CommandProcessor.h](src/CommandProcessor.h),
    [CommandProcessor.cpp](src/CommandProcessor.cpp)
  - [AuthManager.h](src/AuthManager.h), [AuthManager.cpp](src/AuthManager.cpp)
  - [UserManager.h](src/UserManager.h), [UserManager.cpp](src/UserManager.cpp)
  - [RelationshipManager.h](src/RelationshipManager.h),
    [RelationshipManager.cpp](src/RelationshipManager.cpp)
  - [User.h](src/User.h), [User.cpp](src/User.cpp)
- [tests/](tests/)
  - [smoke_test_commands.txt](tests/smoke_test_commands.txt) — scripted smoke
    test input
- [.vscode/](.vscode/) — VS Code tasks and launch configuration
- README.md — this file

## Installation / Build

Requirements:

- A C++17-capable compiler (g++/MinGW, clang, or MSVC compatible with C++17).

From the repository root (Windows PowerShell example):

```powershell
g++ -std=c++17 -fdiagnostics-color=always -g -o src/main.exe \
  src/main.cpp src/CommandProcessor.cpp src/AuthManager.cpp \
  src/RelationshipManager.cpp src/UserManager.cpp src/User.cpp
```

Notes:

- The project includes a VS Code task at
  [.vscode/tasks.json](.vscode/tasks.json) called "C/C++: Build Social Media
  App" which runs a similar g++ command.

## Environment Variables

This project does not require any environment variables.

## Usage / Running the Project

Run the compiled binary and interact with the CLI via standard input.

Windows PowerShell:

```powershell
.\src\main.exe
```

You can run the provided smoke test by piping the test file into the executable:

```powershell
Get-Content .\tests\smoke_test_commands.txt | .\src\main.exe
```

Behavior:

- The program reads commands until `exit` is entered (or EOF).
- Unknown commands print an "Unknown command" message. The `help` command prints
  the supported commands list.

### Supported Commands

- `help` — list available commands
- `signup <username> <password>` — create a new user
- `login <username> <password>` — authenticate and push session
- `logout` — pop the active session
- `follow <username>` — current user follows target
- `unfollow <username>` — current user unfollows target
- `followers [username]` — list followers (defaults to current user)
- `following [username]` — list who the user follows (defaults to current user)
- `mutuals [username]` — list mutual (bidirectional) connections
- `feed_next` — pop and print next notification for current user
- `echo <text...>` — print provided text
- `exit` — terminate the CLI loop

See the command implementations in
[src/CommandProcessor.cpp](src/CommandProcessor.cpp) for exact messaging and
error text.

## Tests

- A smoke-test command sequence is provided in
  [tests/smoke_test_commands.txt](tests/smoke_test_commands.txt).
- There are no automated unit tests in this repository currently.

## Limitations

- In-memory only: restarting the program loses all state.
- Deterministic, non-salted hash: password hashing is a simple deterministic
  hash combined with a constant salt string (see
  [src/AuthManager.cpp](src/AuthManager.cpp)).
- No concurrency control (single-threaded CLI).

## Future Improvements (non-exhaustive)

- Add persistent storage (file or database) for users and relationships.
- Replace the simple hashing scheme with a proper password-hashing algorithm
  (e.g., bcrypt, Argon2) and unique per-user salts.
- Add unit tests and CI integration.
- Provide a network API (HTTP/gRPC) with authentication tokens.

## Where to Look In The Code

- CLI and dispatch: [src/main.cpp](src/main.cpp),
  [src/CommandProcessor.cpp](src/CommandProcessor.cpp)
- Authentication & sessions: [src/AuthManager.cpp](src/AuthManager.cpp)
- User storage: [src/UserManager.cpp](src/UserManager.cpp)
- Relationship logic: [src/RelationshipManager.cpp](src/RelationshipManager.cpp)
- Notification queue: [src/User.cpp](src/User.cpp)
