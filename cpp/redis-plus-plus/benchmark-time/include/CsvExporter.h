#ifndef BENCHMARK_TIME_CSV_EXPORTER_H
#define BENCHMARK_TIME_CSV_EXPORTER_H

#include <string>
#include <vector>
#include <map>
#include <fstream>
#include <iomanip>
#include <iostream>
#include "Statistics.h"

class CsvExporter {
public:
    enum TimeUnit {
        NANOSECONDS,
        MICROSECONDS,
        MILLISECONDS,
        SECONDS
    };

    CsvExporter(const std::string& project_name, const std::string& test_run, TimeUnit unit = NANOSECONDS)
        : project_name_(project_name), test_run_(test_run), output_dir_("results/"), time_unit_(unit) {}

    // Set a custom output directory (default is "results/")
    void setOutputDir(const std::string& dir) {
        this->output_dir_ = dir;
    }

    // Set the time unit for export conversion
    void setTimeUnit(TimeUnit unit) {
        this->time_unit_ = unit;
    }

    // Export raw data for multiple operations to a CSV file
    bool exportRawDataToCsv(const std::map<std::string, Statistics>& operation_stats) const {
        // Construct filename as PROJECTNAME_TESTRUN_raw.csv
        std::string filename = this->project_name_ + "_raw" + this->test_run_ + ".csv";
        std::string filepath = this->output_dir_ + filename;

        std::ofstream ofs(filepath);
        if (!ofs.is_open()) {
            return false; // Failed to open file
        }

        // Set precision for floating-point output based on time unit
        ofs << std::fixed << std::setprecision(this->getPrecision());

        // Write header for raw data section
        std::string unit_label = this->getUnitLabel();
        ofs << "Operation,Delta 1 (" << unit_label << "),Delta 2 (" << unit_label << "),Delta 3 (" << unit_label << "),...\n";

        // Write raw deltas for each operation on a single line
        int operation_count = 0;
        for (const auto& pair : operation_stats) {
            const std::string& operation = pair.first;
            const Statistics& stats = pair.second;
            const std::vector<long long>& deltas = stats.getDeltas();
            ofs << operation;
            for (long long delta : deltas) {
                ofs << "," << this->convertToUnit(delta);
            }
            ofs << "\n";
            operation_count++;
        }

        ofs.close();
        return true;
    }

    // Export statistics for multiple operations to a separate CSV file
    bool exportStatsToCsv(const std::map<std::string, Statistics>& operation_stats) const {
        // Construct filename as PROJECTNAME_TESTRUN_stats.csv
        std::string filename = this->project_name_ + "_stats" + this->test_run_ + ".csv";
        std::string filepath = this->output_dir_ + filename;

        std::ofstream ofs(filepath);
        if (!ofs.is_open()) {
            return false; // Failed to open file
        }

        // Set precision for floating-point output based on time unit
        ofs << std::fixed << std::setprecision(this->getPrecision());

        // Write header for statistics
        std::string unit_label = this->getUnitLabel();
        ofs << "Operation,Mean (" << unit_label << "),Median (" << unit_label << "),P90 (" << unit_label << "),Standard Deviation (" << unit_label << "),Count\n";

        int operation_count = 0;
        for (const auto& pair : operation_stats) {
            const std::string& operation = pair.first;
            const Statistics& stats = pair.second;
            ofs << operation << ","
                << this->convertToUnit(stats.mean()) << ","
                << this->convertToUnit(stats.median()) << ","
                << this->convertToUnit(stats.p90()) << ","
                << this->convertToUnit(stats.standardDeviation()) << ","
                << stats.count() << "\n";
            operation_count++;
        }

        ofs.close();
        return true;
    }

    // Convenience method to export both raw data and statistics
    bool exportAllToCsv(const std::map<std::string, Statistics>& operation_stats) const {
        bool raw_success = this->exportRawDataToCsv(operation_stats);
        bool stats_success = this->exportStatsToCsv(operation_stats);
        return raw_success && stats_success;
    }

private:
    std::string getUnitLabel() const {
        switch (this->time_unit_) {
            case NANOSECONDS: return "ns";
            case MICROSECONDS: return "us";
            case MILLISECONDS: return "ms";
            case SECONDS: return "s";
            default: return "ms";
        }
    }

    int getPrecision() const {
        switch (this->time_unit_) {
            case NANOSECONDS: return 0;
            case MICROSECONDS: return 3;
            case MILLISECONDS: return 6;
            case SECONDS: return 9;
            default: return 6;
        }
    }

    double convertToUnit(long long nanos) const {
        switch (this->time_unit_) {
            case NANOSECONDS: return static_cast<double>(nanos);
            case MICROSECONDS: return static_cast<double>(nanos) / 1'000.0;
            case MILLISECONDS: return static_cast<double>(nanos) / 1'000'000.0;
            case SECONDS: return static_cast<double>(nanos) / 1'000'000'000.0;
            default: return static_cast<double>(nanos) / 1'000'000.0;
        }
    }

    std::string project_name_;
    std::string test_run_;
    std::string output_dir_;
    TimeUnit time_unit_;
};

#endif // BENCHMARK_TIME_CSV_EXPORTER_H
