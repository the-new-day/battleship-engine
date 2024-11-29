#include "Battleship.hpp"

namespace Battleship {

Battleship::Battleship(GameMode mode) : game_mode_(mode) {
    ships_count_[ShipType::kOne] = 0;
    ships_count_[ShipType::kTwo] = 0;
    ships_count_[ShipType::kThree] = 0;
    ships_count_[ShipType::kFour] = 0;
}

bool Battleship::IsPossibleToStart() const {
    if (HasError()) {
        return false;
    }

    return true;
}

bool Battleship::IsLose() const {
    return did_lose_;
}

bool Battleship::IsWin() const {
    return did_win_;
}

void Battleship::SetStrategy(StrategyType strategy) {
    strategy_ = strategy;
    // TODO: maybe need something else
}

bool Battleship::IsFinished() const {
    return is_game_finished_;
}

bool Battleship::SetFieldHeight(uint64_t height) {
    if (field_height_ != 0 || height == 0) {
        return false;
    }

    field_height_ = height;
    return true;
}

bool Battleship::SetFieldWidth(uint64_t width) {
    if (field_width_ != 0 || width == 0) {
        // TODO: set error
        return false;
    }

    field_width_ = width;
    return true;
}

uint64_t Battleship::GetWidth() const {
    return field_width_;
}

uint64_t Battleship::GetHeight() const {
    return field_height_;
}

void Battleship::SetShipsCount(ShipType type, uint64_t amount) {
    // TODO: check if game is already running
    ships_count_[type] = amount;
}

uint64_t Battleship::GetShipsCount(ShipType type) {
    if (!ships_count_.contains(type)) {
        return 0;
    }

    return ships_count_.at(type);
}

} // namespace Battleship
