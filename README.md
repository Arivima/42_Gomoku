# Gomoku

A graphical Gomoku implementation in C++17 with SFML, featuring full game rules enforcement, an AI engine, and a polished dark/light UI.

---

## Features

- **Full rule enforcement** — win by 5-in-a-row, pair captures, endgame capture rule, double-three prohibition
- **Opening rules** — Standard, Pro, Swap, Swap2 (selectable at start screen)
- **Game modes** — Human vs AI, Human vs Human (hotseat)
- **AI engine** — heuristic evaluation with move ranking and coach mode (move suggestions)
- **Undo / Redo** — full game state restoration at any depth
- **Move history** — scrollable popup showing the complete move log
- **Debug mode** — board overlays showing threats, forbidden cells, and AI suggestions
- **Dark / light theme** — toggle at runtime
- **AI stats** — per-move and aggregate timing displayed in the sidebar

---

## Quickstart

### Prerequisites

- macOS with [Homebrew](https://brew.sh)
- SFML 3.x
- g++ with C++17 support
- Google Test (for tests only)

```bash
brew install sfml
brew install googletest   # optional, for tests
```

### Build & run

```bash
git clone <repo-url>
cd 42_Gomoku
make run
```

Other Makefile targets:

| Target | Description |
|--------|-------------|
| `make` / `make all` | Build the executable to `bin/Gomoku` |
| `make run` | Build and launch |
| `make test` | Build and run the Google Test suite |
| `make clean` | Remove compiled objects |
| `make fclean` | Remove objects and binary |
| `make re` | Full rebuild |

---

## Controls

| Key | Action |
|-----|--------|
| Click | Place stone |
| `R` | Restart game (returns to start screen) |
| `Z` | Undo last move |
| `A` | Redo |
| `C` | Toggle coach mode (show AI suggestions) |
| `D` | Toggle debug overlay (threats, forbidden cells) |
| `P` | Toggle dark/light theme |
| `I` | Show rules popup |
| `H` | Show move history |
| `Q` / `Esc` | Quit |

---

## Project structure

```
42_Gomoku/
├── src/
│   ├── main.cpp
│   ├── app.cpp
│   ├── core/           # Game logic: board, rules, AI
│   └── rendering/      # SFML renderers
├── include/
│   ├── app.hpp
│   ├── core/
│   └── rendering/
├── tests/              # Google Test suite
├── assets/fonts/       # Roboto font
├── docs/               # Architecture, review, roadmap, glossary
└── Makefile
```

See [docs/archi.md](docs/archi.md) for a full architecture overview and [docs/review.md](docs/review.md) for the technical review and roadmap.
