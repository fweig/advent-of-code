# https://adventofcode.com/2022/day/12

DO_TEST = false

struct Point
  property x : Int32
  property y : Int32
  getter x, y

  def initialize(@x : Int32, @y : Int32)
  end

  def ==(other : Point) : Bool
    @x == other.x && @y == other.y
  end

  def to_s(io)
    io << "(#{@x}, #{@y})"
  end
end

class Array2D
  property width : Int32
  property height : Int32
  getter width, height

  def initialize(@width : Int32, @height : Int32)
    @data = Array(Int32).new @width * @height, 0
  end

  def [](point : Point) : Int32
    @data[index point.x, point.y]
  end

  def [](x : Int32, y : Int32) : Int32
    @data[index x, y]
  end

  def []=(point : Point, val : Int32)
    @data[index point.x, point.y] = val
  end

  def []=(x : Int32, y : Int32, val : Int32)
    @data[index x, y] = val
  end

  def edge(x : Int32, y : Int32) : Bool
    x <= 0 || x >= @width - 1 || y <= 0 || y >= @height - 1
  end

  def outside(x : Int32, y : Int32) : Bool
    x < 0 || x >= @width || y < 0 || y >= @height
  end

  def outside(point : Point) : Bool
    outside point.x, point.y
  end

  private def index(x : Int32, y : Int32) : Int32
    y * @width + x
  end
end

class HillClimb
  property terrain : Array2D
  property start_x : Int32
  property start_y : Int32
  property end_x : Int32
  property end_y : Int32
  getter terrain, start_x, start_y, end_x, end_y

  def initialize(@terrain, @start_x, @start_y, @end_x, @end_y)
  end

  def HillClimb.parse(input : String) : HillClimb
    lines = input.lines
    width = lines[0].size
    height = lines.size
    terrain = Array2D.new width, height
    start_x = 0
    start_y = 0
    end_x = 0
    end_y = 0
    lines.each_with_index do |line, y|
      line.each_char_with_index do |char, x|
        case char
        when 'S'
          terrain[x, y] = 'a' - 'a'
          start_x = x
          start_y = y
        when 'E'
          terrain[x, y] = 'z' - 'a'
          end_x = x
          end_y = y
        else
          terrain[x, y] = char.ord - 'a'.ord
        end
      end
    end
    HillClimb.new terrain, start_x, start_y, end_x, end_y
  end

  def compute_distances(flexible_start : Bool = false) : Int32
    distances = Array2D.new terrain.width, terrain.height
    distances.@data.fill Int32::MAX

    queue = [] of Point

    if flexible_start
      (0...terrain.width).each do |x|
        (0...terrain.height).each do |y|
          if terrain[x, y] == 0
            distances[x, y] = 0
            queue.push Point.new x, y
          end
        end
      end
    else
      distances[@start_x, @start_y] = 0
      queue.push Point.new @start_x, @start_y
    end

    until queue.empty?
      pos = queue.shift

      distance_from_start = distances[pos] + 1

      pos_up = pos
      pos_up.y -= 1
      unless distances.outside(pos_up) || !can_climb(pos, pos_up) || distances[pos_up] <= distance_from_start
        distances[pos_up] = distance_from_start
        queue.push pos_up
      end

      pos_down = pos
      pos_down.y += 1
      unless distances.outside(pos_down) || !can_climb(pos, pos_down) || distances[pos_down] <= distance_from_start
        distances[pos_down] = distance_from_start
        queue.push pos_down
      end

      pos_left = pos
      pos_left.x -= 1
      unless distances.outside(pos_left) || !can_climb(pos, pos_left) || distances[pos_left] <= distance_from_start
        distances[pos_left] = distance_from_start
        queue.push pos_left
      end

      pos_right = pos
      pos_right.x += 1
      unless distances.outside(pos_right) || !can_climb(pos, pos_right) || distances[pos_right] <= distance_from_start
        distances[pos_right] = distance_from_start
        queue.push pos_right
      end
    end

    distances[@end_x, @end_y]
  end

  def can_climb(from : Point, to : Point) : Bool
    return false if terrain.outside(from) || terrain.outside(to)
    return terrain[to] - terrain[from] <= 1
  end
end

input = File.read(DO_TEST ? "test_input.txt" : "input.txt")
hc = HillClimb.parse input

p! hc.compute_distances
p! hc.compute_distances true
