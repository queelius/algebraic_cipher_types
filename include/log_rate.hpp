#include <cmath>

using std::pow;

struct log_rate
{
    float k;
    operator float() const { return k; };
};

long double to_double(log_rate x)
{
    return pow((long double)2,(long double)k);
}

log_rate operator*(log_rate x, log_rate y) { return log_rate{x.k + y.k}; }
log_rate operator/(log_rate x, log_rate y) { return log_rate{x.k - y.k}; }

bool operator<(log_rate x, log_rate y) { return x.k < y.k; }
bool operator<=(log_rate x, log_rate y) { return x.k <= y.k; }
bool operator==(log_rate x, log_rate y) { return x.k == y.k; }
bool operator!=(log_rate x, log_rate y) { return x.k != y.k; }
bool operator>(log_rate x, log_rate y) { return x.k > y.k; }
bool operator>=(log_rate x, log_rate y) { return x.k >= y.k; }
