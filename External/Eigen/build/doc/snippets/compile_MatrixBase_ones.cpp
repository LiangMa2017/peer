#include <Eigen/Core>
#include <Eigen/Array>
#include <Eigen/LU>
#include <Eigen/Cholesky>
#include <Eigen/Geometry>

using namespace Eigen;
using namespace std;

int main(int, char**)
{
  cout << Matrix2d::Ones() << endl;
cout << 6 * RowVector4i::Ones() << endl;

  return 0;
}
