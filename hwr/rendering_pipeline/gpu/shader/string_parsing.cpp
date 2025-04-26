#include <optional>
#include <tuple>
#include <string>
#include <vector>
#include <algorithm>
#include <cctype>
#include <iostream>

#include "./string_parsing.hpp"

namespace hwr::detail {

// Utility to trim whitespace from both ends of a string
static inline std::string trim(const std::string& s) {
    auto first = s.find_first_not_of(" \t\n\r");
    if (first == std::string::npos) return "";
    auto last = s.find_last_not_of(" \t\n\r");
    return s.substr(first, last - first + 1);
}

// Parses a classic C++ for-loop header (without the surrounding parentheses)
// into its three components: initialization, condition, and update.
// Returns std::nullopt if the header does not split into exactly three parts.
std::optional<forloop> parseForHeader(const std::string& header) {
    std::vector<std::string> parts;
    std::string current;
    int depth = 0;

    for (char ch : header) {
        // Track nested scopes to ignore semicolons inside them
        if (ch == '(' || ch == '{' || ch == '[') {
            ++depth;
        } else if (ch == ')' || ch == '}' || ch == ']') {
            if (depth > 0) --depth;
        }

        // Split on top-level semicolons
        if (ch == ';' && depth == 0) {
            parts.push_back(current);
            current.clear();
        } else {
            current += ch;
        }
    }
    parts.push_back(current);

    if (parts.size() != 3) {
        return std::nullopt;
    }

    auto init = trim(parts[0]);
    auto cond = trim(parts[1]);
    auto upd  = trim(parts[2]);

    return forloop(std::move(init), std::move(cond), std::move(upd));
}


// Counts the number of variable declarations in the initialization string.
// Returns -1 if the input is invalid, otherwise the count (0 if empty).
int32_t countInitDeclarations(const std::string& init) {
    if (init.empty()) return 0;

    std::vector<std::string> decls;
    std::string current;
    int32_t depth = 0;

    for (char ch : init) {
        // Track nested parentheses/brackets to avoid splitting inside them
        if (ch == '(' || ch == '{' || ch == '[') {
            ++depth;
        } else if (ch == ')' || ch == '}' || ch == ']') {
            if (depth > 0) --depth;
        }

        // Split on top-level commas
        if (ch == ',' && depth == 0) {
            decls.push_back(current);
            current.clear();
        } else {
            current += ch;
        }
    }
    decls.push_back(current);

    int32_t count = 0;
    for (auto& d : decls) {
        auto t = trim(d);
        if (t.empty()) return -1;
        ++count;
    }
    return count;
}

// Counts the number of update operations in the update string.
// Returns -1 if the input is invalid, otherwise the count (0 if empty).
int32_t countUpdateOperations(const std::string& upd) {
    if (upd.empty()) return 0;

    std::vector<std::string> ops;
    std::string current;
    int32_t depth = 0;

    for (char ch : upd) {
        // Track nested parentheses/brackets to avoid splitting inside them
        if (ch == '(' || ch == '{' || ch == '[') {
            ++depth;
        } else if (ch == ')' || ch == '}' || ch == ']') {
            if (depth > 0) --depth;
        }

        // Split on top-level commas
        if (ch == ',' && depth == 0) {
            ops.push_back(current);
            current.clear();
        } else {
            current += ch;
        }
    }
    ops.push_back(current);

    int32_t count = 0;
    for (auto& o : ops) {
        auto t = trim(o);
        if (t.empty()) return -1;
        ++count;
    }
    return count;
}

} // namespace hwr::detail