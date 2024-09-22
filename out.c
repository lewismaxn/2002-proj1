#include <stdio.h>

void funcy(double a) {
double b = 10;
double printout0 = b;
if ((printout0 - (int)printout0) == 0) {
printf("%d\n", (int)printout0);
}
else {
printf("%lf\n", printout0);
}
}

double funcer(double value) {
return value;
}



int main(void) {
double y = 10;
double printout1 = y;
if ((printout1 - (int)printout1) == 0) {
printf("%d\n", (int)printout1);
}
else {
printf("%lf\n", printout1);
}
return 0;
}
