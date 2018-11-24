/*
 * Function:  calc_DI_temp
 * --------------------
 * calculates DI from its formula
 *
 *  avgC: average temperature variable
 *  avgH: average humidity variable
 *
 *  returns: DI
 */

float calc_DI_temp(float avgC, float avgH){
   return round(avgC - 0.55 * (1 - 0.01 * avgH) * (avgC - 14.5));
}

/*
 * Function:  calc_DI
 * --------------------
 * calculates DI
 *
 *  temp: average temperature variable
 *
 *  returns: DI (1-6)
 */

int calc_DI(int temp){
  if (temp<=21)
    return 1;
  else if (temp<=24)
    return 2;
  else if (temp<=27)
    return 3;
  else if (temp<=29)
    return 4;
  else if (temp<=32)
    return 5;
  else if (temp>32)
    return 6;
}
