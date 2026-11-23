#include "cputimedelta.h"

#include <cmath>
#include <cstdio>

int main()
{
    // Synthetic tick deltas matching the reported rounded percentages.
    // Nice contributes 82.82%, and user plus system contribute 17.14%.
    const CpuTimeDelta sample{1500, 8282, 214, 4};
    const float fraction = sample.busyFraction();
    if (sample.total() != 10000 || !(std::abs(fraction - 0.9996f) <= 0.000001f))
    {
        std::fprintf(stderr, "Expected 9996 busy ticks out of 10000, got %.4f%% busy over %ld ticks\n",
                     fraction * 100, sample.total());
        return 1;
    }

    if (CpuTimeDelta{0, 100, 0, 0}.busyFraction() != 1.0f ||
        CpuTimeDelta{0, 0, 0, 0}.busyFraction() != 0.0f)
    {
        std::fputs("Expected 100% for all-nice time and 0% for an empty sample\n", stderr);
        return 1;
    }
}
