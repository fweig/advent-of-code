# https://adventofcode.com/2022/day/3

def priority(item : Char) : Int32
  if 'A' <= item && item <= 'Z'
    item - 'A' + 27
  else
    item - 'a' + 1
  end
end

def priority_of_duplicate(rucksack : String) : Int32
  fst_compartment_end : Int32 = rucksack.size // 2
  fst_compartment = Set(Char).new rucksack[...fst_compartment_end].each_char
  snd_compartment = Set(Char).new rucksack[fst_compartment_end..].each_char

  intersection = fst_compartment & snd_compartment

  item_in_both_compartments = intersection.to_a[0]

  priority item_in_both_compartments
end

def common_item(rucksacks : Array(String)) : Char
  intersection_between_rucksacks =
    rucksacks
      .each
      .map { |x| Set(Char).new x.each_char }
      .reduce { |x, y| x & y }
      .to_a

  intersection_between_rucksacks[0]
end

input = File.read "input.txt"

# Solution part 1
p! input.each_line.map { |x| priority_of_duplicate x }.sum

# Solution part 2
p! input.each_line.each_slice(3, true).map { |x| priority common_item x }.sum
