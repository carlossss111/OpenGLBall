#include "physics/physics_util.h"

// https://en.wikipedia.org/wiki/Conversion_between_quaternions_and_Euler_angles
// https://stackoverflow.com/questions/1556260/convert-quaternion-rotation-to-rotation-matrix
// https://math.stackexchange.com/questions/893984/conversion-of-rotation-matrix-to-quaternion

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
    float pitch = 2.f * std::atan2(sinp, cosp) - PI / 2.f;

    // yaw (z-axis rotation)
    float siny_cosp = 2.f * (q.w * q.z + q.x * q.y);
    float cosy_cosp = 1.f - 2.f * (q.y * q.y + q.z * q.z);
    float yaw = std::atan2(siny_cosp, cosy_cosp);

    return physx::PxVec3(roll, pitch, yaw);
}

physx::PxQuat PhysicsUtil::matrixToQuaternion(physx::PxMat44 m){
    physx::PxQuat quat;
    float t;
    if (m.column2.z < 0) {
        if (m.column0.x > m.column1.y) {
            t = 1.f + m.column0.x - m.column1.y - m.column2.z;
            quat = physx::PxQuat(t, m.column1.x + m.column0.y, m.column0.z + m.column2.x, m.column2.y - m.column1.z);
        }
        else {
            t = 1.f - m.column0.x + m.column1.y - m.column2.z;
            quat = physx::PxQuat(m.column1.x + m.column0.y, t, m.column2.y + m.column1.z, m.column0.z - m.column2.x);
        }
    }
    else {
        if (m.column0.x < -m.column1.y) {
            t = 1.f - m.column0.x - m.column1.y + m.column2.z;
            quat = physx::PxQuat(m.column0.z + m.column2.x, m.column2.y + m.column1.z, t, m.column1.x - m.column0.y);
        }
        else {
            t = 1.f + m.column0.x + m.column1.y + m.column2.z;
            quat = physx::PxQuat(m.column2.y - m.column1.z, m.column0.z - m.column2.x, m.column1.x - m.column0.y, t);
        }
    }
    quat *= 0.5f / std::sqrt(t);
    return quat;
}

physx::PxMat44 PhysicsUtil::quaternionToMatrix(physx::PxQuat q){
    physx::PxMat44 rotMatrix = physx::PxMat44(physx::PxIdentity);
    rotMatrix.column0.x = 1.0f - 2.0f * (q.y * q.y) - 2.0f * (q.z * q.z);
    rotMatrix.column1.x = 2.0f * (q.x * q.y) - 2.0f * (q.z * q.w);
    rotMatrix.column2.x = 2.0f * (q.x * q.z) + 2.0f * (q.y * q.w);

    rotMatrix.column0.y = 2.0f * (q.x * q.y) + 2.0f * (q.z * q.w);
    rotMatrix.column1.y = 1.0f - 2.0f * (q.x * q.x) - 2.0f * (q.z * q.z); 
    rotMatrix.column2.y = 2.0f * (q.y * q.z) - 2.0f * (q.x * q.w);
    
    rotMatrix.column0.z = 2.0f * (q.x * q.z) - 2.0f * (q.y * q.w);
    rotMatrix.column1.z = 2.0f * (q.y * q.z) + 2.0f * (q.x * q.w);
    rotMatrix.column2.z = 1.0f - 2.0f * (q.x * q.x) - 2.0f * (q.y * q.y);
    return rotMatrix;
}

float PhysicsUtil::radianToDegree(float rad){
    return (rad * 180.f) / PI;
}

float PhysicsUtil::degreeToRadian(float deg){
    return (deg * PI) / 180.f;
}

physx::PxVec3 PhysicsUtil::glmToPxVec3(glm::vec3 vec){
    return physx::PxVec3(vec.x, vec.y, vec.z);
}

glm::vec3 PhysicsUtil::pxToGlmVec3(physx::PxVec3 vec){
    return glm::vec3(vec.x, vec.y, vec.z);
}

physx::PxMat44 PhysicsUtil::glmToPxMat4(glm::mat4 glMat){
    physx::PxMat44 nvMat;
    nvMat.column0.x = glMat[0][0];
    nvMat.column1.x = glMat[0][1];
    nvMat.column2.x = glMat[0][2];
    nvMat.column3.x = glMat[0][3];
    nvMat.column0.y = glMat[1][0];
    nvMat.column1.y = glMat[1][1];
    nvMat.column2.y = glMat[1][2];
    nvMat.column3.y = glMat[1][3];
    nvMat.column0.z = glMat[2][0];
    nvMat.column1.z = glMat[2][1];
    nvMat.column2.z = glMat[2][2];
    nvMat.column3.z = glMat[2][3];
    nvMat.column0.w = glMat[3][0];
    nvMat.column1.w = glMat[3][1];
    nvMat.column2.w = glMat[3][2];
    nvMat.column3.w = glMat[3][3];
    return nvMat;
}

glm::mat4 PhysicsUtil::pxToGlmMat4(physx::PxMat44 nvMat){
    return glm::mat4 {
        nvMat.column0.x,
        nvMat.column0.y,
        nvMat.column0.z,
        nvMat.column0.w,

        nvMat.column1.x,
        nvMat.column1.y,
        nvMat.column1.z,
        nvMat.column1.w,

        nvMat.column2.x,
        nvMat.column2.y,
        nvMat.column2.z,
        nvMat.column2.w,

        nvMat.column3.x,
        nvMat.column3.y,
        nvMat.column3.z,
        nvMat.column3.w
    };
}
