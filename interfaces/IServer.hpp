//
// Created by tabis on 10/10/2019.
//

#pragma once

namespace GuessGame {
    class IServer {
    public:
        /// \brief virtual destructor
        virtual ~IServer() = default;

        /// \brief runs the server
        virtual void run() = 0;
    };
}