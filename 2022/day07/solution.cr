# https://adventofcode.com/2022/day/7

class Directory
  property root : Directory

  def initialize(@name : String, @root = self)
    @files = Array({String, Int32}).new
    @subdirs = Array(Directory).new
  end

  def add_file(name : String, size : Int32)
    f = {name, size}
    @files.push f
  end

  def add_subdir(dir : Directory)
    dir.set_root self
    @subdirs.push dir
  end

  def get_subdir(name : String) : Directory
    @subdirs.each { |dir|
      if dir.@name == name
        return dir
      end
    }
    raise "Couldn't find subdirectory #{name}"
  end

  def set_root(dir : Directory)
    @root = dir
  end

  def size : Int32
    @files.sum { |x, y| x[0] + x[1] }
  end

  def print(indent : Int32 = 0)
    puts " "*indent + @name + "(" + abs_dir_sizes[-1].to_s + ")"
    indent += 2
    @files.each { |f|
      puts " "*indent + "> " + f[0] + " " + f[1].to_s
    }
    @subdirs.each { |dir|
      dir.print indent
    }
  end

  def abs_dir_sizes : Array(Int32)
    my_size : Int32 = @files.sum 0 { |f| f[1] }
    dir_sizes = Array(Int32).new
    @subdirs.each { |dir|
      subdir_sizes = dir.abs_dir_sizes
      my_size += subdir_sizes[-1] # Last element holds the total size of this subfolder
      dir_sizes += subdir_sizes
    }
    dir_sizes.push my_size
  end

  def used_space : Int32
    used : Int32 = @files.sum 0 { |f| f[1] }
    @subdirs.each { |dir|
      used += dir.used_space
    }
    used
  end
end

def parse(shell : String) : Directory
  lines = shell.each_line.to_a

  pos = 0

  root = Directory.new "/"
  current_dir = root

  while pos < lines.size
    l = lines[pos]
    unless l.starts_with? "$ "
      raise "Line '#{l}' not a command"
    end
    tokens = l.split
    command = tokens[1]

    # p! l

    case command
    when "cd"
      next_dir = tokens[2]
      case next_dir
      when "/"
        current_dir = root
      when ".."
        current_dir = current_dir.@root
      else
        current_dir = current_dir.get_subdir next_dir
      end
      pos += 1
    when "ls"
      while true
        pos += 1
        if pos >= lines.size || lines[pos].starts_with? "$ "
          break
        end
        l = lines[pos]
        # p! l
        case l
        when .starts_with? "dir"
          prefix, dirname = l.split
          current_dir.add_subdir Directory.new dirname
        else
          size, filename = l.split
          current_dir.add_file filename, size.to_i
        end
      end
    else
      raise "Unknown command '#{command}'"
    end
  end

  root
end

input = File.read "input.txt"

root = parse input
root.print

dir_sizes = root.abs_dir_sizes

# Solution part 1
small_dir_sizes = dir_sizes.reject { |x| x > 100000 }
p! small_dir_sizes.sum

# Solution part 2
TOTAL_DISK_SPACE = 70000000
REQ_DISK_SPACE   = 30000000

p! used_disk_space = root.used_space
p! free_disk_space = TOTAL_DISK_SPACE - used_disk_space
p! needs_to_be_freed = REQ_DISK_SPACE - free_disk_space
p! canditates_to_delete = dir_sizes.reject { |x| x < needs_to_be_freed }
p! canditates_to_delete.min
