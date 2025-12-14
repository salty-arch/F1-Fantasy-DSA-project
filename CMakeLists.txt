cmake_minimum_required(VERSION 3.10)
project(F1_Fantasy)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Source files (exclude GUI files)
set(SOURCES
        "src/Game systems/main.cpp"                    # ADD src/ prefix
        src/Core/Constructors.cpp            # ADD src/ prefix
        src/Core/Driver.cpp                  # ADD src/ prefix
        "src/Game systems/RaceSimulator.cpp"           # ADD src/ prefix
        "src/Game systems/Leaderboard.cpp"             # ADD src/ prefix
        # ADD src/ prefix
        src/dsa/AVLTree.cpp                 # ADD src/ prefix
        # ADD src/ prefix
        src/dsa/Sorting.cpp                 # ADD src/ prefix
        src/Core/Team.cpp                    # ADD src/ prefix
        src/Core/User.cpp                    # ADD src/ prefix
        src/dsa/Trie.cpp                    # ADD src/ prefix
)

set(HEADERS
        src/Core/Constructors.h              # ADD src/ prefix
        src/Core/Driver.h                    # ADD src/ prefix
        "src/Game systems/RaceSimulator.h"             # ADD src/ prefix
        "src/Game systems/Leaderboard.h"               # ADD src/ prefix
        # ADD src/ prefix
        src/dsa/AVLTree.h                   # ADD src/ prefix
        # ADD src/ prefix
        src/dsa/Sorting.h                   # ADD src/ prefix
        src/Core/User.h                      # ADD src/ prefix
        src/Core/Team.h                      # ADD src/ prefix
        src/dsa/Trie.h                      # ADD src/ prefix
)
# Create executable
add_executable(F1_Fantasy ${SOURCES} ${HEADERS})

# Optional: Add compiler warnings
if(MSVC)
    target_compile_options(F1_Fantasy PRIVATE /W4)
else()
    target_compile_options(F1_Fantasy PRIVATE -Wall -Wextra -pedantic)
endif()