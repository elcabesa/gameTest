#ifndef OCCUPANCY_H
#define OCCUPANCY_H

#include <cstdint>

class cpuOccupancy{
public:
    void add(int64_t evtTime,int64_t simTime, int64_t drwTime, int64_t disTime);
private:
    int64_t _evtTime = 0;
    int64_t _simTime = 0;
    int64_t _drwTime = 0;
    int64_t _disTime = 0;
    unsigned int _n = 0;
    void _clear();
    void _print() const;
};

#endif