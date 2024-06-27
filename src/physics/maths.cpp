#include "physics/maths.h"

// https://en.wikipedia.org/wiki/Conversion_between_quaternions_and_Euler_angles

physx::PxQuat PhysicsUtil::eulerToQuaternion(physx::PxVec3 eulerAngle){

    float roll = eulerAngle.x, pitch = eulerAngle.y, yaw = eulerAngle.z;
    float cr = std::cos(roll * 0.5);
    float sr = std::sin(roll * 0.5);
    float cp = std::cos(pitch * 0.5);
    float sp = std::sin(pitch * 0.5);
    float cy = std::cos(yaw * 0.5);
    float sy = std::sin(yaw * 0.5);

    physx::PxQuat q;
    q.w = cr * cp * cy + sr * sp * sy;
    q.x = sr * cp * cy - cr * sp * sy;
    q.y = cr * sp * cy + sr * cp * sy;
    q.z = cr * cp * sy - sr * sp * cy;
    return q;
}

physx::PxVec3 PhysicsUtil::quaternionToEuler(physx::PxQuat q){
        // roll (x-axis rotation)
        float sinr_cosp = 2.f * (q.w * q.x + q.y * q.z);
        float cosr_cosp = 1.f - 2.f * (q.x * q.x + q.y * q.y);
        float roll = std::atan2(sinr_cosp, cosr_cosp);

        // pitch (y-axis rotation)
        float sinp = std::sqrt(1.f + 2.f * (q.w * q.y - q.x * q.z));
        float cosp = std::sqrt(1.f - 2.f * (q.w * q.y - q.x * q.z));
        float pitch = 2.f * std::atan2(sinp, cosp) - M_PI / 2.f;

        // yaw (z-axis rotation)
        float siny_cosp = 2.f * (q.w * q.z + q.x * q.y);
        float cosy_cosp = 1.f - 2.f * (q.y * q.y + q.z * q.z);
        float yaw = std::atan2(siny_cosp, cosy_cosp);

        return physx::PxVec3(roll, pitch, yaw);
}

physx::PxVec3 PhysicsUtil::glmToPxVec3(glm::vec3 vec){
    return physx::PxVec3(vec.x, vec.y, vec.z);
}

float PhysicsUtil::radianToDegree(float rad){
    return (rad * 180.f) / M_PI;
}

float PhysicsUtil::degreeToRadian(float deg){
    return (deg * M_PI) / 180.f;
}
