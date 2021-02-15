#ifndef OCCUPANCY_H
#define OCCUPANCY_H

#include <cstdint>
#include <SFML/System/Clock.hpp>

//TODO rename statistics
class cpuOccupancy{
public:
    void print();
    void addDisTime();
    void addEvtTime();
    void addSimTime();
    void addDrwTime();

private:
    int64_t _evtTime = 0;
    int64_t _simTime = 0;
    int64_t _drwTime = 0;
    int64_t _disTime = 0;
    int64_t _frames = 0;
    int64_t _sims = 0;
    void _clear();
    void _print() const;
    int64_t _getTotalTime() const;

    sf::Clock _clk;
};

#endif