#ifndef CPUTIMEDELTA_H
#define CPUTIMEDELTA_H

struct CpuTimeDelta
{
    long user;
    long nice;
    long system;
    long idle;

    long busy() const { return user + nice + system; }
    long total() const { return busy() + idle; }

    float busyFraction() const
    {
        const long elapsed = total();
        return elapsed == 0 ? 0.0f : static_cast<float>(busy()) / elapsed;
    }
};

#endif
