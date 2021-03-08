#ifndef MODEL_CONSTANTS_H_
#define MODEL_CONSTANTS_H_

namespace constants {

constexpr double kEPS = 0.00001;           // epslion for physics
constexpr int kBlockSz = 16;               // in pixels
constexpr int kTick = 10;                  // in milliseconds
constexpr double kAbsoluteMaxSpeedX = 50;  // in blocks per tick
constexpr double kAbsoluteMaxSpeedY = 50;  // in blocks per tick

}  // namespace constants

#endif  // MODEL_CONSTANTS_H_
