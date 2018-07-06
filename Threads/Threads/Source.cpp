#include <condition_variable>
#include <mutex>
#include <thread>
#include <iostream>

int main() {
    int i = 0;
    std::mutex m;
    std::condition_variable cond_var_first;
    std::condition_variable cond_var_second;
    std::condition_variable cond_var_third;
    bool notified_second = false;
    bool notified_third = false;
    bool done = false;

    std::thread first([&]() {
        while (!done) {
            if (i == 96)
                done = true;
            std::unique_lock<std::mutex> lock(m);
            std::cout << "First: " << ++i << "\n";
            notified_second = true;
            cond_var_second.notify_one();
            while (notified_second)
                cond_var_first.wait(lock);
        };
        std::cout << "Result: " << ++i << "\n";
    });

    std::thread second([&]() {
        while (!done) {
            std::unique_lock<std::mutex> lock(m);
            while (!notified_second)
                cond_var_second.wait(lock);
            std::cout << "Second: " << ++i << "\n";
            notified_second = false;
            notified_third = true;
            cond_var_third.notify_one();
        }
    });

    std::thread third([&]() {
        while (!done) {
            std::unique_lock<std::mutex> lock(m);
            while (!notified_third)
                cond_var_third.wait(lock);
            std::cout << "Third: " << ++i << "\n";
            notified_third = false;
            cond_var_first.notify_one();
        }
    });

    first.join();
    second.join();
    third.join();

    getchar();
    getchar();
}