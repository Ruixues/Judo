#include <string>
#include <vector>
#include <thread>
#include <mutex>
#include <iostream>
#include <chrono>
#include <stdlib.h>

std::mutex now_running_thread_lock;
const size_t max_thread = 10;
size_t now_running_thread = 0;    //现在正在运行的线程数量
void testFile(std::string f) {
    //开始进行测试
    _popen()
    now_running_thread_lock.lock();
    --now_running_thread;
    now_running_thread_lock.unlock();
}

int main() {
    std::vector<std::string> files = {"fab"};
    std::vector<std::thread> runs;
    for (auto f:files) {
        now_running_thread_lock.lock();
        if (now_running_thread < max_thread) {  //还可以运行
            ++now_running_thread;
            now_running_thread_lock.unlock();
            runs.push_back(std::thread(testFile, f));
        }
    }
    //开始等待
    while (true) {
        if (now_running_thread != 0) {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            continue;
        } else {
            break;
        }
    }
    return 0;
}