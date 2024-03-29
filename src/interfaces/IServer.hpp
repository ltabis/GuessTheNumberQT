//
// Created by tabis on 10/10/2019.
//

#pragma once

#define DEFAULT_BOUNDX   1
#define DEFAULT_BOUNDY   100
#define INFINITE_LIMIT   0
#define DEFAULT_DEBUG    false
#define DEFAULT_PORT     4242

namespace GuessGame {

    /// \brief
    enum e_code {IS_INACTIVE = 0,
                 IS_RUNNING = 1,
                 STOP_SUCCESS = -1,
                 STOP_FAILED = -2,
                 STOP_SIGNAL = -3};

    class IServer {
    public:
        /// \brief virtual destructor
        virtual ~IServer() = default;

        /// \brief runs the server
        virtual int run() = 0;
        virtual int stop() = 0;
        virtual int getStatus() const = 0;
    };
}