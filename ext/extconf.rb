require "mkmf"

custom_cflags = `mysql_config --cflags`.chomp
custom_libs = `mysql_config --libs`.chomp
# custom_libs << " -I /opt/local/include"

with_cflags(custom_cflags) {
  with_ldflags(custom_libs) {
    create_makefile("affinity")
  }
}
