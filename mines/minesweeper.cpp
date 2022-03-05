#include "minesweeper.h"

#include <algorithm>
#include <tuple>

bool Minesweeper::Cell::operator<(const Cell other) const {
    return std::tie(x, y) < std::tie(other.x, other.y);
}

bool** Minesweeper::MakeBoolArray(size_t height, size_t width) {
    bool** result = new bool*[height];
    for (size_t i = 0; i < height; ++i) {
        result[i] = new bool[width];
        for (size_t j = 0; j < width; ++j) {
            result[i][j] = false;
        }
    }
    return result;
}

void Minesweeper::DeleteBoolArray(bool** ptr, size_t height) {
    for (size_t i = 0; i < height; ++i) {
        delete[] ptr[i];
    }
    delete[] ptr;
};

Minesweeper::~Minesweeper() {
    DeleteBoolArray(cells_with_mines_, height_);
    DeleteBoolArray(cells_marked_, height_);
    DeleteBoolArray(cells_opened_, height_);
}

Minesweeper::Minesweeper(size_t width, size_t height, size_t mines_count) {
    game_status_ = GameStatus::NOT_STARTED;
    random_ = std::mt19937(std::chrono::high_resolution_clock::now().time_since_epoch().count());
    width_ = width;
    height_ = height;
    mines_count_ = mines_count;
    opened_cells_ = 0;
    cells_with_mines_ = MakeBoolArray(height, width);
    cells_marked_ = MakeBoolArray(height, width);
    cells_opened_ = MakeBoolArray(height, width);
    std::vector<bool> mines(width * height, false);
    for (size_t i = 0; i < mines_count; ++i) {
        mines[i] = true;
    }
    std::shuffle(mines.begin(), mines.end(), random_);
    for (size_t i = 0; i < mines_count; ++i) {
        if (mines[i]) {
            cells_with_mines_[i / width][i % width] = 1;
        }
    }
}

Minesweeper::Minesweeper(size_t width, size_t height, const std::vector<Cell>& cells_with_mines) {
    game_status_ = GameStatus::NOT_STARTED;
    random_ = std::mt19937(std::chrono::high_resolution_clock::now().time_since_epoch().count());
    width_ = width;
    height_ = height;
    mines_count_ = cells_with_mines.size();
    opened_cells_ = 0;
    cells_with_mines_ = MakeBoolArray(height, width);
    cells_marked_ = MakeBoolArray(height, width);
    cells_opened_ = MakeBoolArray(height, width);
    for (const auto& cell : cells_with_mines) {
        cells_with_mines_[cell.y][cell.x] = true;
    }
}

void Minesweeper::NewGame(size_t width, size_t height, size_t mines_count) {
    game_status_ = GameStatus::NOT_STARTED;
    DeleteBoolArray(cells_with_mines_, height_);
    DeleteBoolArray(cells_marked_, height_);
    DeleteBoolArray(cells_opened_, height_);
    width_ = width;
    height_ = height;
    mines_count_ = mines_count;
    opened_cells_ = 0;
    cells_with_mines_ = MakeBoolArray(height, width);
    cells_marked_ = MakeBoolArray(height, width);
    cells_opened_ = MakeBoolArray(height, width);
    std::vector<bool> mines(width * height, false);
    for (size_t i = 0; i < mines_count; ++i) {
        mines[i] = true;
    }
    std::shuffle(mines.begin(), mines.end(), random_);
    for (size_t i = 0; i < mines_count; ++i) {
        if (mines[i]) {
            cells_with_mines_[i / width][i % width] = true;
        }
    }
}

void Minesweeper::NewGame(size_t width, size_t height, const std::vector<Cell>& cells_with_mines) {
    game_status_ = GameStatus::NOT_STARTED;
    DeleteBoolArray(cells_with_mines_, height_);
    DeleteBoolArray(cells_marked_, height_);
    DeleteBoolArray(cells_opened_, height_);
    width_ = width;
    height_ = height;
    mines_count_ = cells_with_mines.size();
    opened_cells_ = 0;
    cells_with_mines_ = MakeBoolArray(height, width);
    cells_marked_ = MakeBoolArray(height, width);
    cells_opened_ = MakeBoolArray(height, width);
    for (const auto& cell : cells_with_mines) {
        cells_with_mines_[cell.y][cell.x] = true;
    }
}

bool Minesweeper::ValidNeighbour(int64_t x, int64_t y) const {
    return 0 <= y && y < static_cast<int64_t>(height_) && 0 <= x && x < static_cast<int64_t>(width_);
}

size_t Minesweeper::MineNeighbours(int64_t x, int64_t y) const {
    size_t result = 0;
    for (int64_t dy = -1; dy <= 1; ++dy) {
        for (int64_t dx = -1; dx <= 1; ++dx) {
            if (ValidNeighbour(x + dx, y + dy) && cells_with_mines_[y + dy][x + dx]) {
                ++result;
            }
        }
    }
    return result;
}

void Minesweeper::OpenCell(const Cell& cell) {
    if (game_status_ == GameStatus::DEFEAT || game_status_ == GameStatus::VICTORY || cells_marked_[cell.y][cell.x] ||
        cells_opened_[cell.y][cell.x]) {
        return;
    }
    if (game_status_ == GameStatus::NOT_STARTED) {
        start_of_current_game_ = std::chrono::high_resolution_clock::now();
        game_status_ = GameStatus::IN_PROGRESS;
    }
    if (cells_with_mines_[cell.y][cell.x]) {
        game_status_ = GameStatus::DEFEAT;
        return;
    }
    cells_opened_[cell.y][cell.x] = true;
    ++opened_cells_;
    if (opened_cells_ == height_ * width_ - mines_count_) {
        game_status_ = GameStatus::VICTORY;
        return;
    }
    auto mine_neighbours = MineNeighbours(cell.x, cell.y);
    if (mine_neighbours == 0) {
        for (int64_t dx = -1; dx <= 1; ++dx) {
            for (int64_t dy = -1; dy <= 1; ++dy) {
                int64_t next_x = cell.x + dx, next_y = cell.y + dy;
                if (!ValidNeighbour(next_x, next_y)) {
                    continue;
                }
                if (!cells_marked_[next_y][next_x] && !cells_opened_[next_y][next_x]) {
                    size_t next_x_casted = static_cast<size_t>(next_x);
                    size_t next_y_casted = static_cast<size_t>(next_y);
                    Cell next_cell = {next_x_casted, next_y_casted};
                    OpenCell(next_cell);
                }
            }
        }
    } else {
        return;
    }
}

void Minesweeper::MarkCell(const Cell& cell) {
    if (!cells_opened_[cell.y][cell.x]) {
        cells_marked_[cell.y][cell.x] ^= true;
    }
}

Minesweeper::GameStatus Minesweeper::GetGameStatus() const {
    return game_status_;
}

time_t Minesweeper::GetGameTime() const {
    if (game_status_ == GameStatus::NOT_STARTED) {
        return 0;
    }
    auto current_time = std::chrono::high_resolution_clock::now();
    return (current_time - start_of_current_game_).count();
}

Minesweeper::RenderedField Minesweeper::RenderField() const {
    RenderedField result(height_, std::string(width_, '-'));
    for (size_t i = 0; i < height_; ++i) {
        for (size_t j = 0; j < width_; ++j) {
            if (game_status_ == GameStatus::DEFEAT) {
                if (cells_with_mines_[i][j]) {
                    result[i][j] = '*';
                } else {
                    auto count = MineNeighbours(j + 0ll, i + 0ll);
                    if (count == 0) {
                        result[i][j] = '.';
                    } else {
                        result[i][j] = '0' + count;
                    }
                }
            } else if (game_status_ == GameStatus::VICTORY) {
                if (cells_marked_[i][j]) {
                    result[i][j] = '?';
                } else if (cells_opened_[i][j]) {
                    auto count = MineNeighbours(j + 0ll, i + 0ll);
                    if (count == 0) {
                        result[i][j] = '.';
                    } else {
                        result[i][j] = '0' + count;
                    }
                }
            } else {
                if (cells_marked_[i][j]) {
                    result[i][j] = '?';
                } else if (!cells_opened_[i][j]) {
                    continue;
                } else {
                    auto count = MineNeighbours(j, i);
                    if (count == 0) {
                        result[i][j] = '.';
                    } else {
                        result[i][j] = '0' + count;
                    }
                }
            }
        }
    }
    return result;
}
