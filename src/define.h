#pragma once

#define CLAMP(LO, X, HI) (MIN(MAX(LO, X), HI))
#define MIN(A, B) ((A < B) ? A : B)
#define MAX(A, B) ((A > B) ? A : B)

#define let constexpr static
