Gem::Specification.new do |s|
  s.name = %q{affinity}
  s.version = "0.0.1"
  s.authors = ["Chris Mowforth"]
  s.email = ["chris@mowforth.com"]
  s.summary = "Ruby reccomendation system"
  s.files = Dir.glob("{ext,lib,test}/**/*")
  s.extensions << "ext/extconf.rb"
  s.has_rdoc = false
end
