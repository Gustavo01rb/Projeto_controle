#include "config.hpp"
#include <math.h>

double gaussian(const int x,const double const_param[2]){
    double k = const_param[0];
    double m = const_param[1];
    k = k/2;
    double expoent = (-1)*((x-m)*(x-m))/(k*k);
    return exp(expoent);
  }
  
  int fuzzy(int err){
    const double ant_param[3][2] = {{boundarieFuzzy,-boundarieFuzzy},{boundarieFuzzy,0},{boundarieFuzzy,boundarieFuzzy}};
    const double cons_param[3][2] = {{limitVoltage,-limitVoltage+2},{limitVoltage,0},{limitVoltage,limitVoltage-2}};
    
    double w[3];
    for(int i =0; i < 3 ; ++i){
        w[i] = gaussian(err,ant_param[i]);
    }
    double c[3][2*limitVoltage+1];
    double agreg_out [2*limitVoltage+1];
    for(int i =0;i<2*limitVoltage+1;++i){
        double yw1 = gaussian(i-limitVoltage,cons_param[0]);
        double yw2 = gaussian(i-limitVoltage,cons_param[1]);
        double yw3 = gaussian(i-limitVoltage,cons_param[2]);
        c[0][i] = fmin(w[0],yw1);
        c[1][i] = fmin(w[1],yw2);
        c[2][i] = fmin(w[2],yw3);
        double m = fmax(c[0][i],c[1][i]);
        agreg_out[i] =  fmax(m,c[2][i]);
    }
    double sumN=0;
    double sumD=0;
    for(int i =0;i < 2*limitVoltage+1;++i){ //Centroid
        sumN+=(i-limitVoltage)*agreg_out[i];
        sumD+=agreg_out[i];
    }
    float r = sumN/sumD; 

    return (int)map(r,-limitVoltage,limitVoltage,0,127);
  }
