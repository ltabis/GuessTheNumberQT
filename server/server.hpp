//
// Created by tabis on 10/10/2019.
//

#pragma once

#include <utility>

#include "IServer.hpp"

namespace GuessGame {
    class Server : public IServer {
    public:
        /// \brief constructor
        /// \arg bound_x minimum value
        Server(unsigned int bound_x, unsigned int bound_y, unsigned int limit);
        Server(const std::pair<unsigned int, unsigned int> &bounds, unsigned int limit);

        // Inheritance methods
        void run() override;

        // Server methods
    private:
        std::pair<unsigned int, unsigned int> _bounds;
        unsigned int _limit;
    };
}
