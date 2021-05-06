#ifndef MODEL_CONSTANTS_H_
#define MODEL_CONSTANTS_H_

#include <QPointF>

namespace constants {

constexpr double pi = 3.141592653589;
constexpr double kEps = 0.00001;         // in blocks
constexpr double kAirResistance = 0.05;  // in percents
constexpr int kBlockSz = 16;             // in pixels
constexpr int kTickDurationMsec = 10;
constexpr double kAbsoluteMaxSpeedX = 5.0;  // in blocks per ms
constexpr double kAbsoluteMaxSpeedY = 5.0;  // in blocks per ms
constexpr int kDamageCooldown = 500;        // in ms
constexpr double kFallDamageMin = 0.04;
constexpr double kFallDamagePoint = 0.0005;

constexpr QPointF kPlayerSize{1.125, 2.5};              // in blocks
constexpr double kPlayerWalkAcceleration = 0.001 / 10;  // in blocks per ms
constexpr double kPlayerWalkMaxSpeed = 0.01;            // in blocks per ms
constexpr double kPlayerWalkAirAcceleration =
    kPlayerWalkAcceleration;  // in blocks per tick
constexpr double kPlayerWalkMaxAirAcceleration =
    kPlayerWalkMaxSpeed;                            // in blocks per ms
constexpr double kPlayerGravitySpeed = 0.001 / 10;  // in blocks per ms
constexpr double kPlayerJumpSpeed = -0.03;          // in blocks per ms
constexpr QPointF kPlayerDamageAcceleration = {0.01, -0.015};
constexpr int kPlayerHealth = 100;
constexpr int kPlayerDamage = 10;
constexpr double kPlayerUpperAttackAngle = pi / 4.0;
constexpr double kPlayerLowerAttackAngle = -pi / 4.0;
constexpr int kPlayerAttackTime = 300;
constexpr int kPlayerAttackCooldown = 700;
constexpr double kPlayerAngleTick =
    (kPlayerUpperAttackAngle - kPlayerLowerAttackAngle) / kPlayerAttackTime;
constexpr double kPlayerAttackRadius = 5;

constexpr double kMobWalkAcceleration =
    kPlayerWalkAcceleration / 2.5;  // in blocks per ms
constexpr double kMobWalkMaxSpeed =
    kPlayerWalkMaxSpeed / 1.8;  // in blocks per ms
constexpr double kMobWalkMaxAirAcceleration =
    kPlayerWalkMaxAirAcceleration / 2;                    // in blocks per ms
constexpr double kMobJumpSpeed = kPlayerJumpSpeed / 1.4;  // in blocks per ms
constexpr double kMobJumpHeightInBlocks = 2;              // in blocks
constexpr double kMobJumpLengthInBlocks = 2;              // in blocks
constexpr QPointF kMobDamageAcceleration = {0.01, -0.015};
constexpr int kMobHealth = 120;
constexpr int kMobDamage = 15;

constexpr double kBasicStrategyVisionRadius = 12.0;  // in blocks
constexpr double kBasicStrategyWalkTimeCount = 3000;
constexpr double kBasicStrategyAttackTimeCount = 1000;
constexpr double kBasicStrategyWalkPrecision = 0.1;  // in blocks
constexpr double kBasicStrategyRandomWalkChance = 0.01;
constexpr double kBasicStrategyRandomWalkDistance = 10;  // in blocks

constexpr double kMobSoundChance = 1.0 / 1500.0;  // once in 1500 ticks

constexpr int kDefaultClearTimeMSec = 1'000;

}  // namespace constants

#endif  // MODEL_CONSTANTS_H_
