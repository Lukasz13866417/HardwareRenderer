#ifndef HWR_PROGRAM_HPP
#define HWR_PROGRAM_HPP

#include<type_traits>
#include<string>

namespace hwr{

namespace detail{
    template<typename... Types>
    constexpr bool are_trivially_copyable() {
        return (std::is_trivially_copyable_v<Types> && ...);
    }    
};

template<typename Result, typename... Args>
class Program{
    static_assert(detail::are_trivially_copyable<Args...>(), "All Args must be trivially copyable");
    static_assert(std::is_trivially_copyable_v<Result>, "Result must be trivially copyable");
    public:
        std::string generateCode(){
            return "XD";
        }
};

} // namespace hwr

#endif // HWR_PROGRAM_HPP