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
        /// \param bound_x minimum bound value
        /// \param bound_y maximum bound value
        /// \param limit limit of turn for the player
        Server(unsigned int bound_x, unsigned int bound_y, unsigned int limit);
        Server(const std::pair<unsigned int, unsigned int> &bounds, unsigned int limit);

        // Inheritance methods
        /// \brief runs the server
        void run() override;
        /// \brief stops the server
        /// \param code stop code
        /// \return the code that made the server stop
        int stop(e_code code) const override {return code;};

        // Server methods
    private:
        std::pair<unsigned int, unsigned int> _bounds;
        unsigned int _limit;
    };
}
