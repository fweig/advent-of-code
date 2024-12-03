# https://adventofcode.com/2022/day/10

struct Noop
end

struct AddX
  getter operand

  def initialize(@operand : Int32)
  end
end

alias Op = AddX | Noop

class VM
  property reg_x : Int32 = 1
  private getter reg_x
  private setter reg_x

  property signal_strength_aggr : Int32 = 0
  getter signal_strength_aggr
  private setter signal_strength_aggr

  property cycle : Int32 = 0
  private getter cycle
  private setter cycle

  property crt : Array(String) = [] of String
  getter crt

  def exec(commands : Array(Op))
    commands.each { |op| exec_command op }
  end

  def exec_command(op : Op)
    case op
    in AddX then active_op_cycles = 2
    in Noop then active_op_cycles = 1
    end

    while active_op_cycles > 0
      self.cycle += 1
      if p! ([20, 60, 100, 140, 180, 220].index @cycle) != nil
        p! op
        p! active_op_cycles
        p! reg_x
        p! @signal_strength_aggr += cycle * reg_x
      end
      pixel_drawn = cycle % 40
      if pixel_drawn == 1
        crt.push ""
      end
      crt[-1] +=
        if (pixel_drawn - (reg_x + 1)).abs <= 1
          '#'
        else
          '.'
        end
      active_op_cycles -= 1
      if active_op_cycles == 0
        case op
        in AddX then self.reg_x += op.operand
        in Noop then nil
        end
      end
    end
  end
end

def parse_ops(input : String) : Array(Op)
  input.each_line.map { |l|
    case l
    when .starts_with? "noop"
      Noop.new
    when .starts_with? "addx"
      _, operand = l.split
      AddX.new operand.to_i
    else
      raise "Unknown op '#{l}"
    end
  }
    .to_a
end

vm = VM.new
input = File.read "input.txt"
ops = parse_ops input
vm.exec ops
p! vm.signal_strength_aggr

vm.crt.each { |line| puts line }
