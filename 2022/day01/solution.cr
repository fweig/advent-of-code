# https://adventofcode.com/2022/day/1

caloriesPerElf = Array(Int32).new
caloriesPerElf.push 0

input = File.read "input.txt"
input.each_line { |foodCalories|
  unless foodCalories.empty?
    caloriesPerElf[-1] += foodCalories.to_i
  else
    caloriesPerElf.push 0
  end
}

# Sort elfs by ascending order of calories.
# => Elf with most calories is at end of the array
caloriesPerElf = caloriesPerElf.sort

# Solution part 1
p! caloriesPerElf[-1]

# Solution part 2
p! caloriesPerElf[-1] + caloriesPerElf[-2] + caloriesPerElf[-3]
