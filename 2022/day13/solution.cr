# https://adventofcode.com/2022/day/13

DO_TEST = false

alias PacketData = Array(PacketData) | Int32
alias Packet = NamedTuple(data: Array(PacketData) | Int32)
alias TriBool = Bool | Nil

def packets_ordered(left : Packet, right : Packet) : Bool
  x = packets_ordered_impl left[:data], right[:data]
  case x
  in Bool
    x
  in Nil
    true
  end
end

def packets_ordered_impl(left : PacketData, right : PacketData) : TriBool
  case left
  in Int32
    case right
    in Int32
      if left == right
        nil
      elsif left < right
        true
      else
        false
      end
    in Array(PacketData)
      packets_ordered_impl [left] of PacketData, right
    end
  in Array(PacketData)
    case right
    in Int32
      packets_ordered_impl left, [right] of PacketData
    in Array(PacketData)
      s = Math.min left.size, right.size
      s.times do |i|
        x = packets_ordered_impl left[i], right[i]
        case x
        in Bool
          return x
        in Nil
          next
        end
      end
      if left.size == right.size
        nil
      elsif left.size < right.size
        true
      else
        false
      end
    end
  end
end

def parse_packet(s : String, pos : Int32 = 0) : Tuple(PacketData, Int32)
  if s[pos] == '['
    pos += 1
    arr = [] of PacketData
    while s[pos] != ']'
      x, pos = parse_packet s, pos
      arr << x
      if s[pos] == ','
        pos += 1
      end
    end
    pos += 1
    {arr, pos}
  else
    start = pos
    stop = pos + 1
    while s[stop] != ',' && s[stop] != ']'
      stop += 1
    end
    {s[start...stop].to_i, stop}
  end
end

def parse_packet_list(s : String) : {Array(Packet), Array(Packet)}
  arr_left = [] of Packet
  arr_right = [] of Packet
  lines = s.lines
  x = 0
  while x < lines.size
    left, _ = parse_packet lines[x]
    arr_left << {data: left}
    x += 1
    right, _ = parse_packet lines[x]
    arr_right << {data: right}
    x += 2 # Skip blank line
  end
  {arr_left, arr_right}
end

if DO_TEST
  # p0 = [1, 1, 3, 1, 1] of Packet
  # p1 = [1, 1, 5, 1, 1] of Packet
  # p! packets_ordered p0, p1

  # p0 = [[1] of Packet, [2, 3, 4] of Packet] of Packet
  # p1 = [[1] of Packet, 4] of Packet
  # p! packets_ordered p0, p1

  # p0 = [9] of Packet
  # p1 = [[8, 7, 6] of Packet] of Packet
  # p! packets_ordered p0, p1

  # p! parse_packet "[1,2,3]"
  # p! parse_packet "[1,[2,3]]"
  # p! parse_packet "[1,[2,3],4]"
  # p! parse_packet "[1,[2,3],4,[5,6]]"
  # p! parse_packet "[[[]]]"
else
  input = File.read "input.txt"
  packets_left, packets_right = parse_packet_list input
  correct_score = 0
  (0...packets_left.size).each do |i|
    if packets_ordered packets_left[i], packets_right[i]
      correct_score += i + 1
    end
  end
  # Solution part 1
  p! correct_score

  # Solution part 2
  divider_packet1 = {data: [[2] of PacketData] of PacketData}
  divider_packet2 = {data: [[6] of PacketData] of PacketData}
  all_packets = packets_left + packets_right
  all_packets << divider_packet1
  all_packets << divider_packet2

  all_packets.sort! do |left, right|
    if packets_ordered left, right
      -1
    else
      1
    end
  end

  p! all_packets
  p! (all_packets.index! divider_packet1) + 1
  p! (all_packets.index! divider_packet2) + 1
  p! decoder_key = ((all_packets.index! divider_packet1) + 1) * ((all_packets.index! divider_packet2) + 1)
end
