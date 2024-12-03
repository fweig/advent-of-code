# https://adventofcode.com/2022/day/9

def parse_commands(input : String, n_knots : Int32) : Int32
  knots_x = Array(Int32).new n_knots, 0
  knots_y = Array(Int32).new n_knots, 0
  dx = dy = 0
  tail_positions = Set({Int32, Int32}).new

  # Add initial 0,0 position
  tail_positions.add({knots_x[-1], knots_y[-1]})

  input.each_line { |l|
    direction = l[0]
    nsteps = l[2..].to_i

    case direction
    when 'R'
      dx = 1
      dy = 0
    when 'L'
      dx = -1
      dy = 0
    when 'U'
      dx = 0
      dy = 1
    when 'D'
      dx = 0
      dy = -1
    else raise "Unknown direction '#{direction}'"
    end

    (0...nsteps).each {
      knots_x[0] += dx
      knots_y[0] += dy

      (1...n_knots).each { |i|
        dist_x = knots_x[i - 1] - knots_x[i]
        dist_y = knots_y[i - 1] - knots_y[i]

        if knots_x[i - 1] == knots_x[i] && dist_y.abs >= 2
          knots_y[i] += dist_y.sign
        elsif knots_y[i - 1] == knots_y[i] && dist_x.abs >= 2
          knots_x[i] += dist_x.sign
        elsif dist_y.abs >= 2 || dist_x.abs >= 2
          knots_x[i] += dist_x.sign
          knots_y[i] += dist_y.sign
        end
      }

      tail_positions.add({knots_x[-1], knots_y[-1]})
    }
  }
  # p! tail_positions
  tail_positions.size
end

input = File.read "input.txt"
p! parse_commands input, 2
p! parse_commands input, 10
