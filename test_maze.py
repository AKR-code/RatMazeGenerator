"""Unit tests for the Rat Maze Generator and Solver."""

import pytest

from maze_generator import MazeGenerator
from maze_solver import MazeSolver


# ---------------------------------------------------------------------------
# MazeGenerator tests
# ---------------------------------------------------------------------------


class TestMazeGenerator:
    def test_invalid_size_raises(self):
        with pytest.raises(ValueError):
            MazeGenerator(1, 5)
        with pytest.raises(ValueError):
            MazeGenerator(5, 1)

    def test_grid_dimensions(self):
        gen = MazeGenerator(5, 7, seed=0)
        # Grid should be (2*rows+1) × (2*cols+1)
        assert len(gen.grid) == 11
        assert all(len(row) == 15 for row in gen.grid)

    def test_border_is_all_walls(self):
        gen = MazeGenerator(4, 4, seed=42)
        # Top and bottom rows must be all walls
        assert all(cell == 0 for cell in gen.grid[0])
        assert all(cell == 0 for cell in gen.grid[-1])
        # Left and right columns must be all walls
        assert all(gen.grid[r][0] == 0 for r in range(len(gen.grid)))
        assert all(gen.grid[r][-1] == 0 for r in range(len(gen.grid)))

    def test_all_cell_centres_are_open(self):
        rows, cols = 5, 5
        gen = MazeGenerator(rows, cols, seed=1)
        for r in range(rows):
            for c in range(cols):
                assert gen.grid[2 * r + 1][2 * c + 1] == 1, (
                    f"Cell centre ({r},{c}) should be open"
                )

    def test_reproducible_with_seed(self):
        gen_a = MazeGenerator(8, 8, seed=99)
        gen_b = MazeGenerator(8, 8, seed=99)
        assert gen_a.grid == gen_b.grid

    def test_different_seeds_differ(self):
        gen_a = MazeGenerator(8, 8, seed=1)
        gen_b = MazeGenerator(8, 8, seed=2)
        assert gen_a.grid != gen_b.grid

    def test_display_contains_start_and_end(self):
        gen = MazeGenerator(5, 5, seed=0)
        display = gen.display()
        assert "S" in display
        assert "E" in display

    def test_display_line_count(self):
        rows, cols = 4, 6
        gen = MazeGenerator(rows, cols, seed=0)
        lines = gen.display().splitlines()
        assert len(lines) == 2 * rows + 1

    def test_large_maze(self):
        # Just ensure no crash for a larger maze
        gen = MazeGenerator(20, 20, seed=7)
        assert len(gen.grid) == 41
        assert len(gen.grid[0]) == 41


# ---------------------------------------------------------------------------
# MazeSolver tests
# ---------------------------------------------------------------------------


class TestMazeSolver:
    def _gen(self, rows=5, cols=5, seed=0):
        return MazeGenerator(rows, cols, seed=seed)

    def test_solution_exists(self):
        gen = self._gen()
        solver = MazeSolver(gen.grid)
        assert solver.solution is not None

    def test_solution_starts_and_ends_correctly(self):
        gen = self._gen(seed=3)
        solver = MazeSolver(gen.grid)
        assert solver.solution is not None
        assert solver.solution[0] == solver.start
        assert solver.solution[-1] == solver.end

    def test_solution_path_is_connected(self):
        gen = self._gen(seed=5)
        solver = MazeSolver(gen.grid)
        assert solver.solution is not None
        for i in range(len(solver.solution) - 1):
            r0, c0 = solver.solution[i]
            r1, c1 = solver.solution[i + 1]
            assert abs(r0 - r1) + abs(c0 - c1) == 1, (
                "Consecutive path steps must be adjacent"
            )

    def test_solution_cells_are_open(self):
        gen = self._gen(seed=8)
        solver = MazeSolver(gen.grid)
        assert solver.solution is not None
        for r, c in solver.solution:
            assert gen.grid[r][c] == 1, f"Path cell ({r},{c}) should be open"

    def test_empty_grid_raises(self):
        with pytest.raises(ValueError):
            MazeSolver([])
        with pytest.raises(ValueError):
            MazeSolver([[]])

    def test_no_solution_returns_none(self):
        # Completely walled-in grid (except start) – no path to end
        grid = [
            [0, 0, 0],
            [0, 1, 0],
            [0, 0, 0],
        ]
        solver = MazeSolver(grid, start=(1, 1), end=(0, 0))
        assert solver.solution is None

    def test_custom_start_and_end(self):
        gen = MazeGenerator(6, 6, seed=11)
        solver = MazeSolver(gen.grid, start=(1, 1), end=(1, 1))
        # Start == end → trivial single-cell solution
        assert solver.solution == [(1, 1)]

    def test_display_shows_path_marker(self):
        gen = self._gen(seed=2)
        solver = MazeSolver(gen.grid)
        assert solver.solution is not None
        display = solver.display()
        assert "·" in display

    def test_display_no_path_marker_when_disabled(self):
        gen = self._gen(seed=2)
        solver = MazeSolver(gen.grid)
        display = solver.display(show_path=False)
        assert "·" not in display

    def test_display_contains_start_and_end(self):
        gen = self._gen(seed=4)
        solver = MazeSolver(gen.grid)
        display = solver.display()
        assert "S" in display
        assert "E" in display

    @pytest.mark.parametrize("seed", range(20))
    def test_various_seeds_all_solvable(self, seed):
        gen = MazeGenerator(7, 7, seed=seed)
        solver = MazeSolver(gen.grid)
        assert solver.solution is not None, (
            f"Maze with seed={seed} should always be solvable"
        )
