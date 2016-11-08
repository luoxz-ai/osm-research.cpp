#include <iostream>
#include <cmath>

#define rad2deg(a) ((a)/M_PI * 180.0) /* rad を deg に換算するマクロ関数 */
#define deg2rad(a) ((a)/180.0 * M_PI) /* deg を rad に換算するマクロ関数 */

using namespace std;

class Common {
public:
  static double DistNN(Node a, Node b) {
    double PI = 3.141592653589793;
    double P = ( a.lat+b.lat ) / 2 * PI / 180;
    double dP = ( a.lat-b.lat ) * PI / 180;
    double dR = ( a.lon-b.lon ) * PI / 180;
    double M = 6334834 / sqrt( ( 1-0.006674 * sin( P ) * sin( P ) ) * ( 1-0.006674 * sin( P ) * sin( P ) ) * ( 1-0.006674 * sin( P ) * sin( P ) ) );
    double N = 6377397 / sqrt( 1-0.006674 * sin( P ) * sin( P ) );
    double D = sqrt( ( M * dP ) * ( M * dP ) + ( N * cos( P ) * dR ) * ( N * cos( P ) * dR ) );
    return D;
  }
};
