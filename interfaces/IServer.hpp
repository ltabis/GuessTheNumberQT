//
// Created by tabis on 10/10/2019.
//

#pragma once

namespace GuessGame {

    /// \brief
    enum e_code {INACTIVE = 0,
                 IS_RUNNING = 1,
                 STOP_SUCCESS = -1,
                 STOP_FAILED = -2,
                 STOP_SIGNAL = -3};

    class IServer {
    public:
        /// \brief virtual destructor
        virtual ~IServer() = default;

        /// \brief runs the server
        virtual void run() = 0;
        virtual int stop() = 0;
        virtual int getStatus() const = 0;
    };
}