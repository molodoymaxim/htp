#include <iostream>
#include <chrono>
#include <random>
#include <vector>
#include <numeric>
#include <thread>

uint64_t array[1000000];

void Generate() {
    std::mt19937_64 generator(11);
    std::uniform_int_distribution<uint64_t> distr(0u, std::numeric_limits<uint64_t>::max());
    for (int i = 0; i < 1000000; ++i) {
        array[i] = distr(generator);
    }
}

uint64_t invertBits(unsigned char* data, size_t size, unsigned char mask) {
    for (int i = 0; i < size; i++) {
        data[i] ^= mask;
    }
    return *reinterpret_cast<uint64_t*>(data);
}

void InvertBits(uint64_t* begin, uint64_t* end, uint64_t(*func)(unsigned char*, size_t, unsigned char)) {
    for (auto it = begin; it != end; ++it) {
        unsigned char* data_ptr = reinterpret_cast<unsigned char*>(it);
        *it = func(data_ptr, sizeof(uint64_t), 0xFF); // тут используем нашу маску
    }
}

template <class Function>
uint64_t ParallelInvertBits(uint64_t* begin, uint64_t* end, Function func) {
    if (begin == end) {
        return 0;
    }
    const size_t size = end - begin;
    const size_t min_job_size = 1000; 
    const size_t hardware_threads = std::thread::hardware_concurrency();
    const size_t max_threads = (hardware_threads == 0 ? 2 : hardware_threads);
    const size_t num_threads = std::min(max_threads, (size + min_job_size - 1) / min_job_size);
    std::vector<std::thread> threads;
    threads.reserve(num_threads);
    auto first = begin;
    const auto block_size = size / num_threads;
    for (size_t i = 0; i < num_threads; ++i) {
        const auto last = (i == num_threads - 1) ? end : (first + block_size);
        threads.emplace_back(InvertBits, first, last, func);
        first = last;
    }

    for (auto& t : threads) {
        t.join();
    }

    return std::accumulate(begin, end, uint64_t{ 0 });
}

int main() {
    Generate();

    std::cout << "Before inversion:" << std::endl;
    for (int i = 0; i < 10; ++i) {
        std::cout << std::hex << array[i] << std::endl;
    }
    std::cout << std::endl;

    auto startSimple = std::chrono::high_resolution_clock::now();
    InvertBits(array, array + 1000000, invertBits);
    auto endSimple = std::chrono::high_resolution_clock::now();

    std::cout << "After simple inversion:" << std::endl;
    for (int i = 0; i < 10; ++i) {
        std::cout << std::hex << array[i] << std::endl;
    }
    std::cout << std::endl;

    auto startParallel = std::chrono::high_resolution_clock::now();
    uint64_t sum_inverted = ParallelInvertBits(array, array + 1000000, invertBits);
    auto endParallel = std::chrono::high_resolution_clock::now();

    std::cout << "After parallel inversion:" << std::endl;
    for (int i = 0; i < 10; ++i) {
        std::cout << std::hex << array[i] << std::endl;
    }
    std::cout << std::endl;

    std::cout << "Simple inversion time: "
        << std::chrono::duration_cast<std::chrono::microseconds>(endSimple - startSimple).count()
        << " microseconds" << std::endl;

    std::cout << std::endl;

    std::cout << "Parallel inversion time: "
        << std::chrono::duration_cast<std::chrono::microseconds>(endParallel - startParallel).count()
        << " microseconds" << std::endl;

    return 0;
}