require_relative 'helpers/make-truth'

self.class::include Truth

HP = 8
CONDITION_Y = 25
OUTCOME_X = 30.48
PORT_DY = 15.5
INPUT_TOP = 85
OUTPUT_TOP = INPUT_TOP
OUTPUT_X = 30.48

LAYOUT = {
  hp: HP,
  input_names: %w[A B],
  input_state_combinations: [
    %w[F F],
    %w[F T],
    %w[T F],
    %w[T T],
  ],
  condition_y: CONDITION_Y,
  outcome_x: OUTCOME_X,
  input_top: INPUT_TOP,
  output_top: OUTPUT_TOP,
  output_x: OUTPUT_X,
  port_dy: PORT_DY,
}

make_truth LAYOUT
