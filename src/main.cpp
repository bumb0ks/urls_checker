
#include "checkers_manager/chekers_manager.h"
#include "control_socket/control_socket.h"
#include "traits.h"

#include <iostream>
#include <chrono>
#include <utility>

using namespace std;

// void start()
// {
//     cout << "START IT NOW!";
// }

// void stop()
// {
//     cout << "STOOOooooOOOoooOP IT NOW!";
// }

void run()
{
    checkers_manager manager;

    manager.start();

    // control_socket cs(start, stop);

    // cs.run();
}

int main()
{
    chrono::time_point now = chrono::steady_clock::now();
    
    run();

    auto t = chrono::duration_cast<chrono::milliseconds>(chrono::steady_clock::now() - now).count();
    cout << "Time spent: " << t / 1000 << "." << t % 1000 << "ms";

    return 0;
}