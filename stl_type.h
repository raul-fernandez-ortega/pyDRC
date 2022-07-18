#ifndef STL_type
#define STL_type
	
#include <complex>
#include <vector>
using namespace std;

/*typedef struct {
  double realp;
  double imagp;
} DRCcomplex;
*/
typedef complex<double> STLComplex;
typedef vector<double> STLvectorReal;
typedef vector< complex<double> > STLvectorComplex;

#endif
