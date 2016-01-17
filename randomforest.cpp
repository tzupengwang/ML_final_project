//by tzupengwang™
#include<bits/stdc++.h>
using namespace std;

#define FO(it,c) for (__typeof((c).begin()) it=(c).begin();it!=(c).end();it++)
typedef long long ll;
typedef pair<int,int> ii;

double sigm( int x ) {
  return 1.0 / ( 1.0 + exp( -x ) ) ;
}
double sqr( double x ) { return x * x ; }
struct data {
  double x[ 17 ] ;
  int y ;
  int index ;
  data( double a[] , int c , int b ) : y( c ) , index( b ) {
    for ( int i = 0 ; i < 17 ; i ++ ) x[ i ] = a[ i ] ;
  }
} ;

struct node {
  vector< data > v ;
  bool isleaf ;
  int yn ;
  int pdim ;
  double theta ;
  node *ch[ 2 ] ;
  node() : isleaf( false ) {}
  void clean() {
    if ( isleaf ) return ;
    ch[ 0 ]->clean() ;
    ch[ 1 ]->clean() ;
    delete ch[ 0 ] ;
    delete ch[ 1 ] ;
  }
  void msort( int i ) {
    sort( v.begin() , v.end() , [&]( data x , data y ) {
      return x.x[ i ] < y.x[ i ] ;
    } ) ;
  }
  double gini( int dim , double prt ) {
    int cnt[ 2 ][ 3 ] ;
    memset( cnt , 0 , sizeof cnt ) ;
    for ( data x : v ) {
      if ( x.x[ dim ] < prt ) cnt[ 0 ][ x.y > 0 ? 1 : 0 ] ++ ;
      else cnt[ 1 ][ x.y > 0 ? 1 : 0 ] ++ ;
    }
    cnt[ 0 ][ 2 ] = cnt[ 0 ][ 0 ] + cnt[ 0 ][ 1 ] ;
    cnt[ 1 ][ 2 ] = cnt[ 1 ][ 0 ] + cnt[ 1 ][ 1 ] ;
    double g1 = ( cnt[ 0 ][ 2 ] == 0 ) ? 1 : ( 1 - sqr( (double)cnt[ 0 ][ 0 ] / cnt[ 0 ][ 2 ] ) - sqr( (double)cnt[ 0 ][ 1 ] / cnt[ 0 ][ 2 ] ) ) ;
    double g2 = ( cnt[ 1 ][ 2 ] == 0 ) ? 1 : ( 1 - sqr( (double)cnt[ 1 ][ 0 ] / cnt[ 1 ][ 2 ] ) - sqr( (double)cnt[ 1 ][ 1 ] / cnt[ 1 ][ 2 ] ) ) ;
    // cout << v.size() << "   " << ( g1 * cnt[ 0 ][ 2 ] + g2 * cnt[ 1 ][ 2 ] ) / v.size() << endl ;
    return ( g1 * cnt[ 0 ][ 2 ] + g2 * cnt[ 1 ][ 2 ] ) / v.size() ;
  }
  void findbest() {
    bool pure = true , dif = true ;
    for ( int i = 1 ; i < (int)v.size() ; i ++ ) {
      if ( v[ i - 1 ].y != v[ i ].y ) pure = false ;
      if ( v[ i - 1 ].x[ 0 ] != v[ i ].x[ 0 ] ) dif = false ;
      if ( v[ i - 1 ].x[ 1 ] != v[ i ].x[ 1 ] ) dif = false ;
    }
    if ( pure || dif ) {
      isleaf = true ;
      int y[ 3 ]{ 0 , 0 } ;
      for ( data x : v ) y[ x.y > 0 ? 1 : 0 ] ++ ;
      if ( y[ 0 ] > y[ 1 ] ) yn = -1 ;
      else yn = 1 ;
      return ;
    }
    double mini = 1.0 ;
    for ( int k = 0 ; k < 17 ; k ++ ) {
      msort( k ) ;
      for ( int i = 0 ; i < (int)v.size() ; i ++ ) {
        double th ;
        if ( i == 0 ) th = -10 ;
        else th = ( v[ i - 1 ].x[ k ] + v[ i ].x[ k ] ) / 2 ;
        double tmp = gini( k , th ) ;
        if ( tmp < mini ) {
          pdim = k , theta = th , mini = tmp ;
        }
      }
    }
  }
} *root[ 30005 ] ;

vector< data > vt , trn ;

void init() {
  FILE *train = fopen( "./sample_train_x.csv" , "r" ) ;
  FILE *trainy = fopen( "./truth_train.csv" , "r" ) ;
  char mstr[ 1000 ] ;
  fgets( mstr , 1000 , train ) ;
  for ( int i = 0 ; i < 96434 ; i ++ ) {
    double a[ 20 ] ;
    int ind ;
    fscanf( train , "%[^,]%*c" , mstr ) ;
    ind = atoi( mstr ) ;
    for ( int j = 0 ; j < 17 ; j ++ ) {
      if ( j < 16 ) {
        fscanf( train , "%[^,]%*c" , mstr ) ;
        int tmp = atoi( mstr ) ;
        a[ j ] = tmp ;
      } else fscanf( train , "%lf" , &a[ j ] ) ;
    }
    int yy ;
    fscanf( trainy , "%d,%d" , &ind , &yy ) ;
    trn.emplace_back( a , ( yy == 1 ) ? 1 : -1 , ind ) ;
  }
  fclose( train ) ;
  fclose( trainy ) ;
  FILE *test = fopen( "./sample_test_x.csv" , "r" ) ;
  fgets( mstr , 1000 , test ) ;
  for ( int i = 0 ; i < 24108 ; i ++ ) {
    double a[ 20 ] ;
    int ind ;
    fscanf( test , "%[^,]%*c" , mstr ) ;
    ind = atoi( mstr ) ;
    for ( int j = 0 ; j < 17 ; j ++ ) {
      if ( j < 16 ) {
        fscanf( test , "%[^,]%*c" , mstr ) ;
        int tmp = atoi( mstr ) ;
        a[ j ] = tmp ;
      } else fscanf( test , "%lf" , &a[ j ] ) ;
    }
    vt.emplace_back( a , 0 , ind ) ;
  }
  fclose( test ) ;
  /*
  for ( int i = 0 ; i < 100 ; i ++ ) {
    printf( "%lf %lf %d\n" , root->v[ i ].x[ 0 ] , root->v[ i ].x[ 1 ] , root->v[ i ].y ) ;
  }
  */
}

void build( node *now ) {
  now->findbest() ;
  if ( now->isleaf ) return ;
  now->ch[ 0 ] = new node ;
  now->ch[ 1 ] = new node ;
  for ( data x : now->v ) {
    if ( x.x[ now->pdim ] < now->theta ) now->ch[ 0 ]->v.push_back( x ) ;
    else now->ch[ 1 ]->v.push_back( x ) ;
  }
  build( now->ch[ 0 ] ) ;
  build( now->ch[ 1 ] ) ;
}

int pred( data x , int treeid ) {
  node *now = root[ treeid ] ;
  while ( !now->isleaf ) {
    if ( x.x[ now->pdim ] < now->theta ) now = now->ch[ 0 ] ;
    else now = now->ch[ 1 ] ;
  }
  return now->yn ;
}

int tans[ 30000 ] ;

void process() {
  for ( int k = 0 ; k < 1000 ; k ++ ) {
    root[ k ] = new node ;
    for ( int i = 0 ; i < 100 ; i ++ ) {
      root[ k ]->v.push_back( trn[ rand() % 96434 ] ) ;
    }
    build( root[ k ] ) ;
    for ( int i = 0 ; i < 24108 ; i ++ )
      tans[ i ] += pred( vt[ i ] , k ) ;
    //printf( "%f\n" , (double)err / 1000 ) ;
  }
  // int err = 0 ;
  for ( int i = 0 ; i < 24108 ; i ++ ) {
    int ans = tans[ i ] ;
    if ( ans > 0 ) ans = 1 ;
    else ans = 0 ;
    vt[ i ].y = ans ;
  }
  for ( int i = 0 ; i < 24108 ; i ++ ) printf( "%d,%d\n" , vt[ i ].index , vt[ i ].y ) ;
}

int main() {
  srand( time( NULL ) ) ;
  init() ;
  process() ;
  return 0 ;
}

