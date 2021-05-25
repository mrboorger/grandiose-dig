#ifndef MODEL_CONSTANTS_H_
#define MODEL_CONSTANTS_H_

#include <QDebug>
#include <QPointF>

#include "types.h"

namespace constants {

constexpr double pi = 3.141592653589;
constexpr double kEps = 0.00001;         // in blocks
constexpr double kAirResistance = 0.05;  // in percents
constexpr int kBlockSz = 16;             // in pixels
constexpr int kTickDurationMsec = 10;
constexpr double kAbsoluteMaxSpeedX = 5.0;  // in blocks per ms
constexpr double kAbsoluteMaxSpeedY = 5.0;  // in blocks per ms
constexpr int kDamageCooldown = 500;        // in ms
constexpr double kFallDamageMin = 0.05;
constexpr double kFallDamagePoint = 0.002;

constexpr QPointF kPlayerSize{1.25, 2.5};               // in blocks
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
constexpr int kPlayerWalkAnimation = 50;  // in ms
constexpr int kPlayerWalkPictures = 4;
constexpr int kPlayerAttackPictures = 4;  // number of pictures in animation
constexpr int kPlayerAttackAnimation =
    kPlayerAttackTime / kPlayerAttackPictures;  // in ms

constexpr double kSpeedEffectMultiplier = 1.2;
constexpr double kSlownessEffectMultiplier = 0.8;
constexpr double kStrengthEffectMultiplier = 1.5;
constexpr double kWeaknessEffectMultiplier = 1.0 / 1.5;
constexpr double kPoisonEffectMultiplier = 5;
constexpr double kRegenerationEffectMultiplier = 5;
constexpr double kLightnessEffectMultiplier = 0.8;
constexpr double kHeavinessEffectMultiplier = 1.0 / 0.8;

constexpr int kMaxAnimationPictures = 4;

constexpr MobParameters kZombieParameters = {
    /* walk_acceleration */ kPlayerWalkAcceleration / 2.5,
    /* walk_max_speed */ kPlayerWalkMaxSpeed / 1.8,
    /* walk_air_acceleration */ kPlayerWalkAirAcceleration,
    /* walk_max_air_acceleration */ kPlayerWalkMaxAirAcceleration / 2,
    /* jump_speed */ kPlayerJumpSpeed / 1.4,
    /* jump_in_blocks */ {2, 2},
    /* size */ kPlayerSize,
    /* damage_acceleration */ {0.01, -0.015},
    /* health */ 120,
    /* damage */ 15,
    /* walk_animation */ 100,
    /* walk_pictures */ 4,
    /* attack_animation */ 100,
    /* attack_pictures */ 4};

constexpr MobParameters kZombieLordParameters = {
    /* walk_acceleration */ kPlayerWalkAcceleration / 1.5,
    /* walk_max_speed */ kPlayerWalkMaxSpeed / 1.3,
    /* walk_air_acceleration */ kPlayerWalkAirAcceleration,
    /* walk_max_air_acceleration */ kPlayerWalkMaxAirAcceleration / 1.2,
    /* jump_speed */ kPlayerJumpSpeed / 1.2,
    /* jump_in_blocks */ {4, 3},
    /* size */ {1.875, 3.75},
    /* damage_acceleration */ {0.025, -0.025},
    /* health */ 200,
    /* damage */ 25,
    /* walk_animation */ 100,
    /* walk_pictures */ 4,
    /* attack_animation */ 100,
    /* attack_pictures */ 4};

constexpr MobParameters kQuioxParameters = {
    /* walk_acceleration */ kPlayerWalkAcceleration * 1.5,
    /* walk_max_speed */ kPlayerWalkMaxSpeed * 0.9,
    /* walk_air_acceleration */ kPlayerWalkAirAcceleration,
    /* walk_max_air_acceleration */ kPlayerWalkMaxAirAcceleration * 1.1,
    /* jump_speed */ kPlayerJumpSpeed / 1.4,
    /* jump_in_blocks */ {4, 2},
    /* size */ {2.875, 1.375},
    /* damage_acceleration */ {0.01, -0.015},
    /* health */ 150,
    /* damage */ 15,
    /* walk_animation */ 100,
    /* walk_pictures */ 2,
    /* attack_animation */ 100,
    /* attack_pictures */ 2};

constexpr MobParameters kMagicQuioxParameters = {
    /* walk_acceleration */ kPlayerWalkAcceleration * 1.5,
    /* walk_max_speed */ kPlayerWalkMaxSpeed * 0.9,
    /* walk_air_acceleration */ kPlayerWalkAirAcceleration,
    /* walk_max_air_acceleration */ kPlayerWalkMaxAirAcceleration * 1.1,
    /* jump_speed */ kPlayerJumpSpeed / 1.4,
    /* jump_in_blocks */ {4, 2},
    /* size */ {2.875, 1.375},
    /* damage_acceleration */ {0.01, -0.015},
    /* health */ 200,
    /* damage */ 10,
    /* walk_animation */ 100,
    /* walk_pictures */ 2,
    /* attack_animation */ 100,
    /* attack_pictures */ 2};

constexpr int kMagicStrategyEffectDuration = 10000;
constexpr double kMagicStrategyEffectChance = 0.2;

constexpr double kBasicStrategyVisionRadius = 24.0;  // in blocks
constexpr double kBasicStrategyWalkTimeCount = 3000;
constexpr double kBasicStrategyAttackTimeCount = 1000;
constexpr double kBasicStrategyWalkPrecision = 0.1;  // in blocks
constexpr double kBasicStrategyRandomWalkChance = 0.01;
constexpr double kBasicStrategyRandomWalkDistance = 10;  // in blocks

constexpr double kZombieSummonerStrategyVisionRadius = 40.0;  // in blocks
constexpr double kZombieSummonerStrategyWalkTime = 5000;
constexpr double kZombieSummonerStrategyAttackTime = 1250;
constexpr double kZombieSummonerStrategySummonTime = 1000;  // before check
constexpr double kZombieSummonerStrategySummonDistance = 7.5;
constexpr double kZombieSummonerStrategySummonChance = 0.2;
constexpr int kZombieSummonerStrategySummonAttempts = 200;    // in blocks
constexpr double kZombieSummonerStrategyWalkPrecision = 0.1;  // in blocks
constexpr double kZombieSummonerStrategyRandomWalkChance = 0.015;
constexpr double kZombieSummonerStrategyRandomWalkDistance = 15;  // in blocks

constexpr double kEffectInterval = 1000;          // in ms
constexpr double kMobSoundChance = 1.0 / 1500.0;  // once in 1500 ticks

constexpr int kMusicVolume = 50;

constexpr int kMobsCount = 20;
constexpr double kMobSoundRadius = 50;
constexpr double kMobSoundC1 = 0.99;
constexpr double kMobSoundC2 = 1.1;
constexpr double kMobsDespawnRadius = 128;
constexpr int kMobsSpawnLight = 64;

constexpr double kMobSpawnChance = 0.002;
constexpr double kMobMinSpawnDistance = 24;
constexpr double kMobMaxSpawnDistance = 120;
constexpr int kMobSpawnAttempts = 1000;

constexpr int kMobSpawnStartDelay = 5000;

constexpr double kMobsSpawnRate[4] = {1, 0.2, 0.8, 0.4};
constexpr double kMobsSumRate = kMobsSpawnRate[0] + kMobsSpawnRate[1] +
                                kMobsSpawnRate[2] + kMobsSpawnRate[3];

constexpr int kDefaultClearTimeMSec = 5'000;
constexpr int kSaveDurationMsec = 7'000;

}  // namespace constants

#endif  // MODEL_CONSTANTS_H_
