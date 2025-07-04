#ifndef BENCHMARK_TIME_STATISTICS_H
#define BENCHMARK_TIME_STATISTICS_H

#include <vector>
#include <cmath>
#include <algorithm>
#include <chrono>
#include <limits>

class Statistics {
public:
    using Clock = std::chrono::high_resolution_clock;
    using TimePoint = std::chrono::time_point<Clock>;
    using Duration = std::chrono::nanoseconds;

    Statistics() : overflow_(false) {}

    // Start the timer for a measurement
    void start_timer() {
        start_time_ = Clock::now();
    }

    // Stop the timer and record the duration in nanoseconds
    void stop_timer() {
        TimePoint end_time = Clock::now();
        Duration duration = end_time - start_time_;
        long long nanos = duration.count();
        deltas_.push_back(nanos);
    }

    // Get the number of deltas collected
    size_t count() const {
        return deltas_.size();
    }

    // Check if an overflow occurred
    bool hasOverflow() const {
        return overflow_;
    }

    // Compute the mean of the deltas using integer logic for precision
    long long mean() const {
        if (deltas_.empty()) return 0;
        long long sum = 0;
        for (long long delta : deltas_) {
            if (sum > std::numeric_limits<long long>::max() - delta) {
                overflow_ = true;
                return 0;
            }
            sum += delta;
        }
        return sum / static_cast<long long>(deltas_.size());
    }

    // Compute the median of the deltas
    long long median() const {
        if (deltas_.empty()) return 0;
        std::vector<long long> sorted_deltas = deltas_;
        std::sort(sorted_deltas.begin(), sorted_deltas.end());
        size_t mid = sorted_deltas.size() / 2;
        if (sorted_deltas.size() % 2 == 0) {
            long long lower = sorted_deltas[mid - 1];
            long long upper = sorted_deltas[mid];
            if (lower > std::numeric_limits<long long>::max() - upper) {
                return (lower/2) + (upper/2);
            }
            return (lower + upper)/2;
        } else {
            return sorted_deltas[mid];
        }
    }

    // Compute the 90th percentile (P90) of the deltas
    long long p90() const {
        if (deltas_.empty()) return 0;
        std::vector<long long> sorted_deltas = deltas_;
        std::sort(sorted_deltas.begin(), sorted_deltas.end());
        size_t index = static_cast<size_t>(std::ceil(sorted_deltas.size() * 0.9)) - 1;
        return sorted_deltas[index];
    }

    // Compute the standard deviation of the deltas
    long long standardDeviation() const {
        if (deltas_.empty() || deltas_.size() < 2) return 0;
        long long m = mean();
        if (overflow_) return 0;
        long long sum_squared_diff = 0;
        for (long long delta : deltas_) {
            long long diff = delta - m;
            long long squared_diff = diff * diff;
            if (sum_squared_diff > std::numeric_limits<long long>::max() - squared_diff) {
                overflow_ = true;
                return 0;
            }
            sum_squared_diff += squared_diff;
        }
        return static_cast<long long>(std::sqrt(static_cast<double>(sum_squared_diff) / static_cast<double>(deltas_.size() - 1)));
    }

    // Get a const reference to the deltas for export purposes
    const std::vector<long long>& getDeltas() const {
        return deltas_;
    }

private:
    std::vector<long long> deltas_;
    TimePoint start_time_;
    mutable bool overflow_;
};

#endif // BENCHMARK_TIME_STATISTICS_H
