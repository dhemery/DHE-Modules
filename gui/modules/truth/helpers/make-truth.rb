require_relative '../../../lib/dimensions'

module Truth
  HUE = 240
  FOREGROUND_HSL = [HUE, 100, 30]
  BACKGROUND_HSL = [HUE, 100, 97]

  HEADER_ROWS = 1
  OUTCOME_COLS = 1

  OUTCOME_NAMES = %w[T F Q ¬Q]
  SELECTED_OUTCOME = 2
  CONDITION_NAMES = %w[RISE FALL EDGE HIGH LOW]
  SELECTED_CONDITION = 4
  COLUMN_1_SELECTION = 1

  CONDITION_DX = 10.16
  OUTCOME_DY = 5.08

  def make_truth(layout)
    name "TRUTH #{layout[:input_names].length}"
    hp layout[:hp]
    foreground FOREGROUND_HSL
    background BACKGROUND_HSL
    make_inputs layout
    make_table layout
    make_outputs layout
  end

  def make_inputs(layout)
    input_names = layout[:input_names]
    x = layout[:input_x]
    top = layout[:input_top]
    dy = layout[:port_dy]
    input_names.each_with_index do |input_name, i|
      input_name = '    ' + input_name + '/GATE' if i == input_names.length - 1
      input_button_port x: x, y: top + i * dy, label: input_name, label_size: :large
    end
  end

  def make_table(layout)
    input_names = layout[:input_names]
    input_cols = input_names.length
    input_state_combinations = layout[:input_state_combinations]
    condition_y = layout[:condition_y]
    outcome_x = layout[:outcome_x]
    dx = CONDITION_DX
    dy = OUTCOME_DY
    outcome_top = condition_y + dy
    condition_left = outcome_x - input_cols * dx

    truth_rows = input_state_combinations.length
    table_rows = HEADER_ROWS + truth_rows
    table_cols = input_cols + OUTCOME_COLS
    table_top = condition_y - 0.5 * dy
    table_left = condition_left - 0.5 * dx
    table_right = table_left + table_cols * dx
    table_bottom = table_top + table_rows * dy

    box left: table_left, right: table_right, top: table_top, bottom: table_bottom, corner_radius: 0
    box left: table_left, right: table_right, top: table_top, bottom: table_top + dy, corner_radius: 0, fill: @foreground
    box left: table_right - dx, right: table_right, top: table_top, bottom: table_bottom, corner_radius: 0, fill: @foreground
    (1...table_cols).each do |col|
      line_x = table_left + col * dx
      line x1: line_x, x2: line_x, y1: table_top, y2: table_bottom
    end
    (1...table_rows).each do |row|
      line_y = table_top + row * dy
      line x1: table_left, x2: table_right, y1: line_y, y2: line_y
    end

    label_y = condition_y
    (0...input_cols - 1).each do |input|
      label x: condition_left + input * dx, y: label_y, size: :large, text: input_names[input], alignment: :center, color: @background
    end
    stepper x: outcome_x - dx, y: condition_y, size: :large, name: 'gate-mode', options: CONDITION_NAMES, selection: SELECTED_CONDITION, width: 9
    label x: outcome_x, y: condition_y, size: :large, text: 'Q', alignment: :center, color: @background

    input_state_combinations.each_with_index do |input_state_combination, row|
      input_state_combination.each_with_index do |input_state, col|
        y = outcome_top + row * dy
        x = condition_left + col * dx
        label x: x, y: y, size: :large, text: input_state, alignment: :center
      end
    end

    (0...truth_rows).each do |row|
      y = outcome_top + row * dy
      stepper x: outcome_x, y: y, name: 'outcome', size: :large, options: OUTCOME_NAMES, selection: SELECTED_OUTCOME, width: 9
    end

  end

  def make_outputs(layout)
    x = layout[:output_x]
    top = layout[:output_top]
    dy = layout[:port_dy]
    output_button_port(x: x, y: top, label: 'Q', label_size: :large)
    output_button_port(x: x, y: top + dy, label: '¬ Q', label_size: :large)
  end
end
