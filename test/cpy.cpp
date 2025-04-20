#include <cpy.hpp>

using namespace hpmem;

int main() {
    char* src = new char[130];
    __builtin_memset(src, 'a', 129);
    char* dst = new char[130];

    hpcpy(dst, src, 129);

    return 0;
}
