#include <libmemcached/memcached.h>
#include <mysql.h>
#include <ruby/ruby.h>
// #include <svm.h>
// #include <gsl/gsl_statistics.h>
#include <math.h>

MYSQL mysql;

static double pearson(int setsize) {
  int n = 9;
  double sum1 = 0.0;
  double sum2 = 0.0;
  double sum1Sq = 0.0;
  double sum2Sq = 0.0;
  double pSum = 0.0;
  
  short set_x[] = {1,2,3,4,5,6,7,8,9};
  short set_y[] = {1,2,3,4,5,6,0,0,9};
  
  int i;
  for(i=0; i<n; i++) {
    short this_x;
    short this_y;
    this_x = set_x[i];
    this_y = set_y[i];
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
    return 0.0;
  } else {
    return (num / den);
  }
}

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
  
  int m_count;
  int n_count;
  
  conn = mysql_init(&mysql);
  
  /* Connect to database */
  if (!mysql_real_connect(conn, server, user, password, database, 0, NULL, 0)) {
    rb_raise(rb_eRuntimeError, "Couldn't connect to the database!");
  }
  
  /////////////////////////////////////////////////////////////////////////////////////
  /* m count */
  if (mysql_query(conn, "select count(id) from users")) {
    rb_raise(rb_eRuntimeError, mysql_error(conn));
  }
  res = mysql_use_result(conn);
  row = mysql_fetch_row(res);
  sscanf(row[0], "%d", &m_count);
     
  mysql_free_result(res);
  
  
  /////////////////////////////////////////////////////////////////////////////////////
  /* n count */
  if (mysql_query(conn, "select count(id) from products")) {
    rb_raise(rb_eRuntimeError, mysql_error(conn));
  }
  
  res = mysql_use_result(conn);
  row = mysql_fetch_row(res);
  sscanf(row[0], "%d", &n_count);
  
  mysql_free_result(res);
  
  
  // Close connection
  mysql_close(conn);
  
  // Empty m x n matrix
  int m_n_matrix[m_count][n_count];

  return Qtrue;
}

// Begin method definitions
VALUE rubyAffinity;

void Init_affinity() {
  rubyAffinity = rb_define_class("Affinity", rb_cObject);
  
  rb_define_method(rubyAffinity, "initialize", affinity_initialize, 1);
  rb_define_method(rubyAffinity, "find_similar", affinity_find_similar, 0);
}
