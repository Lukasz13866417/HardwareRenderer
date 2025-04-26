#include <optional>
#include <tuple>
#include <string>
#include <vector>
#include <algorithm>
#include <cctype>
#include <iostream>

namespace hwr::detail{

struct forloop{
    std::string init,cond,upd;
    forloop(std::string&& i,
            std::string&& c,
            std::string&& u) : init(std::move(i)), 
                               cond(std::move(c)),
                               upd(std::move(u))
    {}
    std::tuple<std::string, std::string, std::string> tie(){
        return {init,cond,upd};
    }
};

// Parses a classic C++ for-loop header (without the surrounding parentheses)
// into its three components: initialization, condition, and update.
// Returns std::nullopt if the header does not split into exactly three parts.
std::optional<forloop>
parseForHeader(const std::string& header);

// Counts the number of variable declarations in the initialization string.
// Returns -1 if the input is invalid, otherwise the count (0 if empty).
int32_t countInitDeclarations(const std::string& init);

// Counts the number of update operations in the update string.
// Returns -1 if the input is invalid, otherwise the count (0 if empty).
int32_t countUpdateOperations(const std::string& upd);

}; // namespace hwr::detail