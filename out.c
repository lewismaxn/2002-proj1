#include <stdio.h>



int main(void) {
double y = 12;
double printout0 = y;
if ((printout0 - (int)printout0) == 0) {
printf("%d\n", (int)printout0);
}
else {
printf("%lf\n", printout0);
}
return 0;
}
