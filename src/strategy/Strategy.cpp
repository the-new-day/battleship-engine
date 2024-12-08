#include "Strategy.hpp"
#include "field/Field.hpp"
#include "field/CompressedField.hpp"

namespace Battleship {

Strategy::Strategy(uint64_t field_width, 
                   uint64_t field_height,
                   const std::map<uint8_t, uint64_t>& ship_types,
                   Field* enemy_field)
                   : enemy_ships_count_(ship_types)
                   , field_width_(field_width)
                   , field_height_(field_height)
                   , enemy_field_(enemy_field) {}

FieldPoint Strategy::MakeNextShot() {
    if (!is_game_started_) {
        StartGame();
        enemy_field_->SetBit(last_shot_point_.x, last_shot_point_.y);
        return last_shot_point_;
    }

    FieldPoint point = GetNextShot();
    last_shot_point_ = point;
    enemy_field_->SetBit(last_shot_point_.x, last_shot_point_.y);
    return last_shot_point_;
}

void Strategy::SetLastShotCoords(uint64_t x, uint64_t y) {
    last_shot_point_.x = x;
    last_shot_point_.y = y;
}

void Strategy::SetLastShotResult(ShotResult result) {
    last_shot_result_ = result;
}

bool Strategy::EnemyHasShips() const {
    for (const auto& [ship_size, amount] : enemy_ships_count_) {
        if (amount > 0) {
            return true;
        }
    }

    return false;
}

} // namespace Battleship
