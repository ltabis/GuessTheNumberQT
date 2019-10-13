//
// Created by tabis on 10/10/2019.
//

#include "Client.hpp"
#include "Exception.hpp"

int main(int ac, char **av)
{

    try {
        GuessGame::Client client(ac, av);

        return client.run();
    } catch (Log::Exception &exception) {
        exception.debugErrorMessage();
    }
    return 0;
}