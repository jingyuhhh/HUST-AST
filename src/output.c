#include <stdio.h> 
int _a ; 
long b ; // 
char _d , _e ; 
double c , h ; 
int num[10] ; //dcecwf 
int i ; 
int func ( int a , int b ) { 
    a = 1 ; 
    b = 2l ; 
    c = .14 ; 
    h = 3.13f ; 
    _d = 'x22' ; 
    _e = 't' ; 
    a = 1 + b ; 
    b = 2 + b ; 
    b = c ; 
    if ( a == b ) 
        a = a + b ; 
    else if ( a < b ) { 
        a = a - b ; 
        num[10] = "string" ; 
        if ( a > b ) { 
            a = a + 1 ; 
        } 
    } else { 
        a = a / b ; 
        a = a + 1 ; 
    } 
    if ( a > b ) 
        if ( a > b ) 
        a ++ ; 
    if ( a > b ) { 
        a = ( a + 1 ) * 3 ; 
        b = b + c + 2 ; 
    } 
    do 
    { 
        a ++ ; 
    } while ( a > b ) ; 
    while ( 1 ) { 
        if ( a > b ) { 
            a = ( a + 1 ) * 3 ; 
            b = b + c + 2 ; 
            break ; 
        } 
        if ( a < b ) { 
            continue ; 
        } 
        while ( a == 1 ) { 
            a ++ ; 
        } 
    } 
    for ( i = 0 ; i < a ; i ++ ) { 
        a = a + 1 ; 
    } 
    for ( ; ; i -- ) { 
        a = ( a + 1 ) * 2 ; 
    } 
    do { 
        a = b + c ; 
    } while ( a > 1 ) ; 
    return a + b ; 
} 
/* function def */ 
void def ( ) ; 
