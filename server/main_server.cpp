//
// Created by tabis on 10/10/2019.
//

#include "Exception.hpp"

int main()
{
    try {
        std::cout << "Server main." << std::endl;
    } catch (Log::Exception &exception) {
        exception.debugErrorMessage();
    }
    return 0;

}