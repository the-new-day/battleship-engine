#include "ShipHandler.hpp"

#include "field/MatrixField.hpp"
#include "field/CompressedDenseField.hpp"
#include "field/CompressedField.hpp"
#include "field/MappedField.hpp"

#include <algorithm>
#include <random>
#include <fstream>

namespace Battleship {

ShipHandler::ShipHandler(uint64_t field_width, uint64_t field_height, const std::map<uint8_t, uint64_t>& ships_count) 
    : field_width_(field_width)
    , field_height_(field_height)
    , ships_count_(ships_count) {
    SetField();
}

bool ShipHandler::LoadFromFile(const std::string& filename) {
    std::ifstream file(filename);
    
    if (!file.good()) {
        return false;
    }

    uint64_t width;
    uint64_t height;

    if (!(file >> width >> height)) {
        return false;
    }

    field_width_ = width;
    field_height_ = height;

    delete field_;
    SetField();
    
    for (auto& p : ships_count_) {
        p.second = 0;
    }

    uint16_t ship_size;
    char direction;
    uint64_t x;
    uint64_t y;

    while (file >> ship_size >> direction >> x >> y) {
        bool is_horizontal;

        if (direction == 'v') {
            is_horizontal = false;
        } else if (direction == 'h') {
            is_horizontal = true;
        } else {
            return false;
        }

        if (!IsPossibleToPlaceShip(x, y, ship_size, is_horizontal)) {
            return false;
        }

        PlaceShip(x, y, ship_size, is_horizontal);
        ++ships_count_[ship_size];
    }

    return file.eof();
}

bool ShipHandler::Dump(const std::string& filename) const {
    std::ofstream file(filename);

    if (!file.good()) {
        return false;
    }

    file << field_width_ << ' ' << field_height_ << '\n';
    DumpHorizontalShips(file);
    DumpVerticalShips(file);

    return true;
}

void ShipHandler::DumpHorizontalShips(std::ofstream& file) const {
    for (uint64_t y = 0; y < field_height_; ++y) {
        for (uint64_t x = 0; x < field_width_; ++x) {
            std::vector<FieldPoint> cells;
            FindShipCells(x, y, cells);

            if (cells.empty()) {
                continue;
            }

            if (cells.size() == 1) {
                file << "1 h " << x << ' ' << y << '\n';
                continue;
            }

            if (cells[1].y != y) {
                continue;
            }

            file << cells.size() << " h " << x << ' ' << y << '\n';
            x += cells.size();
        }
    }
}

void ShipHandler::DumpVerticalShips(std::ofstream& file) const {
    for (uint64_t x = 0; x < field_width_; ++x) {
        for (uint64_t y = 0; y < field_height_; ++y) {
            std::vector<FieldPoint> cells;
            FindShipCells(x, y, cells);

            if (cells.empty() || cells.size() == 1) {
                continue;
            }

            if (cells[1].x != x) {
                continue;
            }

            file << cells.size() << " v " << x << ' ' << y << '\n';
            y += cells.size();
        }
    }
}

ShotResult ShipHandler::ProcessShot(uint64_t x, uint64_t y) {
    if (!field_->IsOneAt(x, y)) {
        return ShotResult::kMiss;
    }

    ShotResult result = ShotResult::kHit;

    std::vector<FieldPoint> ship_cells;
    ship_cells.reserve(kMaxShipLength);

    FindShipCells(x, y, ship_cells);
    
    if (IsHitFatal(x, y, ship_cells)) {
        for (const FieldPoint& cell : ship_cells) {
            hit_points_.erase(cell);
        }

        --ships_count_[ship_cells.size()];
        result = ShotResult::kKill;
    } else {
        hit_points_.insert({x, y});
    }

    field_->RemoveBit(x, y);
    return result;
}

bool ShipHandler::IsHitFatal(uint64_t x, uint64_t y, const std::vector<FieldPoint>& ship_cells) const {
    if (ship_cells.empty()) {
        return false;
    }

    for (const FieldPoint& cell : ship_cells) {
        if ((cell.x != x || cell.y != y) && field_->IsOneAt(cell.x, cell.y)) {
            return false;
        }
    }

    return true;
}

bool ShipHandler::IsShipAt(uint64_t x, uint64_t y) const {
    return hit_points_.contains({x, y}) || field_->IsOneAt(x, y);
}

void ShipHandler::FindShipCells(uint64_t x, uint64_t y, std::vector<FieldPoint>& cells) const {
    if (!IsShipAt(x, y) || std::count(cells.begin(), cells.end(), FieldPoint{x, y}) != 0) {
        return;
    }

    cells.emplace_back(x, y);

    if (x < field_width_ - 1) FindShipCells(x + 1, y, cells);
    if (y < field_height_ - 1) FindShipCells(x, y + 1, cells);
    if (x > 0) FindShipCells(x - 1, y, cells);
    if (y > 0) FindShipCells(x, y - 1, cells);
}

bool ShipHandler::PlaceShips() {
    return PlaceShipsRandomly() || PlaceShipsLinear();
}

bool ShipHandler::PlaceShipsLinear() {
    bool is_horizontal = false;

    for (uint8_t ship_size = kMaxShipLength; ship_size > 0; --ship_size) {
        uint64_t amount = ships_count_[ship_size];

        while (amount > 0) {
            bool is_placed = false;

            for (uint64_t i = 0; i < field_width_ && !is_placed; ++i) {
                for (uint64_t j = 0; j < field_height_ && !is_placed; ++j) {
                    if (IsPossibleToPlaceShip(i, j, ship_size, is_horizontal)) {
                        PlaceShip(i, j, ship_size, is_horizontal);
                        is_placed = true;
                    }
                }
            }

            if (!is_placed) {
                return false;
            }

            --amount;
            is_horizontal = !is_horizontal;
        }
    }

    return true;
}

bool ShipHandler::PlaceShipsRandomly() {
    std::random_device rd;
    
    std::mt19937 mt(rd());
    std::uniform_int_distribution<> dist_x(0, field_width_ - 1);
    std::uniform_int_distribution<> dist_y(0, field_height_ - 1);
    std::uniform_int_distribution<> dist_orientation(0, 1);

    for (uint8_t ship_size = kMaxShipLength; ship_size > 0; --ship_size) {
        uint64_t amount = ships_count_[ship_size];

        while (amount > 0) {
            uint64_t x;
            uint64_t y;
            bool is_horizontal;

            bool is_placed = false;

            for (uint64_t attempt = 0; 
            !is_placed && static_cast<double>(attempt) / field_width_ < field_height_;
            ++attempt) {
                x = dist_x(mt);
                y = dist_y(mt);
                is_horizontal = (dist_orientation(mt) == 0);

                if (IsPossibleToPlaceShip(x, y, ship_size, is_horizontal)) {
                    PlaceShip(x, y, ship_size, is_horizontal);
                    is_placed = true;
                }
            }

            if (!is_placed) {
                return false;
            }

            --amount;
        }
    }

    return true;
}

void ShipHandler::SetField() {
    if (static_cast<double>(field_width_) / kMaxMatrixFieldArea * field_height_ <= 1) {
        field_ = new MartixField(field_width_, field_height_);
        return;
    }

    double density = 0;

    for (uint8_t i = 1; i <= kMaxShipLength; ++i) {
        density += static_cast<double>(ships_count_[i]) / field_width_;
    }

    density /= field_height_;

    if (density < 0.1) {
        field_ = new MappedField(field_width_, field_height_);
    } else if (density < 0.75) {
        field_ = new CompressedField(field_width_, field_height_);
    } else {
        field_ = new CompressedDenseField(field_width_, field_height_);
    }
}

void ShipHandler::PlaceShip(uint64_t x, uint64_t y, uint8_t ship_size, bool is_horizontal) {
    for (uint8_t i = 0; i < ship_size; ++i) {
        uint64_t next_x = x + (is_horizontal ? i : 0);
        uint64_t next_y = y + (is_horizontal ? 0 : i);

        field_->SetBit(next_x, next_y);
    }
}

bool ShipHandler::IsPossibleToPlaceShip(uint64_t x, uint64_t y, uint8_t ship_size, bool is_horizontal) const {
    if (is_horizontal && x + ship_size > field_width_
    || !is_horizontal && y + ship_size > field_height_) {
        return false;
    }

    if (is_horizontal) {
        for (uint64_t i = (x == 0 ? 0 : x - 1); i <= x + ship_size; ++i) {
            if (field_->IsOneAt(i, y + 1) || field_->IsOneAt(i, y) || field_->IsOneAt(i, y - 1)) {
                return false;
            }
        }

        return true;
    }

    for (uint64_t i = (y == 0 ? 0 : y - 1); i <= y + ship_size; ++i) {
        if (field_->IsOneAt(x + 1, i) || field_->IsOneAt(x, i) || field_->IsOneAt(x - 1, i)) {
            return false;
        }
    }

    return true;
}

bool ShipHandler::HasAliveShips() const {
    for (const auto& p : ships_count_) {
        if (p.second != 0) {
            return true;
        }
    }

    return false;
}

uint64_t ShipHandler::GetFieldWidth() const {
    return field_width_;
}

uint64_t ShipHandler::GetFieldHeight() const {
    return field_height_;
}

uint64_t ShipHandler::GetShipsCount(uint8_t ship_size) const {
    return ships_count_.at(ship_size);
}

} // namespace Battleship
