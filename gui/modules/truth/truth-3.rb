name 'TRUTH 3'
hp 10

hue = 240
foreground [hue, 100, 30]
background [hue, 100, 97]

input_top = i2mm(3)
input_dy = i2mm(0.6)

input_x = i2mm(0.4)
output_x = @width - i2mm(0.4)

a_box = input_button_port x: input_x, y: input_top + 0 * input_dy, label: 'A'
input_button_port x: input_x, y: input_top + 1 * input_dy, label: 'B'
input_button_port x: input_x, y: input_top + 2 * input_dy, label: 'C'

q_box = output_port(x: output_x, y: input_top + 1 * input_dy, label: 'Q')
output_port(x: output_x, y: input_top + 2 * input_dy, label: '¬ Q')

condition_patterns = %w[FFF FFT FTF FTT TFF TFT TTF TTT]
outcome_rows = condition_patterns.length
condition_cols = condition_patterns[0].length
header_rows = 2
outcome_cols = 1
table_rows = header_rows + outcome_rows
table_cols = condition_cols + outcome_cols
table_left = a_box.left
table_right = q_box.right
table_dx = (table_right - table_left) / table_cols
table_bottom = a_box.top - i2mm(0.15)
table_top = table_bottom - i2mm(1.9)
table_dy = (table_bottom - table_top) / table_rows

box left: table_left, right: table_right, top: table_top, bottom: table_bottom, corner_radius: 0
box left: table_right - table_dx, right: table_right, top: table_top, bottom: table_bottom, corner_radius: 0, fill: @foreground

label x: table_left + 0.5 * table_dx, y: table_top + 0.5 * table_dy, size: :large, text: 'A', alignment: :center
label x: table_left + 1.5 * table_dx, y: table_top + 0.5 * table_dy, size: :large, text: 'B', alignment: :center
label x: table_left + 2.5 * table_dx, y: table_top + 0.5 * table_dy, size: :large, text: 'C', alignment: :center
label x: table_left + 3.5 * table_dx, y: table_top + 1 * table_dy, size: :title, text: 'Q', alignment: :center, color: @background

(1...table_cols - 1).each do |col|
  line_x = table_left + col * table_dx
  line x1: line_x, x2: line_x, y1: table_top, y2: table_bottom
end
line x1: table_right, x2: table_right, y1: table_top, y2: table_bottom

(0...outcome_rows).each do |row|
  line_y = table_top + (row + header_rows) * table_dy
  line x1: table_left, x2: table_right, y1: line_y, y2: line_y
end

condition_patterns.each_with_index do |pattern, row|
  pattern.split('').each_with_index do |condition_value, col|
    y = table_top + (row + 2) * table_dy + table_dy / 2
    x = table_left + col * table_dx + table_dx / 2
    label x: x, y: y, size: :large, text: condition_value, alignment: :center
  end
end

(0...condition_cols).each do |col|
  option_x = table_left + col * table_dx + table_dx / 2
  option_y = table_top + table_dy * 1.4
  stepper x: option_x, y: option_y, name: 'condition', options: %w[RISE FALL EDGE HIGH LOW], selection: 4, width: 8
end

(0...outcome_rows).each do |row|
  option_x = table_right - table_dx / 2
  option_y = table_top + (row + header_rows) * table_dy + table_dy / 2
  stepper x: option_x, y: option_y, name: 'outcome', size: :large, options: %w[F T], selection: 2, width: 8
end
