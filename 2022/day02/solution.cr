# https://adventofcode.com/2022/day/2

enum RPS
  Rock
  Paper
  Scissor
end

enum Outcome
  Win
  Loss
  Draw
end

def move_for_outcome(opponent_move : RPS, desired : Outcome) : RPS
  case desired
  when .draw?
    opponent_move
  when .win?
    case opponent_move
    when .rock?    then RPS::Paper
    when .paper?   then RPS::Scissor
    when .scissor? then RPS::Rock
    else                raise "Unhandled value for RPS"
    end
  when .loss?
    case opponent_move
    when .rock?    then RPS::Scissor
    when .paper?   then RPS::Rock
    when .scissor? then RPS::Paper
    else                raise "Unhandled value for RPS"
    end
  else
    raise "Unhandled value for Outcome"
  end
end

class RPSGame
  def initialize(@opponents_choice : RPS, @my_choice : RPS)
  end

  def score : Int32
    shape_score =
      case @my_choice
      when .rock?    then 1
      when .paper?   then 2
      when .scissor? then 3
      else                raise "Unhandled value for RPS"
      end

    outcome_score =
      case outcome
      when .win?  then 6
      when .draw? then 3
      when .loss? then 0
      else             raise "Unhandled value for Outcome"
      end

    shape_score + outcome_score
  end

  def outcome : Outcome
    case @my_choice
    when @opponents_choice
      Outcome::Draw
    when move_for_outcome @opponents_choice, Outcome::Win
      Outcome::Win
    else
      Outcome::Loss
    end
  end
end

def to_rps_game(str : String) : RPSGame
  opponents_choice_encrypted = str[0]
  opponents_choice =
    if opponents_choice_encrypted == 'A'
      RPS::Rock
    elsif opponents_choice_encrypted == 'B'
      RPS::Paper
    else
      RPS::Scissor
    end

  my_choice_encrypted = str[-1]
  my_choice =
    if my_choice_encrypted == 'X'
      RPS::Rock
    elsif my_choice_encrypted == 'Y'
      RPS::Paper
    else
      RPS::Scissor
    end

  RPSGame.new opponents_choice, my_choice
end

# TODO: Lots of code duplication with to_rps_game
def to_rps_game2(str : String) : RPSGame
  opponents_choice_encrypted = str[0]
  opponents_choice =
    case opponents_choice_encrypted
    when 'A' then RPS::Rock
    when 'B' then RPS::Paper
    when 'C' then RPS::Scissor
    else          raise "Invalid input #{opponents_choice_encrypted}"
    end

  desired_outcome_encrypted = str[-1]
  desired_outcome =
    case desired_outcome_encrypted
    when 'X' then Outcome::Loss
    when 'Y' then Outcome::Draw
    when 'Z' then Outcome::Win
    else          raise "Invalid input #{desired_outcome_encrypted}"
    end

  RPSGame.new opponents_choice, (move_for_outcome opponents_choice, desired_outcome)
end

test_input =
  "A Y
B X
C Z"

input = File.read "input.txt"

# Solution part 1
p! input
  .each_line
  .map { |x| (to_rps_game x).score }
  .sum

# Solution part 2
p! input
  .each_line
  .map { |x| (to_rps_game2 x).score }
  .sum
