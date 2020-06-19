#pragma once

#include <execution>

namespace Grid
{

inline bool to_parallelize = false;

void setParallelizeFlag(bool v) { to_parallelize = v; }
bool getParallelizeFlag() { return to_parallelize; }

void parallelize() { to_parallelize = true; }
void sequencialize() { to_parallelize = false; }

}  // namespace Grid
