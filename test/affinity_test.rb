require "affinity"
require "test/unit"

class AffinityTest < Test::Unit::TestCase
  def test_error_raised_on_initialization_with_incorrect_args
    assert_raise(ArgumentError) do
      Affinity.new()
    end
  end
  
  def test_find_pearson
    a = Affinity.new(1)
    p a.pearson(3, [1,2,3], [1,2,3])
  end
  
  def test_something
    a = Affinity.new(1)
    a.find_similar
  end
end
