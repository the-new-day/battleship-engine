#include "Strategy.hpp"

namespace Battleship {

// Max size of a field that can be stored in a simple binary matrix (30 MB)
const uint64_t kMaxMatrixFieldArea = 251'658'240;

Strategy::Strategy(uint64_t field_width, 
                   uint64_t field_height, 
                   const std::map<ShipType, uint64_t>& ship_types,
                   Field* field,
                   Field* enemy_field)
    : field_width_(field_width)
    , field_height_(field_height)
    , ship_types_(ship_types)
    , field_(field)
    , enemy_field_(enemy_field) {
    // if (static_cast<double>(field_width) / kMaxMatrixFieldArea * field_height < 1) {
    //     field_ = new MartixField(field_width, field_height);
    //     return;
    // }

    // // TODO: choose optimal field
    // field_ = new CompressedField(field_width, field_height);

    // double density = (ship_types.at(ShipType::kOne) / field_width)
    //                + (ship_types.at(ShipType::kTwo) / field_width)
    //                + (ship_types.at(ShipType::kThree) / field_width)
    //                + (ship_types.at(ShipType::kFour) / field_width);

    // density /= field_height;

    // if (density < 0.05) {
        
    // }
}

ShotResult Strategy::RecieveShot(uint64_t x, uint64_t y) {
    if (!field_->HasShip(x, y)) {
        return ShotResult::kMiss;
    }

    ShotResult result = ShotResult::kHit;
    if ((x == field_width_ - 1 || !field_->HasShip(x + 1, y))
    && (y == field_height_ - 1 || !field_->HasShip(x, y + 1))
    && (x == 0 || !field_->HasShip(x - 1, y))
    && (y == 0 || !field_->HasShip(x, y - 1))) {
        DecreaseShipsAmount();
        result = ShotResult::kKill;
    }

    field_->RemoveShip(x, y);
    return result;
}

bool Strategy::HasAliveShips() const {
    for (size_t i = 0; i < kShipTypesAmount; ++i) {
        if (ships_count_[0] > 0) {
            return true;
        }
    }

    return false;
}

void Strategy::DecreaseShipsAmount() {
    for (size_t i = 0; i < kShipTypesAmount; ++i) {
        if (ships_count_[0] > 0) {
            --ships_count_[0];
            return;
        }
    }
}

void Strategy::SetLastShotResult(ShotResult result) {
    last_shot_result_ = result;
}

} // namespace Battleship
