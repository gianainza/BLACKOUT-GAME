#pragma once
// R5 - QA and Performance: whole-frame CPU wall-clock timing harness.
// C++14, standard library only. No OpenGL dependency in this file.

#include <algorithm>
#include <chrono>
#include <cmath>
#include <cstddef>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <numeric>
#include <string>
#include <utility>
#include <vector>

class FrameTimeLogger {
public:
    static constexpr std::size_t WarmupFrames = 120;
    static constexpr std::size_t MeasurementFrames = 300;

    explicit FrameTimeLogger(std::string outputPath)
        : outputPath_(std::move(outputPath)) {
        samplesMs_.reserve(MeasurementFrames);
    }

    void BeginFrame() {
        if (finished_) return;
        frameStart_ = Clock::now();
        frameStarted_ = true;
    }

    void EndFrame() {
        if (finished_ || !frameStarted_) return;

        const auto frameEnd = Clock::now();
        frameStarted_ = false;
        const double milliseconds =
            std::chrono::duration<double, std::milli>(frameEnd - frameStart_).count();

        ++framesSeen_;
        if (framesSeen_ <= WarmupFrames) return;
        samplesMs_.push_back(milliseconds);

        if (samplesMs_.size() == MeasurementFrames) {
            WriteResults();
            finished_ = true;
        }
    }

    bool IsFinished() const { return finished_; }

private:
    using Clock = std::chrono::steady_clock;
    Clock::time_point frameStart_{};
    std::string outputPath_;
    std::vector<double> samplesMs_;
    std::size_t framesSeen_ = 0;
    bool frameStarted_ = false;
    bool finished_ = false;

    void WriteResults() const {
        const double total = std::accumulate(samplesMs_.begin(), samplesMs_.end(), 0.0);
        const double mean = total / static_cast<double>(samplesMs_.size());
        std::vector<double> sorted = samplesMs_;
        std::sort(sorted.begin(), sorted.end());
        const double median = (sorted[149] + sorted[150]) / 2.0;
        const double p95 = sorted[284]; // ceil(0.95 * 300) - 1, zero-based

        // File writing happens only AFTER the timed 300 frames.
        std::ofstream csv(outputPath_.c_str(), std::ios::out | std::ios::trunc);
        if (!csv.is_open()) {
            std::cerr << "[R5] ERROR: Could not write " << outputPath_
                      << ". Check the Visual Studio working directory.\n";
        } else {
            csv << "frame,frame_time_ms\n";
            csv << std::fixed << std::setprecision(4);
            for (std::size_t i = 0; i < samplesMs_.size(); ++i) {
                csv << (i + 1) << ',' << samplesMs_[i] << '\n';
            }
            csv.close();
            if (!csv) {
                std::cerr << "[R5] ERROR: Could not finish writing " << outputPath_ << "\n";
            } else {
                std::cout << "[R5] Saved 300 frame times to " << outputPath_ << "\n";
            }
        }
        std::cout << std::fixed << std::setprecision(3)
                  << "[R5] Mean: " << mean << " ms/frame (" << (1000.0 / mean)
                  << " FPS equivalent), median: " << median
                  << " ms, P95: " << p95 << " ms, min: " << sorted.front()
                  << " ms, max: " << sorted.back() << " ms.\n";
        std::cout << "[R5] CPU-observed loop time, including buffer swap; "
                     "NOT an isolated GPU timer.\n";
    }
};
