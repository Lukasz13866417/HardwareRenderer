#ifndef HWR_PROGRAM_CONTEXT_HPP
#define HWR_PROGRAM_CONTEXT_HPP

#include <stack>
#include <string>

namespace hwr {
    class Program;
}

namespace hwr::detail::program_context {
    
    // Flag to control code generation in ShaderValue constructor
    extern bool should_append_code;

    void push_program(Program& p);
    void pop_program();
    Program& current_program();

    void decr_counter();
    void incr_counter();
    int32_t get_counter();

    void push_opencl_type(const std::string& opencl_type);
    std::string pop_opencl_type();
    void appendToProgramCode(const std::string& line);

    void push_rvalue(const std::string& what);
    std::string pop_rvalue();

    void set_forloop_header_generation();
    void unset_forloop_header_generation();
    bool is_forloop_header_being_generated();

    void replace_possible_comma_with_semicolon();
    void remove_last_char();

    bool is_first_def();
    void set_first_def();
    void unset_first_def();

    void ignore_next_k_appends(int32_t k);
    void undo_last_k_appends(int32_t k);

    void rollback_name_counter(int32_t k);

    std::string make_temp_name();

} // namespace hwr::detail::program_context

#endif // HWR_PROGRAM_CONTEXT_HPP 