# RatMazeGenerator

A Python program that **generates** a random rat maze and **solves** it using backtracking algorithms.

## How it works

| Module | Purpose |
|---|---|
| `maze_generator.py` | Generates a perfect maze using recursive backtracking (DFS). Every maze has exactly one path between any two cells. |
| `maze_solver.py` | Solves the maze with recursive backtracking, marking the solution path. |
| `main.py` | Command-line entry point that ties generation and solving together with ASCII output. |
| `test_maze.py` | Unit tests for both the generator and solver. |

## Requirements

- Python 3.10+

## Usage

```bash
python main.py [--rows ROWS] [--cols COLS] [--seed SEED]
```

| Option | Default | Description |
|---|---|---|
| `--rows` | 10 | Number of logical maze rows (≥ 2) |
| `--cols` | 10 | Number of logical maze columns (≥ 2) |
| `--seed` | random | Integer seed for reproducible mazes |

### Example

```
$ python main.py --rows 8 --cols 8 --seed 42

Generating a 8×8 maze …

█████████████████
█S    █ █       █
█████ █ █ █████ █
...

Solving the maze …

█████████████████
█S····█ █       █
█████·█ █ █████ █
...
Solution found! Path length: 33 steps.
```

**Legend:**
- `█` – wall
- `S` – start (top-left cell)
- `E` – end (bottom-right cell)
- `·` – solution path

## Running the tests

```bash
python -m pytest test_maze.py -v
```
