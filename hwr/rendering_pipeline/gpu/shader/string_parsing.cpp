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


std::vector<std::string> extractVariableNames(const std::string& code)
{
    std::vector<std::string> names;

    // --- helper lambdas ----------------------------------------------------
    auto stripInitializer = [](std::string s) -> std::string {
        auto pos = s.find_first_of("=[{(");
        if (pos != std::string::npos) s.erase(pos);
        // remove any trailing array-extent:  var[10]
        pos = s.find('[');
        if (pos != std::string::npos) s.erase(pos);
        return trim(s);
    };

    auto harvestFromDecl = [&](std::string decl) {
        decl = trim(decl);
        if (decl.empty()) return;

        // Skip leading attributes such as [[nodiscard]] or alignas(...)
        std::size_t start = 0, depth = 0;
        for (; start < decl.size(); ++start) {
            char ch = decl[start];
            if (ch == '[' || ch == '(' || ch == '{' || ch == '<') ++depth;
            else if (ch == ']' || ch == ')' || ch == '}' || ch == '>') --depth;
            else if (std::isspace(static_cast<unsigned char>(ch)) && depth == 0) break;
        }
        if (start == decl.size()) return;          // malformed: no whitespace

        // After the type part, split the remainder on top-level commas.
        std::string afterType = decl.substr(start);
        std::string current;
        depth = 0;
        for (char ch : afterType) {
            if (ch == '(' || ch == '{' || ch == '[' || ch == '<') ++depth;
            else if (ch == ')' || ch == '}' || ch == ']' || ch == '>') --depth;

            if (ch == ',' && depth == 0) {
                auto id = stripInitializer(current);
                if (!id.empty()) names.push_back(id);
                current.clear();
            } else {
                current += ch;
            }
        }
        auto id = stripInitializer(current);
        if (!id.empty()) names.push_back(id);
    };

    // --- split the whole string on top-level semicolons --------------------
    std::string current;
    int depth = 0;
    for (char ch : code) {
        if (ch == '(' || ch == '{' || ch == '[' || ch == '<') ++depth;
        else if (ch == ')' || ch == '}' || ch == ']' || ch == '>') --depth;

        if (ch == ';' && depth == 0) {
            harvestFromDecl(current);
            current.clear();
        } else {
            current += ch;
        }
    }
    harvestFromDecl(current);        // last declaration (no trailing ';')

    return names;
}

} // namespace hwr::detail