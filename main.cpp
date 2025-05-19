#include "fraction.hpp"
using namespace std;

int main() {
    Fraction x(1, 3);
    Fraction result = x * Fraction(3);
    cout << result << endl;        
    return 0;
}
