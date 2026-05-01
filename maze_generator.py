"""Maze generator using recursive backtracking (DFS)."""

import random


class MazeGenerator:
    """Generates a random maze on a grid using recursive backtracking.

    The maze is represented as a 2-D grid of cells where each cell tracks
    which of its four walls (N, S, E, W) are still standing.  After
    generation, every pair of adjacent cells is either connected (wall
    removed) or separated (wall intact), guaranteeing a perfect maze with
    exactly one path between any two cells.

    For external consumers the maze is also exposed as a binary grid
    (``self.grid``) that is compatible with the solver:
        0 – wall / blocked
        1 – open path
    """

    # Direction vectors: (row-delta, col-delta)
    DIRECTIONS = {
        "N": (-1, 0),
        "S": (1, 0),
        "E": (0, 1),
        "W": (0, -1),
    }
    OPPOSITE = {"N": "S", "S": "N", "E": "W", "W": "E"}

    def __init__(self, rows: int, cols: int, seed: int | None = None) -> None:
        if rows < 2 or cols < 2:
            raise ValueError("Maze must be at least 2×2.")
        self.rows = rows
        self.cols = cols
        self._rng = random.Random(seed)

        # Each cell stores which walls have been *removed* (True = open).
        self._walls: list[list[dict[str, bool]]] = [
            [{"N": False, "S": False, "E": False, "W": False} for _ in range(cols)]
            for _ in range(rows)
        ]
        self._visited: list[list[bool]] = [
            [False] * cols for _ in range(rows)
        ]

        self._generate(0, 0)
        self.grid = self._build_grid()

    # ------------------------------------------------------------------
    # Private helpers
    # ------------------------------------------------------------------

    def _in_bounds(self, row: int, col: int) -> bool:
        return 0 <= row < self.rows and 0 <= col < self.cols

    def _generate(self, row: int, col: int) -> None:
        """Recursive DFS carver."""
        self._visited[row][col] = True
        directions = list(self.DIRECTIONS.keys())
        self._rng.shuffle(directions)

        for direction in directions:
            dr, dc = self.DIRECTIONS[direction]
            nr, nc = row + dr, col + dc
            if self._in_bounds(nr, nc) and not self._visited[nr][nc]:
                # Remove wall between (row, col) and (nr, nc)
                self._walls[row][col][direction] = True
                self._walls[nr][nc][self.OPPOSITE[direction]] = True
                self._generate(nr, nc)

    def _build_grid(self) -> list[list[int]]:
        """Convert the wall structure into a binary grid.

        Each logical cell becomes a 2×2 block in the grid so that walls
        are represented as impassable cells between blocks.  The full grid
        has dimensions (2*rows + 1) × (2*cols + 1).
        """
        h = 2 * self.rows + 1
        w = 2 * self.cols + 1
        grid = [[0] * w for _ in range(h)]

        for r in range(self.rows):
            for c in range(self.cols):
                # Cell centre
                gr, gc = 2 * r + 1, 2 * c + 1
                grid[gr][gc] = 1

                # Open southern passage
                if self._walls[r][c]["S"]:
                    grid[gr + 1][gc] = 1
                # Open eastern passage
                if self._walls[r][c]["E"]:
                    grid[gr][gc + 1] = 1

        return grid

    # ------------------------------------------------------------------
    # Public helpers
    # ------------------------------------------------------------------

    def display(self) -> str:
        """Return an ASCII string representation of the maze.

        Legend:
            █  – wall
            space – open path
            S  – start (top-left cell)
            E  – end (bottom-right cell)
        """
        lines: list[str] = []
        start_r, start_c = 1, 1
        end_r, end_c = 2 * self.rows - 1, 2 * self.cols - 1

        for r, row in enumerate(self.grid):
            line: list[str] = []
            for c, cell in enumerate(row):
                if r == start_r and c == start_c:
                    line.append("S")
                elif r == end_r and c == end_c:
                    line.append("E")
                elif cell == 0:
                    line.append("█")
                else:
                    line.append(" ")
            lines.append("".join(line))
        return "\n".join(lines)
