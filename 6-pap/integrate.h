#ifndef integrate_h__
#define integrate_h__


typedef double(*IntegrateFunction)(double);


double integrate(IntegrateFunction f, double l, double r, int steps);
double integrateOMP(IntegrateFunction f, double l, double r, int steps);

//  quadrant inline
double integrateInline(double l, double r, int steps);
double integrateSimdInline(double l, double r, int steps);

#endif // integrate_h__
