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
        Server(int ac, const char * const *av);

        // Inheritance methods
        /// \brief runs the server
        void run() override;
        /// \brief stops the server
        /// \return the code that made the server stop
        int stop() override;
        /// \brief server status
        /// Gets the status of the server
        /// \return the status code of the server
        int getStatus() const {return _status};


        // Server methods
    private:
        std::pair<unsigned int, unsigned int> _bounds;
        unsigned int _limit;
        e_code _status;
    };
}
