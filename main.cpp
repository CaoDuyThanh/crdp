#include <iostream>
#include "src/rdp.h"

int main() {
    auto a = std::vector<P2D>();
    a.push_back(P2D{0.0, 5.0});
    a.push_back(P2D{4.0, 7.0});
    a.push_back(P2D{10.0, 5.0});
    a.push_back(P2D{15.0, 5.0});
    auto result = rdp(a, 4);
    for (P2D p: result) {
        std::cout << p[0] << " " << p[1] << "\n";
    }
 }