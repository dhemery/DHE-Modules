require_relative '../../../lib/shapes/position-marker'
require_relative '../../../lib/dimensions'

module Truth
  HUE = 240
  FOREGROUND_HSL = [HUE, 100, 30]
  BACKGROUND_HSL = [HUE, 100, 97]

  TRUTH = {
    3 => {
      hp: 10,
      inputs: 'ABC',
      patterns: %w[FFF FFT FTF FTT TFF TFT TTF TTT],
    },
    4 => {
      hp: 16,
      inputs: 'ABCD',
      patterns: %w[FFFF FFFT FFTF FFTT FTFF FTFT FTTF FTTT TFFF TFFT TFTF TFTT TTFF TTFT TTTF TTTT],
    },
  }

  def make_background(inputs)
    name "TRUTH #{inputs}"
    hp TRUTH[inputs][:hp]
    foreground FOREGROUND_HSL
    background BACKGROUND_HSL
  end

  def make_table(n:, x:, top:, bottom:)
    header_rows = 2
    outcome_cols = 1
    pattern_rows = TRUTH[n][:patterns].length
    table_rows = header_rows + pattern_rows
    table_cols = n + outcome_cols
    dx = i2mm(0.4)
    dy = (bottom - top) / table_rows
    width = table_cols * dx
    left = x - (width / 2)
    right = x + (width / 2)

    box left: left, right: right, top: top, bottom: bottom, corner_radius: 0
    box left: right - dx, right: right, top: top, bottom: bottom, corner_radius: 0, fill: @foreground

    (0...n).each do |col|
      label x: left + (col + 0.5) * dx, y: top + 0.5 * dy, size: :large, text: TRUTH[n][:inputs][col], alignment: :center
    end
    label x: left + (n + 0.5) * dx, y: top + 1 * dy, size: :title, text: 'Q', alignment: :center, color: @background

    (1...table_cols - 1).each do |col|
      line_x = left + col * dx
      line x1: line_x, x2: line_x, y1: top, y2: bottom
    end
    line x1: right, x2: right, y1: top, y2: bottom

    (2...table_rows).each do |row|
      line_y = top + row * dy
      line x1: left, x2: right, y1: line_y, y2: line_y
    end

    TRUTH[n][:patterns].each_with_index do |pattern, row|
      pattern.split('').each_with_index do |letter, col|
        y = top + (row + 2) * dy + dy / 2
        x = left + col * dx + dx / 2
        label x: x, y: y, size: :large, text: letter, alignment: :center
      end
    end

    (2...table_rows).each do |row|
      option_x = right - dx / 2
      option_y = top + row * dy + dy / 2
      stepper x: option_x, y: option_y, name: 'outcome', size: :large, options: %w[F T], selection: 2, width: 8
    end

    (0...table_cols - 1).each do |col|
      option_x = left + col * dx + dx / 2
      option_y = top + dy * 1.4
      stepper x: option_x, y: option_y, name: 'condition', options: %w[RISE FALL EDGE HIGH LOW], selection: 4, width: 8
    end
  end

  def make_inputs(n:, x:, top:, dy:)
    bounding_boxes = (0...n).map do |i|
      input_button_port x: x, y: top + i * dy, label: TRUTH[n][:inputs][i]
    end
    {
      top: bounding_boxes[0].top,
      right: bounding_boxes[0].right,
      bottom: bounding_boxes[n - 1].right,
      left: bounding_boxes[0].right,
    }

  end

  def make_outputs(x:, top:, dy:)
    q_box = output_port(x: x, y: top, label: 'Q')
    qnot_box = output_port(x: x, y: top + dy, label: '¬ Q')
    {
      top: q_box.top,
      right: q_box.right,
      bottom: qnot_box.bottom,
      left: q_box.left,
    }
  end
end