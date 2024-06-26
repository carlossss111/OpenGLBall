#include "scene/procedural/sphere.h"

Sphere::Sphere(std::string directoryPath, std::string filePath) {
    // A sphere can be defined with triangles.
    // The sector_count is the number of rectangles around the cirumference horizontally
    // The stack_count is the number of rectangles around the circumference vertically
    std::vector<Vertex> verts;
    std::vector<unsigned int> indices;
    std::vector<Texture> textures;
    constexpr float PI = glm::pi<float>();

    float x, y, z, xy;
    float nx, ny, nz, lengthInv = 1.f / mRadius;

    float sectorStep = 2 * PI / mSectorCount;
    float stackStep = PI / mStackCount;
    float sectorAngle, stackAngle;

    // Define vertices
    for (int i = 0; i <= mStackCount; ++i) {
        stackAngle = PI / 2 - i * stackStep;       // starting from pi/2 to -pi/2
        xy = mRadius * cosf(stackAngle);             // r * cos(u)
        z = mRadius * sinf(stackAngle);              // r * sin(u)

        // Add (sectorCount + 1) vertices per stack
        for (int j = 0; j <= mSectorCount; ++j) {
            Vertex vertex;
            sectorAngle = j * sectorStep;          // starting from 0 to 2pi

            // Positions
            x = xy * cosf(sectorAngle);             // r * cos(u) * cos(v)
            y = xy * sinf(sectorAngle);             // r * cos(u) * sin(v)
            glm::vec3 pos(x, y, z);

            // Normals
            nx = x * lengthInv;
            ny = y * lengthInv;
            nz = z * lengthInv;
            glm::vec3 nor = glm::vec3(nx, ny, nz);

            // Texels
            float u = static_cast<float>(j / mSectorCount);
            float v = static_cast<float>(i / mStackCount);
            glm::vec2 tex(u, v);

            verts.push_back(Mesh::initVertex(pos, nor, tex));
        }
    }


    // Define the indices to make triangles from the array of vertices
    unsigned int curStkStart, nxtStkStart;
    for (int i = 0; i < mStackCount; ++i) {
        curStkStart = i * (static_cast<unsigned int>(mSectorCount) + 1);
        nxtStkStart = curStkStart + static_cast<unsigned int>(mSectorCount) + 1;

        for (int j = 0; j < mSectorCount; ++j, ++curStkStart, ++nxtStkStart) {
            // 2 triangles per sector excluding 1st and last stacks
            if (i != 0) {
                indices.push_back(curStkStart);
                indices.push_back(nxtStkStart);
                indices.push_back(curStkStart + 1);
            }

            if (i != (mStackCount - 1)) {
                indices.push_back(curStkStart + 1);
                indices.push_back(nxtStkStart);
                indices.push_back(nxtStkStart + 1);
            }
        }
    }

    // Textures
    Texture tex;
    tex.id = PreRender::createTexture(directoryPath + "/" + filePath);
    tex.path = filePath;
    tex.type = "diffuse";
    textures.push_back(tex);

    mMeshes.push_back(Mesh(verts, indices, textures));
}

Sphere::Sphere(std::string directoryPath, std::string filePath,
	glm::vec3 position, glm::vec3 rotation, glm::vec3 scale,
    std::set<std::string> tags)
	: Sphere(directoryPath, filePath) {
	pos = position;
	rot = rotation;
	scl = scale;
    mTags = tags;
}

float Sphere::getRadius(){
    return mRadius;
}
