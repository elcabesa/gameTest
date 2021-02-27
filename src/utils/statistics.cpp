#include <SFML/System/Time.hpp>

#include "spdlog/sinks/stdout_color_sinks.h"
#include "statistics.h"

EngineStatistics::EngineStatistics() {
    _logger = spdlog::stdout_color_mt("Statistics");
}

void EngineStatistics::addDisTime() {
    _disTime += _clk.restart().asMicroseconds();
}

void EngineStatistics::addEvtTime() {
    _evtTime += _clk.restart().asMicroseconds();
}

void EngineStatistics::addSimTime() {
    ++_sims;
    _simTime += _clk.restart().asMicroseconds();
}

void EngineStatistics::addDrwTime() {
    ++_frames;
    _drwTime += _clk.restart().asMicroseconds();
}

int64_t EngineStatistics::_getTotalTime() const {
    return _evtTime + _simTime + _drwTime + _disTime;
}

void EngineStatistics::print() {
    if (_getTotalTime() >= 1000000) {
        _print();
        _clear();
    }
}

void EngineStatistics::_clear() {
    _evtTime = 0;
    _simTime = 0;
    _drwTime = 0;
    _disTime = 0;
    _frames = 0;
    _sims = 0;
}

void EngineStatistics::_print() const {
    int64_t total = _getTotalTime();
    _logger->trace("evt:{}% sim:{}% draw:{}% display:{}% total:{}us", 
        100.0 * _evtTime / total,
        100.0 * _simTime / total,
        100.0 * _drwTime / total,
        100.0 * _disTime / total,
        total
    );

    float fps = (_frames * 1e6f) / total;
    _logger->debug("FPS:{} ({}us)", fps, 1e6f/fps);
    float simps = (_sims * 1e6f) / total;
    _logger->debug("SimPS:{} ({}us)", simps, 1e6f/simps);
}