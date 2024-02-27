#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>

std::mutex mtx;
std::condition_variable cv;
bool resourceAvailable = false;

void worker(int id) {
    std::unique_lock<std::mutex> lock(mtx);
    std::cout << "Поток " << id << " ожидает" << std::endl;
    cv.wait(lock, [] { return resourceAvailable; });

    std::cout << "Поток " << id << " получил ресурс" << std::endl;

    //имитация выполнения работы
    std::this_thread::sleep_for(std::chrono::seconds(1));

    std::cout << "Поток " << id << " освободил ресурс" << std::endl;
}

int main() {

    setlocale(LC_ALL, "Russian");

    const int numThreads = 5;
    std::thread threads[numThreads];

    for (int i = 0; i < numThreads; i++)
        threads[i] = std::thread(worker, i);

    //время на запуск потока
    std::this_thread::sleep_for(std::chrono::seconds(2));

    {
        std::lock_guard<std::mutex> lock(mtx);
        std::cout << "Ресурс теперь доступен" << std::endl;
        resourceAvailable = true;
    }

    cv.notify_all();

    for (auto& thread : threads)
        thread.join();

    return 0;
}
