"""Entry point for the Rat Maze Generator & Solver."""

import argparse

from maze_generator import MazeGenerator
from maze_solver import MazeSolver


def parse_args() -> argparse.Namespace:
    parser = argparse.ArgumentParser(
        description="Generate a random rat maze and solve it."
    )
    parser.add_argument(
        "--rows",
        type=int,
        default=10,
        help="Number of maze rows (default: 10, minimum: 2)",
    )
    parser.add_argument(
        "--cols",
        type=int,
        default=10,
        help="Number of maze columns (default: 10, minimum: 2)",
    )
    parser.add_argument(
        "--seed",
        type=int,
        default=None,
        help="Random seed for reproducible mazes (default: random)",
    )
    return parser.parse_args()


def main() -> None:
    args = parse_args()

    print(f"\nGenerating a {args.rows}×{args.cols} maze …\n")
    generator = MazeGenerator(args.rows, args.cols, seed=args.seed)
    print(generator.display())

    print("\nSolving the maze …\n")
    solver = MazeSolver(generator.grid)

    if solver.solution:
        print(solver.display())
        print(f"\nSolution found! Path length: {len(solver.solution)} steps.")
    else:
        print("No solution found.")


if __name__ == "__main__":
    main()
