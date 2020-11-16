require_relative 'helpers/make-truth'

self.class::include Truth

HP = 10
CONDITION_Y = 23
OUTCOME_X = 40.64
PORT_DY = 14.5
INPUT_TOP = 80
OUTPUT_TOP = INPUT_TOP + PORT_DY
OUTPUT_X = 40.64

LAYOUT = {
  hp: HP,
  input_names: %w[A B C],
  input_state_combinations: [
    %w[F F F],
    %w[F F T],
    %w[F T F],
    %w[F T T],
    %w[T F F],
    %w[T F T],
    %w[T T F],
    %w[T T T],
  ],
  condition_y: CONDITION_Y,
  outcome_x: OUTCOME_X,
  input_top: INPUT_TOP,
  output_top: OUTPUT_TOP,
  output_x: OUTPUT_X,
  port_dy: PORT_DY,
}

make_truth LAYOUT
