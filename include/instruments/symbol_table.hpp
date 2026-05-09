#pragma once

#include <string>
#include <unordered_map>

#include "types.hpp"

namespace instruments {
    class SymbolTable {
    public:
        explicit SymbolTable();
        std::optional<SymbolId> lookup(const std::string& symbol) const;
        SymbolId insert(const std::string& symbol);
        std::size_t size() const noexcept;

    private:
        std::unordered_map<std::string, SymbolId> symbol_to_id_;
    };

    SymbolTable::SymbolTable()
            : symbol_to_id_{} {}

    std::optional<SymbolId> SymbolTable::lookup(const std::string& symbol) const {
        auto it = symbol_to_id_.find(symbol);

        if (it == symbol_to_id_.end()) return {};

        std::optional<SymbolId> res(it->second);
        return res;
    }

    SymbolId SymbolTable::insert(const std::string& symbol) {
        auto it = symbol_to_id_.find(symbol);

        if (it != symbol_to_id_.end()) return it->second;

        SymbolId id = static_cast<SymbolId>(symbol_to_id_.size());
        symbol_to_id_.emplace(symbol, id);
        return id;
    }

    std::size_t SymbolTable::size() const noexcept {
        return symbol_to_id_.size();
    }
} // namespace instruments