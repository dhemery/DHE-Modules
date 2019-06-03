name 'RANGER'
hp 6
foreground [60, 100, 15]
background [60, 100, 97]

left = 1.5 * MM_PER_HP
right = width - left
center = (left + right) / 2.0

y = 14.5
delta_y = 15.0

separator(y: y)

y += delta_y * 0.75

medium_knob(x: left, y: y, label: 'LEVEL')
output_port(x: right, y: y)

y += delta_y
cv_port(x: left, y: y)
attenuverter(x: right, y: y)

y += delta_y / 2.0
limit_top = y
separator(y: y)
y += delta_y * 0.75

medium_knob(x: left, y: y, label: 'CCW')
medium_knob(x: right, y: y, label: 'CW')

y += delta_y
attenuverter(x: left, y: y)
attenuverter(x: right, y: y)

y += delta_y
cv_port(x: left, y: y)
cv_port(x: right, y: y)

y += delta_y
polarity_toggle(x: left, y: y)
polarity_toggle(x: right, y: y)

y += delta_y * 0.6
limit_bottom = y
separator(y: limit_bottom)
line(x1:center, y1: limit_top, x2: center, y2: limit_bottom)
