name 'TRUTH 4'
hp 16

hue = 240
foreground [hue, 100, 30]
background [hue, 100, 97]

top = i2mm(1.1)
dy = i2mm(0.6)

port_x = i2mm(0.4)

a_box = input_button_port x: port_x, y: top + 0 * dy, label: 'A'
input_button_port x: port_x, y: top + 1 * dy, label: 'B'
input_button_port x: port_x, y: top + 2 * dy, label: 'C'
input_button_port x: port_x, y: top + 3 * dy, label: 'D'

output_port(x: port_x, y: top + 4 * dy, label: 'Q')
qnot_box = output_port(x: port_x, y: top + 5 * dy, label: '¬ Q')

table_values = %w[FFFF FFFT FFTF FFTT FTFF FTFT FTTF FTTT TFFF TFFT TFTF TFTT TTFF TTFT TTTF TTTT]
truth_rows = table_values.length
table_rows = truth_rows + 2
table_cols = 5
table_left = i2mm(1.1)
table_right = @width - a_box.left
table_dx = (table_right - table_left) / table_cols
table_top = a_box.top
table_bottom = qnot_box.bottom
table_dy = (table_bottom - table_top) / table_rows

box left: table_left, right: table_right, top: table_top, bottom: table_bottom, corner_radius: 0
box left: table_right - table_dx, right: table_right, top: table_top, bottom: table_bottom, corner_radius: 0, fill: @foreground

label x: table_left + 0.5 * table_dx, y: table_top + 0.5 * table_dy, size: :large, text: 'A', alignment: :center
label x: table_left + 1.5 * table_dx, y: table_top + 0.5 * table_dy, size: :large, text: 'B', alignment: :center
label x: table_left + 2.5 * table_dx, y: table_top + 0.5 * table_dy, size: :large, text: 'C', alignment: :center
label x: table_left + 3.5 * table_dx, y: table_top + 0.5 * table_dy, size: :large, text: 'D', alignment: :center
label x: table_left + 4.5 * table_dx, y: table_top + 1 * table_dy, size: :title, text: 'Q', alignment: :center, color: @background

(1...table_cols - 1).each do |col|
  line_x = table_left + col * table_dx
  line x1: line_x, x2: line_x, y1: table_top, y2: table_bottom
end
line x1: table_right, x2: table_right, y1: table_top, y2: table_bottom

(2...table_rows).each do |row|
  line_y = table_top + row * table_dy
  line x1: table_left, x2: table_right, y1: line_y, y2: line_y
end

table_values.each_with_index do |table_value, row|
  table_value.split('').each_with_index do |letter, col|
    y = table_top + (row + 2) * table_dy + table_dy / 2
    x = table_left + col * table_dx + table_dx / 2
    label x: x, y: y, size: :large, text: letter, alignment: :center
  end
end

(2...table_rows).each do |row|
  option_x = table_right - table_dx / 2
  option_y = table_top + row * table_dy + table_dy / 2
  stepper x: option_x, y: option_y, name: 'outcome', size: :large, options: %w[F T], selection: 2, width: 8
end

(0...table_cols - 1).each do |col|
  option_x = table_left + col * table_dx + table_dx / 2
  option_y = table_top + table_dy * 1.4
  stepper x: option_x, y: option_y, name: 'condition', options: %w[RISE FALL EDGE HIGH LOW], selection: 4, width: 8
end

