// bbpr.c -- Finds all roots of polynomial by first finding quadratic
//             factors using Bairstow's method, then extracting roots
//             from quadratics. Implements new algorithm for managing
//             multiple roots.
//
//  (C) 2002, 2003, C. Bond. All rights reserved.
//

#include "bbpr.h"
#define maxiter 500

int roots(double *a,int n,double *wr,double *wi)
{
    double sq,b2,c,disc;
    int m,numroots;

    m = n;
    numroots = 0;
    while (m > 1) {
        b2 = -0.5*a[m-2];
        c = a[m-1];
        disc = b2*b2-c;
        if (disc < 0.0) {                   // complex roots
            sq = sqrt(-disc);
            wr[m-2] = b2;
            wi[m-2] = sq;
            wr[m-1] = b2;
            wi[m-1] = -sq;
            numroots+=2;
        }
        else {                              // real roots
            sq = sqrt(disc);
            wr[m-2] = fabs(b2)+sq;
            if (b2 < 0.0) wr[m-2] = -wr[m-2];
            if (wr[m-2] == 0)
                wr[m-1] = 0;
            else {
                wr[m-1] = c/wr[m-2];
                numroots+=2;
            }
            wi[m-2] = 0.0;
            wi[m-1] = 0.0;
        }
        m -= 2;
    }
    if (m == 1) {
       wr[0] = -a[0];
       wi[0] = 0.0;
       numroots++;
    }
    return numroots;
}

void deflate(double *a,int n,double *b,double *quad,double *err)
{
    double r,s;
    int i;

    r = quad[1];
    s = quad[0];

    b[1] = a[1] - r;

    for (i=2;i<=n;i++){
        b[i] = a[i] - r * b[i-1] - s * b[i-2];
    }
    *err = fabs(b[n])+fabs(b[n-1]);
}

void find_quad(double *a,int n,double *b,double *quad,double *err, int *iter)
{
    double *c,dn,dr,ds,drn,dsn,eps,r,s;
    int i;

    c = malloc((n+1) * sizeof(*c));
    c[0] = 1.0;
    r = quad[1];
    s = quad[0];
    eps = 1e-15;
    *iter = 1;

    do {
        if (*iter > maxiter) break;
        if (((*iter) % 200) == 0) {
            eps *= 10.0;
        }
        b[1] = a[1] - r;
        c[1] = b[1] - r;

        for (i=2;i<=n;i++){
            b[i] = a[i] - r * b[i-1] - s * b[i-2];
            c[i] = b[i] - r * c[i-1] - s * c[i-2];
        }
        dn=c[n-1] * c[n-3] - c[n-2] * c[n-2];
        drn=b[n] * c[n-3] - b[n-1] * c[n-2];
        dsn=b[n-1] * c[n-1] - b[n] * c[n-2];

        if (fabs(dn) < 1e-10) {
            if (dn < 0.0) dn = -1e-8;
            else dn = 1e-8;
        }
        dr = drn / dn;
        ds = dsn / dn;
        r += dr;
        s += ds;
        (*iter)++;
    } while ((fabs(dr)+fabs(ds)) > eps);
    quad[0] = s;
    quad[1] = r;
    *err = fabs(ds)+fabs(dr);
    free(c);
}

void diff_poly(double *a,int n,double *b)
{
    double coef;
    int i;

    coef = (double)n;
    b[0] = 1.0;
    for (i=1;i<n;i++) {
        b[i] = a[i]*((double)(n-i))/coef;            
    }
}

void recurse(double *a,int n,double *b,int m,double *quad,
    double *err,int *iter)
{
    double *c,*x,rs[2],tst;

    if (fabs(b[m]) < 1e-16) m--;    // this bypasses roots at zero
    if (m == 2) {
        quad[0] = b[2];
        quad[1] = b[1];
        *err = 0;
        *iter = 0;
        return;
    }
    c = malloc((m+1)*sizeof(*c));
    x = malloc((n+1)*sizeof(*x));
    c[0] = x[0] = 1.0;
    rs[0] = quad[0];
    rs[1] = quad[1];
    *iter = 0;
    find_quad(b,m,c,rs,err,iter);
    tst = fabs(rs[0]-quad[0])+fabs(rs[1]-quad[1]);
    if (*err < 1e-12) {
        quad[0] = rs[0];
        quad[1] = rs[1];
    }
// tst will be 'large' if we converge to wrong root
    if (((*iter > 5) && (tst < 1e-4)) || ((*iter > 20) && (tst < 1e-1))) {
        diff_poly(b,m,c);
        recurse(a,n,c,m-1,rs,err,iter);
        quad[0] = rs[0];
        quad[1] = rs[1];
    }
    free(x);
    free(c);
}

void get_quads(double *a,int n,double *quad,double *x)
{
    double *b,*z,err,tmp;
    int iter,i,m;

    if ((tmp = a[0]) != 1.0) {
        a[0] = 1.0;
        for (i=1;i<=n;i++) {
            a[i] /= tmp;
        }
    }
    if (n == 2) {
        x[0] = a[1];
        x[1] = a[2];
        return;
    }
    else if (n == 1) {
        x[0] = a[1];
        return;
    }
    m = n;
    b = malloc((n+1)*sizeof(*b));
    z = malloc((n+1)*sizeof(*z));
    b[0] = 1.0;
    for (i=0;i<=n;i++) {
        z[i] = a[i];
        x[i] = 0.0;
    }
    do {            
        if (n > m) {
            quad[0] = 3.14159e-1;
            quad[1] = 2.78127e-1;
        }
        do {                    // This loop tries to assure convergence
            for (i=0;i<5;i++) { 
                find_quad(z,m,b,quad,&err,&iter);
                if ((err > 1e-7) || (iter > maxiter)) {
                    diff_poly(z,m,b);
                    recurse(z,m,b,m-1,quad,&err,&iter);
                }
                deflate(z,m,b,quad,&err);
                if (err < 0.001) break;
                // quad[0] = random(8) - 4.0;
                // quad[1] = random(8) 
                quad[0] = rand()%8 - 4.0;
                quad[1] = rand()%8 - 4.0;
            }
            if (err > 0.01) {
                // cout << "Error! Convergence failure in quadratic x^2 + r*x + s." << endl;
                // cout << "Enter new trial value for 'r': ";
                // cin >> quad[1];
                // cout << "Enter new trial value for 's' ( 0 to exit): ";
                // cin >> quad[0];
                // if (quad[0] == 0) exit(1);
                exit(1);
            }
        } while (err > 0.01);
        x[m-2] = quad[1];
        x[m-1] = quad[0];
        m -= 2;
        for (i=0;i<=m;i++) {
            z[i] = b[i];
        }
    } while (m > 2);
    if (m == 2) {
        x[0] = b[1];
        x[1] = b[2];
    }
    else x[0] = b[1];
    free(z);
    free(b);
}

double findRoot(double coeffs[], int n) {
    double x[n+1];
    double wr[n+1];
    double wi[n+1];
    double quad[2];

    // initialize estimate for 1st root pair 
    quad[0] = 2.71828e-1;
    quad[1] = 3.14159e-1;
    
    // get roots
    get_quads(coeffs, n, quad, x);
    
    // Return only the (supposed to be unique) real positive root
    for (int i = 0; i < n;i++) {
        if ((wr[i] >= 0.0) && (wi[i] == 0.0))
            return wr[i];
    }

    return 0;
}
