#ifndef MODEL_CONSTANTS_H_
#define MODEL_CONSTANTS_H_

namespace constants {

constexpr double kEps = 0.00001;         // in blocks
constexpr double kAirResistance = 0.05;  // in percents
constexpr int kBlockSz = 16;             // in pixels
constexpr int kTickDurationMsec = 10;
constexpr double kAbsoluteMaxSpeedX = 50.0;  // in blocks per tick
constexpr double kAbsoluteMaxSpeedY = 50.0;  // in blocks per tick

}  // namespace constants

#endif  // MODEL_CONSTANTS_H_
