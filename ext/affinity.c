#include <libmemcached/memcached.h>
#include <mysql.h>
#include <ruby/ruby.h>
// #include <svm.h>
// #include <gsl/gsl_statistics.h>
#include <math.h>

MYSQL mysql;

static VALUE affinity_initialize(VALUE self, VALUE target_id) {
    rb_iv_set(self, "@target", target_id);
}

static VALUE affinity_find_similar(VALUE self) {
  MYSQL *conn;
  MYSQL_RES *res;
  MYSQL_ROW row;
  
  char *server = "localhost";
  char *user = "root";
  char *password = NULL;
  char *database = "similar";
  
  conn = mysql_init(&mysql);
  
  /* Connect to database */
  if (!mysql_real_connect(conn, server, user, password, database, 0, NULL, 0)) {
    fprintf(stderr, "%s\n", mysql_error(conn));
    exit(1);
  }
  
  /* send SQL query */
  if (mysql_query(conn, "select count(*) from orders")) {
    rb_raise(rb_eRuntimeError, mysql_error(conn));
    exit(1);
  }
  res = mysql_use_result(conn);
  
  /* output table name */
  while ((row = mysql_fetch_row(res)) != NULL)
     printf("%s \n", row[0]);

  row = mysql_fetch_row(res);
   
  mysql_free_result(res);
  
  mysql_close(conn);
  
  return Qtrue;
  
  // double ds1[4] = {1,2,3,4};
  // double ds2[4] = {1,2,3,4};
  // double r;
  // r = gsl_stats_sd(ds1, 1,4);
// return Qtrue;
}

int wibble() {
  return 1;
}

static VALUE affinity_pearson(VALUE self, VALUE setsize, VALUE set_x, VALUE set_y) {
  int n = NUM2INT(setsize);
  double sum1 = 0.0;
  double sum2 = 0.0;
  double sum1Sq = 0.0;
  double sum2Sq = 0.0;
  double pSum = 0.0;

  // VALUE *x_a = RARRAY_PTR(set_x);
  // VALUE *y_a = RARRAY_PTR(set_y);
  
  int x_a[] = {1,2,3};
  int y_a[] = {1,2,3};
  
  int i;
  for(i=0; i<n; i++) {
    double this_x;
    double this_y;
    this_x = x_a[i];
    this_y = y_a[i];
    // this_x = NUM2DBL(x_a[i]);
    // this_y = NUM2DBL(y_a[i]);
    sum1 += this_x;
    sum2 += this_y;
    sum1Sq += pow(this_x, 2);
    sum2Sq += pow(this_y, 2);
    pSum += this_y * this_x;
  }
  
  double num;
  double den;
  
  num = pSum - ( ( sum1 * sum2 ) / n );
  den = sqrt( ( sum1Sq - ( pow(sum1, 2) ) / n ) * 
        ( sum2Sq - ( pow(sum2, 2) ) / n ) );
        
  if(den == 0){
    // return 0.0;
    return Qtrue;
  } else {
    // puts(num / den);
    // return DOUBLE2NUM(num);
    return Qfalse;
  }
}

// Begin method definitions
VALUE rubyAffinity;

void Init_affinity() {
  rubyAffinity = rb_define_class("Affinity", rb_cObject);
  
  rb_define_method(rubyAffinity, "initialize", affinity_initialize, 1);
  rb_define_method(rubyAffinity, "pearson", affinity_pearson, 3);
  rb_define_method(rubyAffinity, "find_similar", affinity_find_similar, 0);
}
