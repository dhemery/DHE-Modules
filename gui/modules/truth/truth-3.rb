require_relative 'helpers/make-truth'

self.class::include Truth

INPUTS = 3
make_background INPUTS

input_top = i2mm(3)
port_dy = i2mm(0.55)
input_x = i2mm(0.38)
output_x = @width - input_x
output_top = input_top + port_dy

make_inputs n: INPUTS, top: input_top, x: input_x, dy: port_dy
make_outputs top: output_top, x: output_x, dy: port_dy

table_x = @width / 2
table_top = i2mm(0.8)
table_bottom = table_top + i2mm(1.8)

make_table n: INPUTS, top: table_top, bottom: table_bottom, x: table_x
