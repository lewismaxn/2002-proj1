#include <stdio.h>



int main(void) {
double x = 10;
double printout = x;
if ((printout - (int)printout) == 0) {
printf("%d", (int)printout);
}
else {
printf("%lf", printout);
}
return 0;
}