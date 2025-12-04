# AoC-Cpp-Template

Advent Of Code template for use in C++

# Requirements

- Docker

# Getting Started

- Clone this template and make a new repo.

- Open this project in VSCode.
    - Ensure you have the devcontainer plugin installed

- Open the dev container

- At the root of the project, create a .env file with a `AOC_SESSION` var in there. 
    - Get your session token using f12 in your browser.

- Each day, run `just new-day xx`, where xx is the day you are doing.

## Useful CLI Flags

- `--input <path>` / `--input=<path>`: override the puzzle input file.
- `--test-dir <path>` / `--test-dir=<path>`: override the tests directory (sample inputs).
- `--skip-part1`, `--skip-part2`, `--only-part1`, `--only-part2`: toggle puzzle execution per part.
- `--sample` / `--samples` / `--run-samples`: run only the sample inputs outside of GoogleTest. Use the part-selection flags above (e.g., `--only-part1`) to choose which parts execute. Shorthands like `--sample-part1` / `--sample-part2` are also available.
- `--run-input` / `--input-only` / `--puzzle`: skip GoogleTest and run the real puzzle input directly, again respecting the part-selection flags.