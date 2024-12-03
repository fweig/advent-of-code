# https://adventofcode.com/2022/day/4

DO_DEBUG = false

def Bool.to_i(b : Bool) : Int32
  if b
    1
  else
    0
  end
end

struct ElfSections
  def initialize(@start : Int32, @end : Int32)
  end

  def contains?(other : ElfSections) : Bool
    @start <= other.@start && @end >= other.@end
  end

  def overlaps(other : ElfSections) : Bool
    (@start <= other.@start && other.@start <= @end) || (@start <= other.@end && other.@end <= @end)
  end
end

def ElfSections.parse(range : String) : ElfSections
  first, last = range.split('-')
  ElfSections.new first.to_i, last.to_i
end

def ElfSections.parsePair(ranges : String) : Tuple(ElfSections, ElfSections)
  first, second = ranges.split(',')
  Tuple.new (ElfSections.parse first), (ElfSections.parse second)
end

input = File.read(if DO_DEBUG
  "test_input.txt"
else
  "input.txt"
end)

# Solution part 1
p! input
  .each_line
  .map { |line|
    first, second = ElfSections.parsePair line
    first.contains?(second) | second.contains?(first)
  }
  .map { |b| Bool.to_i b }
  .sum

# Solution part 2
p! input
  .each_line
  .map { |line|
    first, second = ElfSections.parsePair line
    first.overlaps(second) | second.overlaps(first)
  }
  .map { |b| Bool.to_i b }
  .sum
