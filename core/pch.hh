#ifndef CORE_PCH_HH
#define CORE_PCH_HH
#pragma once

#include <cassert>
#include <cctype>
#include <cinttypes>
#include <cmath>
#include <cstdarg>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <cstring>

#include <algorithm>
#include <array>
#include <chrono>
#include <filesystem>
#include <format>
#include <fstream>
#include <iostream>
#include <limits>
#include <mutex>
#include <ranges>
#include <sstream>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

#include <Eigen/Eigen>

#include <parson.h>

#include <spdlog/spdlog.h>

#include <spdlog/sinks/stdout_color_sinks.h>

using Vector2f = Eigen::Vector2f;
using Vector3f = Eigen::Vector3f;
using Vector4f = Eigen::Vector4f;

using Hyperplane3f = Eigen::Hyperplane<float, 3>;

using Matrix3f = Eigen::Matrix3f;
using Matrix4f = Eigen::Matrix4f;

#endif // CORE_PCH_HH
