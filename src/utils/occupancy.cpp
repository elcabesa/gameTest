#include <iostream>
#include "occupancy.h"

void cpuOccupancy::add(int64_t evtTime,int64_t simTime, int64_t drwTime, int64_t disTime) {
    _evtTime += evtTime;
    _simTime += simTime;
    _drwTime += drwTime;
    _disTime += disTime;
    if (++_n >= 50) {
        _print();
        _clear();
    }
}

void cpuOccupancy::_clear() {
    _evtTime = 0;
    _simTime = 0;
    _drwTime = 0;
    _disTime = 0;
    _n = 0;
}

void cpuOccupancy::_print() const {
    int64_t total = _evtTime + _simTime + _drwTime + _disTime;
    std::cout 
        << "evt:" << 100.0 * _evtTime / total
        << "% sim:" << 100.0 * _simTime / total
        << "% draw:" << 100.0 * _drwTime / total
        << "% display:" << 100.0 * _disTime / total
        << "%" << std::endl;
    float fps = 50.0 / total * 1e6;
    std::cout<<"FPS:"<<fps<< " ("<<total/50 <<"us)"<<std::endl;
}