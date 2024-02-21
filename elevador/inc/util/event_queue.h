#include <queue>
#include <mutex>
#include <condition_variable>

template<typename T>
class thread_safe_queue {
private:
    std::queue<T> queue;
    mutable std::mutex mutex;
    std::condition_variable cond_var;
public:
    thread_safe_queue() = default;

    thread_safe_queue(const thread_safe_queue& other) = delete;

    thread_safe_queue& operator=(const thread_safe_queue& other) = delete;

    // this will push the event into the queue
    void push(T value) {
        std::lock_guard<std::mutex> lock(mutex);
        queue.push(std::move(value));
        cond_var.notify_one();
    }

    // this will pop the event from the queue, as well as handle the event
    T pop() {
        std::unique_lock<std::mutex> lock(mutex);
        cond_var.wait(lock, [this]{ return !queue.empty(); });
        T value = std::move(queue.front());
        queue.pop();
        return value;
    }

    bool empty() const {
        std::lock_guard<std::mutex> lock(mutex);
        return queue.empty();
    }
};