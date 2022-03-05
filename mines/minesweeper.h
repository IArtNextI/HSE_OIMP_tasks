#pragma once

#include <chrono>
#include <random>
#include <string>
#include <set>
#include <vector>

class Minesweeper {
public:
    struct Cell {
        size_t x = 0;
        size_t y = 0;

        bool operator<(const Cell other) const;
    };

    enum class GameStatus {
        NOT_STARTED,
        IN_PROGRESS,
        VICTORY,
        DEFEAT,
    };

    using RenderedField = std::vector<std::string>;

    Minesweeper(size_t width, size_t height, size_t mines_count);
    Minesweeper(size_t width, size_t height, const std::vector<Cell>& cells_with_mines);

    void NewGame(size_t width, size_t height, size_t mines_count);
    void NewGame(size_t width, size_t height, const std::vector<Cell>& cells_with_mines);

    void OpenCell(const Cell& cell);
    void MarkCell(const Cell& cell);

    GameStatus GetGameStatus() const;
    time_t GetGameTime() const;

    RenderedField RenderField() const;

    ~Minesweeper();

private:
    size_t width_;
    size_t height_;
    size_t mines_count_;
    bool** cells_with_mines_;
    bool** cells_opened_;
    bool** cells_marked_;
    size_t opened_cells_;
    std::mt19937 random_;
    GameStatus game_status_;
    std::chrono::system_clock::time_point start_of_current_game_;

    bool** MakeBoolArray(size_t height, size_t width);
    void DeleteBoolArray(bool** ptr, size_t height);

    bool ValidNeighbour(int64_t x, int64_t y) const;
    size_t MineNeighbours(int64_t x, int64_t y) const;
};
