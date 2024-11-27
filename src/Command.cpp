#include "Command.hpp"

#include <sstream>

namespace Battleship {

std::string Command::GetResponse() const {
    return response_;
}

void Command::SetResponse(const std::string& responce) {
    response_ = responce;
}

void Command::SetValue(const std::string& command){
    std::istringstream iss(command);
    
    command_parts_.clear();
    command_parts_.reserve(std::count(command.begin(), command.end(), ' '));

    std::string part;
    while (std::getline(iss, part, ' ')) {
        command_parts_.push_back(part);
    }
}

std::optional<std::string> Command::GetPart(size_t index) {
    if (index >= command_parts_.size()) {
        return std::nullopt;
    }

    return command_parts_.at(index);
}

template <>
std::optional<uint64_t> Command::GetValue(size_t index) {
    if (index >= command_parts_.size()) {
        return std::nullopt;
    }

    auto parsing_result = ParseNumber<uint64_t>(*GetPart(index));
    if (!parsing_result.has_value()) {
        return std::nullopt;
    }

    return parsing_result.value();
}

template <>
std::optional<std::string> Command::GetValue(size_t index) {
    return GetPart(index);
}

} // namespace Battleship
