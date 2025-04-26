# Hardware renderer from scratch
> Continuation of my PoC project [here](https://github.com/Lukasz13866417/GPU_SDL_Drawing)
This is going to be a cleaner, powerful API, **ISO C++ compliant** (compilable with most pedantic flags). There will be a very **flexible, yet simple and intuitive** rendering pipeline.
## Features builtin custom Shading Language that compiles to Open Computing Language.
Example:
```C++
//  shader. Is compiled to OpenCL code.
    hwr::Program shader{[](){
        // Variables with all basic types
        Float a = 1.0f;
        Int x = 5;
        Double p = 3.14159;
        Bool isEven = false;

        a *= 2.0f;
        x %= 3;       
        
        HWR_IF(x == 2) {
            a += p;
            isEven = true;
        } HWR_ELSE_IF(x == 1) {
            a -= p;
            isEven = false;
        } HWR_ELSE {
            a = 0.0f;
        }
        
        Int sum = 0;
        HWR_FOR(Int i = 0, i < 10, ++i) {
            sum += i;
            Int mask = 1 << 2;
            HWR_IF(i & mask) {
                sum *= 2;
            }
        }
    }};
    // This will call the lambda passed to the constructor.
    // The language is "inside" C++ - the code inside the lambda is analyzed
    // and translated to OpenCL at runtime, with full compile-time type checking.
    std::string code = shader.compile();
    std::cout << code << std::endl;
```
The language has been made with **state-of-the-art C++ template metaprogramming**. Yes, the language is "inside" C++. Types like ```Int``` are custom types that automatically generate OpenCL kernel code.That means you will get **compile-time** errors (contrary to GLSL). And a way cleaner view of your code. This will be **very easily extensible to do all the 3D math for you**, but you will still have fine-grained control over everything, if you want.