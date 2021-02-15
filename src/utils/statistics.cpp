#include <iostream>
#include "statistics.h"

#include <SFML/System/Time.hpp>

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
    std::cout 
        << "evt:" << 100.0 * _evtTime / total
        << "% sim:" << 100.0 * _simTime / total
        << "% draw:" << 100.0 * _drwTime / total
        << "% display:" << 100.0 * _disTime / total
        << "% total:" << total
        << "us" << std::endl;
    float fps = (_frames * 1e6f) / total;
    std::cout<<"FPS:"<<fps<< " ("<<1e6f/fps <<"us)"<<std::endl;
    float simps = (_sims * 1e6f) / total;
    std::cout<<"SimPS:"<<simps<< " ("<<1e6f/simps <<"us)"<<std::endl;
}