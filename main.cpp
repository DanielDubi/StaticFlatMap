#include "iostream"
#include "StaticFlatMap.hpp"

void main()
{
    StaticFlatMap<int, int, 100> testMap;
    testMap[1] = 2;
    testMap[2] = 10;
    testMap[3] = 5;
    for (auto num : testMap)
    {
        std::cout << num << " ";
    }
    std::cout << std::endl;
}
