/**
 * @file compare_vector_list.cpp
 * @brief Compares the performance of sorted insertion and random removal 
 *        in std::vector<int> vs. std::list<int>.
 *
 * This program measures and reports the time it takes to:
 * - Insert N random integers into a container while maintaining sorted order.
 * - Remove N elements by picking a random index each time.
 *
 * Containers compared:
 *   - std::vector<int>
 *   - std::list<int>
 *
 * The results are printed to standard output as:
 *     N=<size> vector=<time_microseconds> list=<time_microseconds> list/vector=<ratio>
 *
 * Where:
 *   - <size> is the number of elements N used for that test.
 *   - <time_microseconds> is the total microseconds for insertion + removal.
 *   - <ratio> is (time for list) / (time for vector).
 *
 * You can adjust the test sizes in the `testSizes` vector in main().
 */

#include <algorithm>    
#include <chrono>       
#include <iostream>    
#include <list>        
#include <random>       
#include <vector>     

/**
 * @brief Inserts N random integers into a std::vector<int> in sorted order.
 *
 * @param v   Reference to a std::vector<int> to insert into.
 * @param N   Number of elements to insert.
 * @param gen A random number generator (Mersenne Twister).
 *
 * Each new integer is placed in the correct position to maintain sorted order,
 * using std::lower_bound to locate the insertion position in O(log N) time,
 * but insertion itself can be O(N) due to shifting elements in a std::vector.
 */
void insertSortedVector(std::vector<int>& v, int N, std::mt19937& gen) {
    std::uniform_int_distribution<int> dist(0, 1000000);
    for(int i = 0; i < N; ++i) {
        int val = dist(gen);
        auto pos = std::lower_bound(v.begin(), v.end(), val);
        v.insert(pos, val);
    }
}

/**
 * @brief Removes N elements from a std::vector<int>, each time erasing 
 *        at a random index.
 *
 * @param v   Reference to a std::vector<int> to remove from.
 * @param N   Number of elements to remove.
 * @param gen A random number generator (Mersenne Twister).
 *
 * For each removal:
 *   1. A random index in the range [0, v.size() - 1] is chosen.
 *   2. The element at that index is erased (O(N) for a vector).
 */
void removeRandomPositionsVector(std::vector<int>& v, int N, std::mt19937& gen) {
    for(int i = 0; i < N; ++i) {
        std::uniform_int_distribution<size_t> dist(0, v.size() - 1);
        v.erase(v.begin() + dist(gen));
    }
}

/**
 * @brief Inserts N random integers into a std::list<int> in sorted order.
 *
 * @param lst Reference to a std::list<int> to insert into.
 * @param N   Number of elements to insert.
 * @param gen A random number generator (Mersenne Twister).
 *
 * Each integer is placed in its correct position to maintain sorted order.
 * Finding the position is O(N) because std::list only supports bidirectional
 * iterators, but insertion itself is O(1).
 */
void insertSortedList(std::list<int>& lst, int N, std::mt19937& gen) {
    std::uniform_int_distribution<int> dist(0, 1000000);
    for(int i = 0; i < N; ++i) {
        int val = dist(gen);

        // linear search for the first position where *it > val
        auto it = lst.begin();
        for(; it != lst.end(); ++it) {
            if(*it > val) {
                break;
            }
        }
        lst.insert(it, val);
    }
}

/**
 * @brief Removes N elements from a std::list<int>, each time erasing
 *        the element at a random index.
 *
 * @param lst Reference to a std::list<int> to remove from.
 * @param N   Number of elements to remove.
 * @param gen A random number generator (Mersenne Twister).
 *
 * For each removal:
 *   1. A random index in the range [0, lst.size() - 1] is chosen.
 *   2. We advance a list iterator to that index in O(N).
 *   3. Erase the element in O(1).
 */
void removeRandomPositionsList(std::list<int>& lst, int N, std::mt19937& gen) {
    for(int i = 0; i < N; ++i) {
        std::uniform_int_distribution<size_t> dist(0, lst.size() - 1);
        size_t idx = dist(gen);

        auto it = lst.begin();
        std::advance(it, idx);
        lst.erase(it);
    }
}

/**
 * @brief Main entry point. Tests performance for various N values using both std::vector and std::list.
 *
 * @return int Program exit code (0 for success).
 *
 * For each N in testSizes:
 *   1. Insert N random numbers in sorted order in std::vector, remove N random positions.
 *   2. Insert N random numbers in sorted order in std::list, remove N random positions.
 *   3. Compare and print times in microseconds along with ratio listTime / vectorTime.
 */
int main() {
    std::vector<int> testSizes = {100, 1000, 10000, 30000, 100000};

    std::random_device rd;
    std::mt19937 gen(rd());

    for (auto N : testSizes) {
        {
            std::vector<int> vec;
            auto start = std::chrono::high_resolution_clock::now();
            insertSortedVector(vec, N, gen);
            removeRandomPositionsVector(vec, N, gen);
            auto end = std::chrono::high_resolution_clock::now();
            auto vecTime = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

            std::list<int> lst;
            start = std::chrono::high_resolution_clock::now();
            insertSortedList(lst, N, gen);
            removeRandomPositionsList(lst, N, gen);
            end = std::chrono::high_resolution_clock::now();
            auto listTime = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

            double ratio = static_cast<double>(listTime) / vecTime;

            std::cout << "N=" << N
                      << " vector=" << vecTime
                      << " list=" << listTime
                      << " list/vector=" << ratio
                      << "\n";
        }
    }

    return 0;
}
