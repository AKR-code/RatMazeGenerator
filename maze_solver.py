"""Maze solver using backtracking."""


class MazeSolver:
    """Solves a binary maze grid using recursive backtracking.

    The grid uses the same convention as ``MazeGenerator.grid``:
        0 – wall / blocked
        1 – open path

    The rat starts at ``start`` (default: top-left open cell) and must
    reach ``end`` (default: bottom-right open cell).  The solver finds
    *a* solution path; it does not guarantee the shortest path.

    Attributes:
        solution (list[tuple[int, int]] | None):
            Ordered list of (row, col) positions on the solution path,
            or ``None`` if no path exists.
    """

    def __init__(
        self,
        grid: list[list[int]],
        start: tuple[int, int] | None = None,
        end: tuple[int, int] | None = None,
    ) -> None:
        if not grid or not grid[0]:
            raise ValueError("Grid must not be empty.")

        self.grid = grid
        self.rows = len(grid)
        self.cols = len(grid[0])

        self.start = start if start is not None else (1, 1)
        self.end = end if end is not None else (self.rows - 2, self.cols - 2)

        self._visited: list[list[bool]] = [
            [False] * self.cols for _ in range(self.rows)
        ]
        self._path: list[tuple[int, int]] = []
        self.solution: list[tuple[int, int]] | None = None

        self._solve(self.start[0], self.start[1])

    # ------------------------------------------------------------------
    # Private helpers
    # ------------------------------------------------------------------

    def _in_bounds(self, row: int, col: int) -> bool:
        return 0 <= row < self.rows and 0 <= col < self.cols

    def _solve(self, row: int, col: int) -> bool:
        """Recursive backtracking solver.  Returns True when solution is found."""
        if not self._in_bounds(row, col):
            return False
        if self.grid[row][col] == 0 or self._visited[row][col]:
            return False

        self._visited[row][col] = True
        self._path.append((row, col))

        if (row, col) == self.end:
            self.solution = list(self._path)
            return True

        for dr, dc in ((-1, 0), (1, 0), (0, -1), (0, 1)):
            if self._solve(row + dr, col + dc):
                return True

        # Backtrack
        self._path.pop()
        return False

    # ------------------------------------------------------------------
    # Public helpers
    # ------------------------------------------------------------------

    def display(self, show_path: bool = True) -> str:
        """Return an ASCII string of the maze overlaid with the solution path.

        Legend:
            █  – wall
            space – open, unvisited
            S  – start
            E  – end
            ·  – solution path (when ``show_path`` is True)
        """
        path_set = set(self.solution) if self.solution and show_path else set()
        start_r, start_c = self.start
        end_r, end_c = self.end

        lines: list[str] = []
        for r, row in enumerate(self.grid):
            line: list[str] = []
            for c, cell in enumerate(row):
                if r == start_r and c == start_c:
                    line.append("S")
                elif r == end_r and c == end_c:
                    line.append("E")
                elif cell == 0:
                    line.append("█")
                elif (r, c) in path_set:
                    line.append("·")
                else:
                    line.append(" ")
            lines.append("".join(line))
        return "\n".join(lines)
