# Hardware renderer from scratch
> Continuation of my PoC project [here](https://github.com/Lukasz13866417/GPU_SDL_Drawing)
This is going to be a cleaner, powerful API, **ISO C++ compliant** (compilable with most pedantic flags). There will be a very **flexible, yet simple and intuitive** rendering pipeline.
## Features builtin custom Shading Language that compiles to Open Computing Language.
Example:
```C++
//  shader. Is compiled to OpenCL code.
    hwr::Program sum{[](){
        Float fl = 1;
        HWR_FOR(Int a = 0; a < 10; ++a) {
            HWR_FOR(Int b = 0; b < 10; ++b) {
                HWR_IF(a+b==0){
                    Double xd = -1.0f + fl;
                }
            }
            Float fl2 = 1;
            fl -= a;
            ++a;
        }
    }};
    // This will call the lambda passed to the constructor.
    // When the lambda is called, the code inside it is run, 
    // and my custom types (Float etc.) generate the OpenCL code automatically,
    // thanks to operator overloads (+, *, -, / etc).
    std::string res = sum.compile();
    // print the compiled OpenCL code.
    std::cout<<res<<std::endl;
```
The language has been made with **state-of-the-art C++ template metaprogramming**. Yes, the language is "inside" C++. Types like ```Int``` are custom types that automatically generate OpenCL kernel code.That means you will get **compile-time** errors (contrary to GLSL). And a way cleaner view of your code. This will be **very easily extensible to do all the 3D math for you**, but you will still have fine-grained control over everything, if you want.