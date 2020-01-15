#pragma once
#include "thirdparty/glm/ext.hpp"
#include "thirdparty/glm/glm.hpp"
#include "thirdparty/glm/gtx/matrix_decompose.hpp"

template <class T, int dimension> using Vector = glm::vec<dimension, T, glm::defaultp>;

using Vector2 = Vector<float, 2>;
using Vector3 = Vector<float, 3>;
using Vector4 = Vector<float, 4>;
using Vector2i = Vector<int, 2>;
using Vector3i = Vector<int, 3>;
using Vector4i = Vector<int, 4>;

template <class T, int cdimension, int rdimension> using Matrix = glm::mat<cdimension, rdimension, T, glm::defaultp>;

using Matrix2 = Matrix<float, 2, 2>;
using Matrix3 = Matrix<float, 3, 3>;
using Matrix4 = Matrix<float, 4, 4>;
using Matrix2i = Matrix<int, 2, 2>;
using Matrix3i = Matrix<int, 3, 3>;
using Matrix4i = Matrix<int, 4, 4>;

template <class T> using Quat = glm::qua<T, glm::defaultp>;
using Quaternion = Quat<float>;

namespace lm
{
using namespace glm;

const Vector3 kWorldX{1.0F, 0.0F, 0.0F};
const Vector3 kWorldY{0.0F, 1.0F, 0.0F};
const Vector3 kWorldZ{0.0F, 0.0F, 1.0F};
const Vector3 kWorldRight{1.0F, 0.0F, 0.0F};
const Vector3 kWorldUp{0.0F, 1.0F, 0.0F};
const Vector3 kWorldFront{0.0F, 0.0F, -1.0F};
const Vector3 kWorldZero{0.0F, 0.0F, 0.0F};
const Vector3 kWorldOne{1.0F, 1.0F, 1.0F};

const Vector3 kRed{1.0F, 0.0F, 0.0F};
const Vector3 kGreen{0.0F, 1.0F, 0.0F};
const Vector3 kBlue{0.0F, 0.0F, 1.0F};
const Vector3 kBlack{0.0F, 0.0F, 0.0F};
const Vector3 kWhite{1.0F, 1.0F, 1.0F};

} // namespace lm