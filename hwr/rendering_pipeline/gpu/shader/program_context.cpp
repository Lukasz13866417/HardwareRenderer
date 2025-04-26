#include <hwr/log.hpp>
#include "program_context.hpp"
#include "shader.hpp"  // Program declaration
#include <cassert>
#include <vector>

namespace hwr::detail::program_context {

    namespace {

        int32_t counter=0;
        std::vector<Program*> s_program_stack;
        std::vector<std::string> s_type_stack;
        std::vector<std::string> s_rvalue_expr_stack;

    }
        
    void push_program(Program& p) {
        s_program_stack.push_back(&p);
    }

    void pop_program() {
        if (!s_program_stack.empty()) {
            s_program_stack.pop_back();
        }
    }

    Program& current_program() {
        HWR_ASSERT(!s_program_stack.empty(),
                   "Stack empty");
        return *s_program_stack.back();
    }

    void decr_counter(){
        --counter;
    }

    void incr_counter(){
        ++counter;
    }

    int32_t get_counter(){
        return counter;
    }

    void push_opencl_type(const std::string& opencl_type) {
        s_type_stack.push_back(opencl_type);
    }

    std::string pop_opencl_type() {
        if (s_type_stack.empty()) {
            return "ERROR_NO_TYPE_AVAILABLE";
        }
        std::string type = s_type_stack.back();
        s_type_stack.pop_back();
        return type;
    }

    void appendToProgramCode(const std::string& line) {
        current_program().append(line);
    }

    void push_rvalue(const std::string& what){
        s_rvalue_expr_stack.push_back(what);
    }
    std::string pop_rvalue(){
        if(s_program_stack.empty()){
            HWR_FATAL("Empty stack");
        }
        std::string res = s_rvalue_expr_stack.back();
        s_rvalue_expr_stack.pop_back();
        return res;
    }

    static bool _is_forloop_header_being_generated = false;

    void set_forloop_header_generation(){
        _is_forloop_header_being_generated = true;
    }

    void unset_forloop_header_generation(){
        _is_forloop_header_being_generated = false;
    }

    bool is_forloop_header_being_generated(){
        return _is_forloop_header_being_generated;
    }

    void replace_possible_comma_with_semicolon(){
        if(s_program_stack.empty()){
            HWR_FATAL("Empty program stack");
        }else{
            s_program_stack.back()->replace_possible_comma_with_semicolon();
        }
    }

    void remove_last_char(){
        if(s_program_stack.empty()){
            HWR_FATAL("Empty program stack");
        }else{
            s_program_stack.back()->remove_last_char();
        }
    }

    namespace {
        bool _is_first_def = false;
    }

    bool is_first_def(){
        return _is_first_def;
    }
    void set_first_def(){
        _is_first_def = true;
    }
    void unset_first_def(){
        _is_first_def = false;
    }

    void ignore_next_k_appends(int32_t k){
        if(s_program_stack.empty()){
            HWR_FATAL("Empty program stack");
        }else{
            s_program_stack.back()->ignore_next_k_appends(k);
        }
    }

    void undo_last_k_appends(int32_t k){
        if(s_program_stack.empty()){
            HWR_FATAL("Empty program stack");
        }else{
            s_program_stack.back()->undo_last_k_appends(k);
        }
    }

    namespace{
        int32_t temp_counter = 0;
    }

    std::string make_temp_name() {
        return "tmp" + std::to_string(temp_counter++);
    }

    void rollback_name_counter(int32_t k){
        HWR_ASSERT(temp_counter >= k,
                   "temp_counter smaller than k makes no sense");
        temp_counter -= k;
    }

} // namespace hwr::detail::program_context 