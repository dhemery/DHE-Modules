name 'SWAVE'
hp 4
foreground [16, 100, 50]
background [16, 100, 97]

y = 25.0
dy = 18.5

x = width / 2.0

shape_toggle x: x, y: y

y += dy
large_knob(x: x, y: y, label: 'CURVE')

y += dy
cv_port(x: x, y: y)

y = 82.0
dy = 15.0

y += dy;
input_port(x: x, y: y)

y += dy
output_port(x: x, y: y)
