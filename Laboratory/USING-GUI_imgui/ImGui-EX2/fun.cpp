#include <iostream>
#include <tuple>
#include <algorithm>
#include <vector>


/* ----------------------- factorial ----------------------- */
int factorial(int x){
    int ans = x; 
    for(int i=2; i<x; i++) ans *= i;
    return ans;
}
/* ----------------------- factorial  ----------------------- */



/* ----------------------- fibonacci  ----------------------- */
std::vector<int> fibo(int limit){
    std::vector<int> ans;
    int a = 0 , b = 1;
    ans.push_back(a);
    for (int i=0; b <= limit; i++){
        ans.push_back(b);
        int aux = b;
        b += a; a = aux;
    }
    return ans;
}
/* ----------------------- fibonacci  ----------------------- */



/* ----------------------- extended euclidean algorithm ----------------------- */
std::tuple<int,int,int> extended_euclidean(int a, int b) {

    int r0 = a>b? a:b;
    int r1 = r0==a? b:a;
    int s0=1, s1=0, t0=0, t1=1;

    while (r1 != 0) {
        int r = r0 % r1;
        int q = int(r0/r1);
        int s = s0 - q * s1;
        int t = t0 - q * t1;

        r0 = r1;
        r1 = r;
        s0 = s1;
        s1 = s;
        t0 = t1;
        t1 = t;
    }

    return std::make_tuple(r0, s0, t0);
}

int inverse_mod(int n, int m) {
    int mcd, s, t;
    std::tie(mcd, s, t) = extended_euclidean(n, m);
    if (mcd != 1)
        throw std::overflow_error("GCD is not one, inverse modular doesn't exist\n");
    return n<m? t%m:s%m;
}
/* ----------------------- extended euclidean algorithm ----------------------- */