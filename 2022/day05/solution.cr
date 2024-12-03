# https://adventofcode.com/2022/day/5

N_STACKS = 9

alias CrateStack = Array(Char)

class Crates
  def initialize(nstacks : Int32 = N_STACKS)
    @stacks = Array(CrateStack).new

    (0...nstacks).each { |_|
      @stacks.push CrateStack.new
    }
  end

  def top_of_all_stacks : String
    @stacks.reduce "" { |acc, stack|
      acc + stack[-1]
    }
  end

  def push(stack : Int32, value : Char)
    @stacks[stack].push value
  end

  def push(stack : Int32, values : Array(Char))
    @stacks[stack] += values
  end

  def pop(stack : Int32) : Char
    @stacks[stack].pop
  end

  def pop(stack : Int32, n : Int32) : Array(Char)
    @stacks[stack].pop n
  end

  def parse_and_exec1(command : String)
    _, amount, _, source_stack, _, target_stack = command.split

    amount = amount.to_i
    source_stack = source_stack.to_i - 1
    target_stack = target_stack.to_i - 1

    # p! top_of_all_stacks

    (0...amount).each { |_| push target_stack, pop source_stack }
  end

  def parse_and_exec2(command : String)
    _, amount, _, source_stack, _, target_stack = command.split

    amount = amount.to_i
    source_stack = source_stack.to_i - 1
    target_stack = target_stack.to_i - 1

    # p! top_of_all_stacks

    push target_stack, (pop source_stack, amount)
  end
end

def Crates.parse(input : String) : Crates
  lines = input.each_line.to_a
  lines = lines.reverse

  crates = Crates.new

  lines.each { |line|
    line
    pos = 0
    crate = 0
    loop do
      case line[pos]
      when ' '
        nil
      when '['
        crates.push crate, line[pos + 1]
      else raise "Read invalid character '#{line[pos]}"
      end
      pos += 4
      crate += 1
      break unless pos < line.size
    end
  }

  crates
end

input = File.read "crates.txt"
crates = Crates.parse input

p! crates.@stacks
p! crates.top_of_all_stacks

commands = File.read "commands.txt"

# solution part 2
commands.each_line { |command| crates.parse_and_exec2 command }
p! crates.top_of_all_stacks
