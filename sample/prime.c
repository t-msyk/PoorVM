#include<stdio.h>
int main ( void ) {
  int prime[100]={2};
  int i=1;
  int n,p;
  for ( n=3; n<100; n+=2 ) {
    for ( p=3; p*p<n+1; p+=2 ) {
      if ( n % p == 0 ) goto NotPrime;
    }
    prime[i++] = n;
    NotPrime:
    ;
  }
  for ( n=0; n<i; n++ ) printf("%d\n",prime[n]);
  return 0;
}
