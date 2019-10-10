//
// Created by tabis on 10/10/2019.
//

#pragma once

namespace GuessGame {

    /// \brief
    enum e_code {SUCCESS, FAILED, SIGNAL};

    class IServer {
    public:
        /// \brief virtual destructor
        virtual ~IServer() = default;

        /// \brief runs the server
        virtual void run() = 0;
        virtual int stop(e_code code) const = 0;
    };
}