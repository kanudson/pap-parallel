#ifndef integrate_h__
#define integrate_h__


typedef double(*IntegrateFunction)(double);


double integrate(IntegrateFunction f, double l, double r, int steps);
double integrateParallel(IntegrateFunction f, double l, double r, int steps);

#endif // integrate_h__