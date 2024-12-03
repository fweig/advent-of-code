# https://adventofcode.com/2022/day/6

input = File.read "input.txt"
p! input

def first_unique_sequence_end(input : String, length : Int32) : Int32
  pos = 0

  while pos < input.size - length
    buffer = Set(Char).new input[pos, length].chars
    if buffer.size == length
      break
    end
    pos += 1
  end

  pos + length
end

p! first_unique_sequence_end input, 4
p! first_unique_sequence_end input, 14
