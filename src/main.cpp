#include <iostream>
#include <string>
#include <vector>

// Sample function to demonstrate functionality
std::string greet(const std::string &name)
{
    return "Hello, " + name + "!";
}

int main(int argc, char *argv[])
{
    std::cout << "=== C++ Cross-Platform Project ===" << std::endl;

    // Parse command line arguments
    std::vector<std::string> args(argv + 1, argv + argc);

    if (args.empty())
    {
        std::cout << greet("World") << std::endl;
    }
    else
    {
        for (const auto &arg : args)
        {
            std::cout << greet(arg) << std::endl;
        }
    }

#ifdef _WIN32
    std::cout << "Platform: Windows" << std::endl;
#elif __linux__
    std::cout << "Platform: Linux" << std::endl;
#elif __APPLE__
    std::cout << "Platform: macOS" << std::endl;
#else
    std::cout << "Platform: Unknown" << std::endl;
#endif

#ifdef NDEBUG
    std::cout << "Build: Release" << std::endl;
#else
    std::cout << "Build: Debug" << std::endl;
#endif

    return 0;
}
