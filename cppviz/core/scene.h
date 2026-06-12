#pragma once
#include <vector>
#include "Coordinate.h"

namespace cppviz {

class Scene {
private:
    std::vector<Coordinate>       coordinates_;
    std::vector<SignalDataResult> signals_;

public:
    void add(const Coordinate& obj) {
        coordinates_.push_back(obj);
    }

    void add(const SignalDataResult& obj) {
        signals_.push_back(obj);
    }

    void clear() {
        coordinates_.clear();
        signals_.clear();
    }

    const auto& coordinates() const { return coordinates_; }
    const auto& signals() const { return signals_; }
};

} // namespace cppviz