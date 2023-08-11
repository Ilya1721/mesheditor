#pragma once

#include "MeshCore/Object3D.h"

const MeshCore::Vertex defaultVertex(glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f));

MeshCore::Mesh getDefaultMesh();
MeshCore::Object3D getDefaultObject3D();
MeshCore::Mesh loadCube();
