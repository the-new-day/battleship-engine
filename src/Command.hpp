#pragma once

#include "utils.hpp"

#include <string>
#include <cstddef>
#include <optional>
#include <vector>
#include <cstdint>

namespace Battleship {

class Command {
public:
    explicit Command(const std::string& name) : name_(name) {};

    std::string GetResponse() const;
    void SetResponse(const std::string& responce);
    void SetValue(const std::string& command);

    std::optional<std::string> GetPart(size_t index = 0);

    template<typename T>
    std::optional<T> GetValue(size_t index = 0);

private:
    std::vector<std::string> command_parts_;
    std::string name_;
    std::string response_;
};

} // namespace Battleship
