# Hardware renderer from scratch
> Continuation of my PoC project [here](https://github.com/Lukasz13866417/GPU_SDL_Drawing)
This is going to be a cleaner, powerful API, **ISO C++ compliant** (compilable with most pedantic flags). There will be a very **flexible, yet simple and intuitive** rendering pipeline.
## Features builtin custom Shading Language that compiles to Open Computing Language.
Example:
```C++
//  shader. Is compiled to OpenCL code.
    hwr::Program sum{[](){
        Float a = 1.0f;
        Float b = 3.0f;
        
        Int x = 1;
        Int y = 3;

        Double p = 1.123;
        Double q = 3.321;

        Double A = a*x*q + b*y*p + x+y+a+b + p/q;

        Bool xd = 3*x == y;
    }};
    // This will call the lambda passed to the constructor.
    // When the lambda is called, the code inside it is run, 
    // and my custom types (Float etc.) generate the OpenCL code automatically,
    // thanks to operator overloads (+, *, -, /).
    std::string res = sum.compile();
    // print the compiled OpenCL code.
    std::cout<<res<<std::endl;
```
The language has been made with **state-of-the-art C++ template metaprogramming**. Yes, the language is "inside" C++. Types like ```Int``` are custom types that automatically generate OpenCL kernel code.That means you will get **compile-time** errors (contrary to GLSL). And a way cleaner view of your code. This will be **very easily extensible to do all the 3D math for you**, but you will still have fine-grained control over everything, if you want.