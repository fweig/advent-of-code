# https://adventofcode.com/2022/day/8

class Array2D
  def initialize(@width : Int32, @height : Int32)
    @data = Array(Int32).new @width * @height, 0
  end

  def [](x : Int32, y : Int32) : Int32
    @data[index x, y]
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

  private def index(x : Int32, y : Int32) : Int32
    y * @width + x
  end
end

def set_height(v : Int32, h : Int32) : Int32
  v & 0b10000 | height h
end

def height(v : Int32) : Int32
  v & 0b01111
end

def make_visible(v : Int32) : Int32
  # Keep the visible bit in v on purpose,
  # in case the tree is already visible from another direction
  v | 0b10000
end

def is_visible(v : Int32) : Bool
  v & 0b10000 != 0
end

def parse(input : String) : Array2D
  lines = input.each_line.to_a

  p! width = lines[0].size
  p! height = lines.size

  grid = Array2D.new(width, height)

  y = 0
  lines.each { |l|
    x = 0
    l.each_char { |c|
      grid[x, y] = height(c - '0')
      x += 1
    }
    y += 1
  }

  grid
end

def update_visibility(x : Int32, y : Int32, grid : Array2D, max_tree_heights : Array(Int32), scan_vertical : Bool)
  val = grid[x, y]
  h = height val
  # p! x, y, val, h, max_tree_heights[scan_vertical ? x : y], ""
  max_tree_height_in_direction = max_tree_heights[scan_vertical ? x : y]
  visible = h > max_tree_height_in_direction || grid.edge x, y
  # visible = grid.edge x, y
  if visible
    grid[x, y] = make_visible val
  end
  max_tree_heights[scan_vertical ? x : y] = Math.max(h, max_tree_height_in_direction)
end

def scan(grid : Array2D)
  max_tree_heights = Array(Int32).new grid.@width, 0

  # Check if trees visible from top
  (0...grid.@height).each { |y|
    (0...grid.@width).each { |x|
      update_visibility x, y, grid, max_tree_heights, true
    }
  }

  # p! grid

  # Check if trees visible from bottom
  max_tree_heights.fill 0
  (0...grid.@height).reverse_each { |y|
    (0...grid.@width).each { |x|
      update_visibility x, y, grid, max_tree_heights, true
    }
  }

  # p! grid

  # Check if trees visible from left
  max_tree_heights.fill 0
  (0...grid.@width).each { |x|
    (0...grid.@height).each { |y|
      update_visibility x, y, grid, max_tree_heights, false
    }
  }

  # p! grid

  # Check if trees visible from right
  max_tree_heights.fill 0
  (0...grid.@width).reverse_each { |x|
    (0...grid.@height).each { |y|
      update_visibility x, y, grid, max_tree_heights, false
    }
  }
end

def tree_score_1direction(grid : Array2D, x : Int32, y : Int32, dx : Int32, dy : Int32) : Int32
  score = 0
  h = height grid[x, y]

  x += dx
  y += dy

  until grid.outside x, y
    h_other = height grid[x, y]

    score += 1

    if h_other >= h
      break
    end

    x += dx
    y += dy
  end

  score
end

def best_tree_score(grid : Array2D) : Int32
  best_score : Int32 = 0

  (1...grid.@height - 1).each { |y|
    (1...grid.@width - 1).each { |x|
      score_left = tree_score_1direction grid, x, y, -1, 0
      score_right = tree_score_1direction grid, x, y, 1, 0
      score_up = tree_score_1direction grid, x, y, 0, -1
      score_down = tree_score_1direction grid, x, y, 0, 1

      best_score = Math.max best_score, score_left * score_right * score_up * score_down

      # p! x, y, score_left, score_right, score_down, score_up, best_score
    }
  }

  best_score
end

input = File.read "input.txt"

grid = parse input
# p! grid

scan(grid)
# p! grid

p! grid.@data.count { |x| is_visible x }
p! best_tree_score grid
