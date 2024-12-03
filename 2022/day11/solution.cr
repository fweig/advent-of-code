# https://adventofcode.com/2022/day/11

DO_TEST = false

class Monkey
  property items : Array(UInt128)
  property monkey1 : UInt128
  property monkey2 : UInt128
  property update : Proc(UInt128, UInt128)
  property divisor : UInt128
  property n_items_inspected : UInt128 = 0
  getter items, monkey1, monkey2, update, divisor, n_items_inspected
  private setter n_items_inspected

  def initialize(@items, @monkey1, @monkey2, @update, @divisor)
  end

  def do_round(monkeys : Array(Monkey), relief : Proc(UInt128, UInt128))
    items.each { |worry_level|
      self.n_items_inspected += 1
      worry_level = update.call(worry_level)
      worry_level = relief.call(worry_level)

      next_monkey = worry_level % divisor == 0 ? monkey1 : monkey2
      monkeys[next_monkey].items.push worry_level
    }
    items.clear
  end

  def clone
    Monkey.new items.clone, monkey1, monkey2, update, divisor
  end
end

def do_rounds(monkeys : Array(Monkey), n : UInt128, relief : Proc(UInt128, UInt128))
  (0...n).each { monkeys.each &.do_round(monkeys, relief) }
end

if DO_TEST
  monkeys = [
    Monkey.new([79, 98] of UInt128, 2, 3, ->(x : UInt128) { x * 19_u128 }, 23_u128),
    Monkey.new([54, 65, 75, 74] of UInt128, 2, 0, ->(x : UInt128) { x + 6 }, 19),
    Monkey.new([79, 60, 97] of UInt128, 1, 3, ->(x : UInt128) { x * x }, 13),
    Monkey.new([74] of UInt128, 0, 1, ->(x : UInt128) { x + 3 }, 17),
  ]
else
  monkeys = [
    Monkey.new([54, 89, 94] of UInt128, 5, 3, ->(x : UInt128) { x * 7 }, 17),
    Monkey.new([66, 71] of UInt128, 0, 3, ->(x : UInt128) { x + 4 }, 3),
    Monkey.new([76, 55, 80, 55, 55, 96, 78] of UInt128, 7, 4, ->(x : UInt128) { x + 2 }, 5),
    Monkey.new([93, 69, 76, 66, 89, 54, 59, 94] of UInt128, 5, 2, ->(x : UInt128) { x + 7 }, 7),
    Monkey.new([80, 54, 58, 75, 99] of UInt128, 1, 6, ->(x : UInt128) { x * 17 }, 11),
    Monkey.new([69, 70, 85, 83] of UInt128, 2, 7, ->(x : UInt128) { x + 8 }, 19),
    Monkey.new([89] of UInt128, 0, 1, ->(x : UInt128) { x + 6 }, 2),
    Monkey.new([62, 80, 58, 57, 93, 56] of UInt128, 6, 4, ->(x : UInt128) { x * x }, 13),
  ]
end

monkeys2 = monkeys.clone

do_rounds monkeys, 20, ->(x : UInt128) { x // 3 }
n_items_inspected = monkeys.map &.n_items_inspected
n_items_inspected.sort!
p! monkey_buisness = n_items_inspected[-1] * n_items_inspected[-2]

p! lcd = monkeys2.map(&.divisor).product
do_rounds monkeys2, 10000, ->(x : UInt128) { x % lcd }
n_items_inspected2 = monkeys2.map &.n_items_inspected
n_items_inspected2.sort!
p! monkey_buisness2 = n_items_inspected2[-1] * n_items_inspected2[-2]
