#include "mex.h"
#include "drakeGeometryUtil.h"

using namespace Eigen;
using namespace std;

void mexFunction(int nlhs, mxArray* plhs[], int nrhs, const mxArray* prhs[])
{
  if(nrhs != 1)
  {
    mexErrMsgTxt("Incorrect usage, quat2expmex(q)");
  }
  Map<Vector4d> q(mxGetPr(prhs[0]));
  int gradient_order = nlhs==2?1:0;
  GradientVar<double,3,1> ret = quat2exp(q,gradient_order);
  plhs[0] = mxCreateDoubleMatrix(3,1,mxREAL);
  memcpy(mxGetPr(plhs[0]),ret.value().data(),sizeof(double)*3);
  if(nlhs>1)
  {
    plhs[1] = mxCreateDoubleMatrix(3,4,mxREAL);
    memcpy(mxGetPr(plhs[1]),ret.gradient().value().data(),sizeof(double)*12);
  }
}
